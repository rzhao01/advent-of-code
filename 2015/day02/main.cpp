#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

int get_paper_area(int a, int b, int c) {
    // Swap the largest value to c
    if (a > b) swap(a,b);
    if (b > c) swap(b,c);
    //cout << a << ", " << b << ", " << c << endl;

    // a and b for the smallest side
    return 3*a*b + 2*(b*c + a*c);
}

int get_ribbon_length(int a, int b, int c) {
    // Swap the largest value to c
    if (a > b) swap(a,b);
    if (b > c) swap(b,c);

    // a and b for the smallest side
    return 2*(a+b) + a*b*c;
}

int main (int argc, char* argv[]) {
    if (argc < 2)
        throw runtime_error("Must give input file name");

    ifstream f(argv[1]);
    string s;

    char temp[10];

    int total_paper = 0;
    int total_ribbon = 0;

    while (f.good()) {
        getline(f, s, '\n');

        int l, w, h;
        
        istringstream ss(s);
        ss.getline(temp, 10, 'x');
        l = stoi(temp);
        ss.getline(temp, 10, 'x');
        w = stoi(temp);
        ss.getline(temp, 10, '\n');
        h = stoi(temp);

        total_paper += get_paper_area(l, w, h);
        total_ribbon += get_ribbon_length(l, w, h);
    }

    cout << "Total paper: " << total_paper << endl;
    cout << "Total ribbon: " << total_ribbon << endl;

    return 0;
}