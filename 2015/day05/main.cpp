#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

class CharPairLookup {
    vector<bool> _data;

public:
    CharPairLookup() {
        _data = vector<bool>(26*26, false);
        assert(_data.size() == 26*26);
    }

    bool get(char a, char b) const {
        return _data[(a-'a')*26 + (b-'a')];
    }

    void set(char a, char b) {
        _data[(a-'a')*26 + (b-'a')] = true;
    }

    void reset() {
        for (int i = 0; i < _data.size(); i++)
            _data[i] = false;
    }
};

int main (int argc, char* argv[]) {
    if (argc < 2)
        throw runtime_error("Must give input file name");

    ifstream f(argv[1]);
    string s;

    CharPairLookup lookup;
    int num_nice = 0;

    while (f.good()) {
        getline(f, s, '\n');

        // lookup table for whether a pair has been found
        lookup.reset();

        int rpair = false;
        bool twice = false;
        char p1 = 'a'-2;
        char p2 = 'a'-1;

        for (char c : s) {
            if (c == '\r') // carriage return
                break;

            //cout << c << " " << static_cast<int>(c-'a') << " " << endl;
            // Use c and p1 to do the lookup
            if (p1 >= 'a' && lookup.get(c, p1))
                rpair = true;
            // use p1 and p2 to update the lookup
            if (p2 >= 'a')
                lookup.set(p1, p2);

            if (c == p2)
                twice = true;

            p2 = p1;
            p1 = c;
        }

        if (rpair && twice)
            num_nice++;
    }

    cout << "num_nice: " << num_nice << endl;

    return 0;
}