#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>
#include <vector>
#include <unordered_map>

/* Find Hamiltonian path in fully connected graph
 */

using namespace std;

class AdjacencyMatrix {
    vector<vector<int>> _data;
public:
    size_t size() const { return _data.size(); }
    int operator()(int i, int j) const {
        assert(i != j);
        return _data[i][j] + _data[j][i];
    }
    void sanity_check() const {
        int sz = _data.size();
        // Check square matrix
        for (int i = 0; i < sz; i++) {
            assert(_data[i].size() == sz);
        }
    }
    void print(vector<string>& names) const {
        for (int i = 0; i < size(); i++) {
            for (int j = 0; j < size(); j++) {
                if (i != j)
                    cout << names[i] << ":" << names[j] << " = " << _data[i][j] << endl;
            }
        }        
    }

    void add_edge(int i, int j, int val) {
        // Assumes the final graph is fully connected!
        int m = max(i,j);
        if (m >= _data.size()) _data.resize(m+1);
        if (m >= _data[i].size()) _data[i].resize(m+1);
        if (m >= _data[j].size()) _data[j].resize(m+1);
        _data[i][j] = val;
    }

    // Returns the cost of the min Hamiltonian cycle
    // Brute force enumeration, BFS with backtracking
    int min_hcycle_helper(int start, int i, vector<bool>& visited,
                         int num_visited=0, int cost=0) const {
        // Base case
        if (num_visited+1 == size())
            return cost + (*this)(i,start);

        // visit i
        visited[i] = true;
        
        // Fully connected node so i connects to all
        int min_cost = INT32_MAX;
        for (int j = 0; j < size(); j++) {
            if (!visited[j]) {
                int new_cost = min_hcycle_helper(
                    start, j, visited, num_visited+1, cost + (*this)(i,j));
                min_cost = min(min_cost, new_cost);
            }
        }

        visited[i] = false;
        return min_cost;
    }

    // Return the cost of the minimum Hamiltonian cycle
    int min_hcycle() const {
        vector<bool> visited(size(), false);
        int cost = min_hcycle_helper(0, 0, visited);
        return cost;
    }
};

bool is_number(const string& s) {
    if (s.empty()) return false;
    for (auto c : s)
        if (!isdigit(c))
            return false;
    return true;
}

vector<string> strtok(const string& s, char delim=' ') {
    istringstream sstream(s);
    vector<string> result;
    string tok;
    while(getline(sstream, tok, delim)) {
        result.push_back(tok);
    }
    return result;
}

int main (int argc, char* argv[]) {
    if (argc < 2)
        throw runtime_error("Must give input file name");

    vector<string> names; // names
    unordered_map<string, int> name_idx; // map from name to idx
    AdjacencyMatrix graph;

    ifstream f(argv[1]);
    string s;

    while (f.good()) {
        getline(f, s, '\n');
        if (s.back() == '\r') s.pop_back();

        vector<string> toks = strtok(s);
        assert(toks.size() == 11);
        assert(toks[1] == "would");
        assert(toks[4] == "happiness");
        int val = stoi(toks[3]);
        if (toks[2] == "gain")  // inverse the gain/loss values
            val = -val;
        else
            assert(toks[2] == "lose");
        int i1, i2;

        // Check name 1
        string n1 = toks[0];
        if (name_idx.find(n1) == name_idx.end()) {
            names.push_back(n1);
            i1 = names.size() - 1;
            name_idx[n1] = i1;
        } else {
            i1 = name_idx[n1];
        }
        // Check name 2
        string n2 = toks[10];
        n2.pop_back(); // remove trailing period
        if (name_idx.find(n2) == name_idx.end()) {
            names.push_back(n2);
            i2 = names.size() - 1;
            name_idx[n2] = i2;
        } else {
            i2 = name_idx[n2];
        }

        graph.add_edge(i1, i2, val);
    }

    // Add yourself
    names.push_back("You");
    int j = names.size() - 1;
    name_idx["You"] = j;
    for (int i = 0; i < names.size()-1; i++) {
        graph.add_edge(i, j, 0);
        graph.add_edge(j, i, 0);
    }

    // Graph is fully built at this point
    graph.sanity_check();
    graph.print(names);

    cout << "Graph size = " << graph.size() << endl;
    cout << "Min path cost = " << graph.min_hcycle() << endl;

    return 0;
}
