#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>
#include <vector>
#include <unordered_map>

using namespace std;

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

    ifstream f(argv[1]);
    string s;
    
    unordered_map<string, int> ticker;
    ticker["children"] = 3;
    ticker["cats"] = 7;
    ticker["samoyeds"] = 2;
    ticker["pomeranians"] = 3;
    ticker["akitas"] = 0;
    ticker["vizslas"] = 0;
    ticker["goldfish"] = 5;
    ticker["trees"] = 3;
    ticker["cars"] = 2;
    ticker["perfumes"] = 1;
    
    while (f.good()) {
        getline(f, s, '\n');
        if (s.back() == '\r') s.pop_back();

        vector<string> toks = strtok(s);
        assert(toks.size() == 8);

        toks[1].pop_back();
        int n_sue = stoi(toks[1]);
        bool correct_sue = true;
        
        // Parse the rest
        int j = 2;
        while (j < toks.size()) {
            toks[j].pop_back();
            string key = toks[j];

            if (toks[j+1].back() == ',')
                toks[j+1].pop_back();
            int n = stoi(toks[j+1]);

            assert(ticker.count(key) != 0);
            int n_lim = ticker[key];

            bool check;
            if (key == "cats" || key == "trees")
                check = (n > n_lim);
            else if (key == "pomeranians" || key == "goldfish")
                check = (n < n_lim);
            else
                check = (n == n_lim);

            if (!check) {
                //printf("Sue %d, %s = %d > %d\n", n_sue, key.data(), n, n_lim);
                correct_sue = false;
                break;
            }

            j += 2;
        }

        if (correct_sue)
            printf(">> Correct sue: %d\n", n_sue);
    }

    return 0;
}
