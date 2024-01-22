#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>
#include <vector>
#include <unordered_map>

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
    int total = 0;

    //while (f.good()) {
    getline(f, s, '\n');
    if (s.back() == '\r') s.pop_back();
    //cout << s << endl;

     // Go thru the str, remove all red objects
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] == 'r' && s[i+1] == 'e' && s[i+2] == 'd') {
            // Search for the starting and ending braces
            bool in_array = false;
            int array_frames = 0;
            int brace_frames = 0;
            int start_idx = i-1;
            for (;; start_idx--) {
                if (s[start_idx] == '}')
                    brace_frames++;
                else if (s[start_idx] == '{')
                    if (brace_frames == 0)
                        break;
                    else
                        brace_frames--;
                else if (s[start_idx] == ']')
                    array_frames++;
                else if (s[start_idx] == '[') {
                    if (array_frames == 0) {
                        in_array = true;
                        break;
                    }
                    else
                        array_frames--;
                }
            }

            if (in_array)
                continue;

            brace_frames = 0;
            int end_idx = i+3;
            for (;; end_idx++) {
                if (s[end_idx] == '{')
                    brace_frames++;
                else if (s[end_idx] == '}')
                    if (brace_frames == 0)
                        break;
                    else
                        brace_frames--;
            }

            s.erase(s.begin()+start_idx, s.begin()+end_idx+1);
            i = start_idx;

            cout << s << endl;
        }
    }

    // Go through the str and extract all numerical values
    bool running = false;
    int frame_sum = 0;
    int sum = 0;
    bool neg = false;
    for (size_t i = 0; i < s.size(); i++) {
        if (isdigit(s[i])) {
            if (!running) {
                total += neg ? -sum : sum;
                running = true;
                sum = 0;
                neg = (s[i-1] == '-') ? true : false;                
            }

            sum = 10*sum + static_cast<int>(s[i]-'0');
        }
        else
            running = false;
    }
    total += neg ? -sum : sum;

    cout << "total = " << total << endl;

    return 0;
}