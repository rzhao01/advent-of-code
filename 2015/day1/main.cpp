#include<string>
#include<iostream>
#include<fstream>
#include<stdexcept>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2)
        throw runtime_error("Must give input file name");

    ifstream f(argv[1]);
    string s;
    getline(f, s);

    bool entered_basement = false;
    int floor = 0;
    int i = 1;
    for (char c : s) {
        if (c == '(')
            floor++;
        else if (c == ')')
            floor--;
        else
            throw runtime_error("Error!");

        if (!entered_basement && floor < 0) {
            cout << "First entered basement at character " << i << endl;
            entered_basement = true;
        }

        i++;
    }

    cout << floor << endl;

    return 0;
}