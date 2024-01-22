#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>
#include <vector>
#include <unordered_map>

/*
 * - One increasing straight of 3 letters
 * - No i, o , l
 * - Two different non-overlapping pairs
 */

using namespace std;

const string input = "hepxcrrq";
const string input2 = "hepxxyzz";
const string test1 = "abcdefgh";
const string test2 = "ghijklmn";

bool is_good(const string& s) {
    bool straight = false;
    vector<bool> pairs(26, false); // track pairs of each letter
    
    int n_pairs = 0;
    // track previous chars
    char p2 = '\0';
    char p1 = '\n';

    for (auto c : s) {
        if (c == 'i' || c == 'o' || c == 'l')
            return false;
        
        if (c == p1+1 && p1 == p2+1)
            straight = true;

        if (c == p1 && !pairs[c]) {
            pairs[c] = true;
            n_pairs++;
        }

        p2 = p1;
        p1 = c;
    }

    return (n_pairs >= 2) && straight;
}

int main (int argc, char* argv[]) {
    string curr = input2;
    cout << "curr password: " << curr << endl;

    do {
        // Increment curr
        bool carry = true;
        int digit = 0;
        while (carry && digit <= 7) {
            if (curr[7-digit] == 'z') {
                curr[7-digit] = 'a';
            }
            else {
                curr[7-digit] += 1;
                carry = false;
            }
            digit++;
        }
    } while(!is_good(curr));

    cout << "next password: " << curr << endl;

    return 0;
}