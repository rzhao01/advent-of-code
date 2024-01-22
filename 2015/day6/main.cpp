#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

vector<string> strtok(const string& s, char delim=' ') {
    istringstream sstream(s);
    vector<string> result;
    string tok;
    while(getline(sstream, tok)) {
        result.push_back(tok);
    }
    return result;
}

enum class Mode {On, Off, Toggle, None};

typedef struct _Coord {
    int y;
    int x;
} Coord;

Coord get_coord(const string& s) {
    istringstream sstream(s);
    string temp;
    Coord coord;
    getline(sstream, temp, ','); // first coord
    coord.y = stoi(temp);
    getline(sstream, temp, '\n'); // second coord
    coord.x = stoi(temp);
    return coord;
}

class Grid {
    vector<int> _arr;
    static constexpr int sz = 1000*1000;

    void swtch(Coord c1, Coord c2, bool val) {
        for (int i = c1.y; i <= c2.y; i++)
            for (int j = c1.x; j <= c2.x; j++)
                _arr[i*1000 + j] = val;
    }

public:
    Grid() {
        _arr = vector<int>(sz, 0);
    }

    void toggle(Coord c1, Coord c2) {
        for (int i = c1.y; i <= c2.y; i++)
            for (int j = c1.x; j <= c2.x; j++)
                _arr[i*1000 + j] += 2;
    }

    void  on(Coord c1, Coord c2) {
        for (int i = c1.y; i <= c2.y; i++)
            for (int j = c1.x; j <= c2.x; j++)
                _arr[i*1000 + j]++;
    }
    void off(Coord c1, Coord c2) {
        for (int i = c1.y; i <= c2.y; i++)
            for (int j = c1.x; j <= c2.x; j++)
                if (_arr[i*1000 + j] != 0)
                    _arr[i*1000 + j]--;
    }

    int luminosity() const {
        int t = 0;
        for (int i = 0; i < sz; ++i)
            t += _arr[i];
        return t;
    }
};

int main (int argc, char* argv[]) {
    if (argc < 2)
        throw runtime_error("Must give input file name");

    ifstream f(argv[1]);
    string s;

    Grid grid;
    Coord c1, c2;
    Mode m;

    while (f.good()) {
        getline(f, s, '\n');
        //cout << s << endl;

        // tokenize s
        istringstream sstream(s);
        int i = 0;
        string tok;
        m = Mode::None;

        getline(sstream, tok, ' '); // turn/toggle
        if (tok == "turn") {
            getline(sstream, tok, ' '); // on/off
            if (tok == "on")
                m = Mode::On;
            else if (tok == "off")
                m = Mode::Off;
            else
                throw runtime_error("Expected on/off");
        }
        else {
            assert(tok == "toggle");
            m = Mode::Toggle;
        }
        
        getline(sstream, tok, ' '); // coord
        if (tok != "on" && tok != "off") {
            c1 = get_coord(tok);
        }

        getline(sstream, tok, ' '); // through
        assert(tok == "through");

        getline(sstream, tok, ' '); // coord
        if (tok != "on" && tok != "off") {
            c2 = get_coord(tok);
        }

        //cout << c1.y << ", " << c1.x << "  " << c2.y << ", " << c2.x << endl;

        switch(m) {
            case Mode::On:
                grid.on(c1, c2); break;
            case Mode::Off:
                grid.off(c1, c2); break;
            case Mode::Toggle:
                grid.toggle(c1, c2); break;
            default:
                throw runtime_error("Invalide mode");
        }

        //cout << " lit: " << grid.num_lit() << endl;
    }

    cout << "Luminosity: " << grid.luminosity() << endl;

    return 0;
}