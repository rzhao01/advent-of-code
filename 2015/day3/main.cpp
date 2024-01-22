#include <string>
#include <fstream>
#include <istream>
#include <sstream>
#include <iostream>
#include <unordered_set>

using namespace std;

typedef pair<int,int> Coord;

struct CoordHash {
  size_t operator()(const Coord& c) const noexcept {
      return std::hash<int>{}(c.first) + std::hash<int>{}(c.second << 2);
  }
};

class Deliverator {
    Coord coord{0,0};

public:
    Coord get() const { return coord; }

    void move(const char c) {
        switch(c) {
            case '^':
                coord.first--; break;
            case 'v':
                coord.first++; break;
            case '<':
                coord.second--; break;
            case '>':
                coord.second++; break;
            default:
                cout << "Unknown char " << c << endl;
                throw runtime_error("Deliverator error");
        }
    }
};

int main (int argc, char* argv[]) {
    if (argc < 2)
        throw runtime_error("Must give input file name");

    ifstream f(argv[1]);
    char c;

    unordered_set<Coord, CoordHash> lookup;
    lookup.insert(make_pair(0,0));
    Deliverator d1, d2;
    bool flag = false;
    int houses = 1;

    while (f.get(c)) {
        Coord temp_;
        if (!flag) {
            d1.move(c);
            temp_ = d1.get();
        }
        else {
            d2.move(c);
            temp_ = d2.get();
        }

        flag = !flag;

        if (lookup.find(temp_) == lookup.end()) {
            houses++;
            lookup.insert(temp_);
        }
    }

    cout << "Total houses: " << houses << endl;

    return 0;
}