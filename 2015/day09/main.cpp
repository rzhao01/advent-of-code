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
        return _data[i][j];
    }
    void sanity_check() const {
        int sz = _data.size();
        // Check square matrix
        for (int i = 0; i < sz; i++) {
            assert(_data[i].size() == sz);
            for (int j = i+1; j < sz; j++)
                assert(_data[i][j] == _data[j][i]);
        }
    }

    void add_edge(int i, int j, int val) {
        // Assumes the final graph is fully connected!
        int m = max(i,j);
        if (m >= _data.size()) _data.resize(m+1);
        if (m >= _data[i].size()) _data[i].resize(m+1);
        if (m >= _data[j].size()) _data[j].resize(m+1);
        _data[i][j] = val;
        _data[j][i] = val;
    }

    // Returns the cost of the min Hamiltonian path starting from i
    // Brute force enumeration, BFS with backtracking
    int min_hpath_helper(int i, vector<bool>& visited,
                         int num_visited=0, int cost=0) const {
        // Base case
        if (num_visited+1 == size())
            return cost;

        // visit i
        visited[i] = true;
        
        // Fully connected node so i connects to all
        int min_cost = INT32_MAX; // assume the min path has +ve cost
        for (int j = 0; j < size(); j++) {
            if (!visited[j]) {
                int new_cost = min_hpath_helper(
                    j, visited, num_visited+1, cost+_data[i][j]);
                min_cost = min(min_cost, new_cost);
            }
        }

        visited[i] = false;
        return min_cost;
    }

    // Return the cost of the minimum Hamiltonian path
    // Assumes the min path has +ve cost
    int min_hpath() const {
        vector<bool> visited(size(), false);

        int min_cost = INT32_MAX;
        for (int i = 0; i < size(); i++) {
            int cost = min_hpath_helper(i, visited);
            min_cost = min(min_cost, cost);
        }

        return min_cost;
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

    vector<string> cities; // name of cities
    unordered_map<string, int> city_idx; // map from city name to its idx
    AdjacencyMatrix graph;

    ifstream f(argv[1]);
    string s;

    // First pass to get names of all cities
    while (f.good()) {
        getline(f, s, '\n');
        if (s.back() == '\r') s.pop_back();

        vector<string> toks = strtok(s);
        assert(toks[1] == "to");
        assert(toks[3] == "=");
        int val = stoi(toks[4]);
        int i1, i2;

        // Check name 1
        string n1 = toks[0];
        if (city_idx.find(n1) == city_idx.end()) {
            cities.push_back(n1);
            i1 = cities.size() - 1;
            city_idx[n1] = i1;
        } else {
            i1 = city_idx[n1];
        }
        // Check name 2
        string n2 = toks[2];
        if (city_idx.find(n2) == city_idx.end()) {
            cities.push_back(n2);
            i2 = cities.size() - 1;
            city_idx[n2] = i2;
        } else {
            i2 = city_idx[n2];
        }

        graph.add_edge(i1, i2, val);
    }

    // Graph is fully built at this point
    graph.sanity_check();

    cout << "Graph size = " << graph.size() << endl;
    cout << "Min path cost = " << graph.min_hpath() << endl;

    return 0;
}
