#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>
#include <vector>
#include <unordered_map>

/* Division and modulo
 */

using namespace std;

typedef struct _Deer {
    int speed;
    int p_move, p_rest;
    int t_move{0}, t_rest{0};
    int dist{0};
    int score{0};

    int advance() { // advance 1 time step
        if (t_move != 0) {
            t_move--;
            dist += speed;
            if (t_move == 0) t_rest = p_rest;
        }
        else {
            t_rest--;
            if (t_rest == 0) t_move = p_move;
        }
    }

} Deer;

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
    const int total_t = 2503;
    vector<Deer> deers;

    while (f.good()) {
        getline(f, s, '\n');
        if (s.back() == '\r') s.pop_back();

        vector<string> toks = strtok(s);
        assert(toks.size() == 15);

        Deer d;
        d.speed = stoi(toks[3]);
        d.p_move = stoi(toks[6]);
        d.p_rest = stoi(toks[13]);
        d.t_move = d.p_move;

        deers.push_back(d);
    }

    for (int t = 0; t < total_t; t++) {
        int max_dist = 0;
        for (auto& d : deers) {
            d.advance();
            max_dist = max(max_dist, d.dist);
        }
        for (auto& d : deers) {
            if (d.dist == max_dist) d.score++;
        }
    }

    int max_score = 0;
    int max_dist = 0;
    for (auto d : deers) {
        max_dist = max(max_dist, d.dist);
        max_score = max(max_score, d.score);
    }

    cout << "Max dist  = " << max_dist << endl;
    cout << "Max score = " << max_score << endl;

    return 0;
}
