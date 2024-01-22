#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>
#include <vector>
#include <unordered_map>

/*
 */

using namespace std;

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

    ifstream f(argv[1]);
    string s;
    
    int total_str = 0;
    int total_mem = 0;
    int total_new = 0;

    while (f.good()) {
        getline(f, s, '\n');
        if (s.back() == '\r') s.pop_back();

        // length in code
        int len_str = s.size();
        total_str += len_str;

        // length in mem
        int len_mem = 0;
        for (int i = 1; i < s.size()-1; i++) { // skip first/last dquote
            if (s[i] == '\\') {
                if (s[i+1] == 'x') { // hex escaped seq
                    i += 3;
                }
                else {
                    assert(s[i+1] == '\\' || s[i+1] == '"');
                    i++;
                }
            }
            len_mem++;
        }
        total_mem += len_mem;

        // for newly encoded string:
        // 1. Add 2 for the new enclosing quotes
        // 2. Add 1 for each " or \ to escape it
        int len_new = 2;
        for (auto c : s) {
            if (c == '\\' || c == '"')
                len_new += 2;
            else
                len_new++;
        }
        total_new += len_new;

        cout << s << " = " << len_str << ", " << len_new << endl;
    }

    cout << "\ndiff = " << total_new - total_str << endl;
    return 0;
}
