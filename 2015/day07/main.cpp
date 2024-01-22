#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>
#include <vector>
#include <unordered_map>

/*
 * Each op has a single output and up to 2 inputs
 * One-input Ops: NOT, CONST
 * Two-input Ops: AND, OR, LSHIFT, RSHIFT
 */

using namespace std;

enum class Op {Const, Var, Not, And, Or, Lshift, Rshift, None};

class Node {
    // Table of all nodes, mapping from name
    string _name;
    Op _op;
    uint16_t _value;    // only valid for const or shift nodes
    bool _evaluated{false};
    Node* _p1{nullptr};
    Node* _p2{nullptr};

public:
    Node(string name, Op op) : _name{name}, _op{op} {}
    void set_op(Op op) { _op = op; }
    void set_value(uint16_t v) { _value = v; }
    void set_parents(Node* p1, Node* p2) { _p1 = p1; _p2 = p2; }

    Op get_op() const { return _op; }
    uint16_t get_value() const { return _value; }

    uint16_t evaluate() {
        if (_evaluated) return _value;

        switch(_op) {
            case Op::Const:
                break;
            case Op::Var:
                _value = _p1->evaluate(); break;
            case Op::Not:
                _value = ~(_p1->evaluate()); break;
            case Op::And:
                _value = _p1->evaluate() & _p2->evaluate(); break;
            case Op::Or:
                _value = _p1->evaluate() | _p2->evaluate(); break;
            case Op::Lshift:
                assert(_p2->_op == Op::Const);
                _value = _p1->evaluate() << _p2->evaluate(); break;
            case Op::Rshift:
                assert(_p2->_op == Op::Const);
                _value = _p1->evaluate() >> _p2->evaluate(); break;
            default:
                throw runtime_error(_name + ": invalid _op" + to_string(static_cast<int>(_op)));
        }

        _evaluated = true;
        return _value; 
    }
};

bool is_number(const string& s) {
    if (s.empty()) return false;
    for (auto c : s)
        if (!isdigit(c))
            return false;
    return true;
}

Node* get_maybe_const_node(string name) {
    Node* node;
    if (is_number(name)) {
        node = new Node(name, Op::Const);
        node->set_value(stoi(name));
    }
    else
        node = new Node(name, Op::None);
    return node;
}

Op str2op(const string& s) {
    if (s == "AND")
        return Op::And;
    if (s == "OR")
        return Op::Or;
    if (s == "LSHIFT")
        return Op::Lshift;
    if (s == "RSHIFT")
        return Op::Rshift;
    if (s == "NOT")
        return Op::Not;
    throw runtime_error("Invalid str2op");
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

    unordered_map<string, Node*> table;

    while (f.good()) {
        getline(f, s, '\n');
        if (s.back() == '\r') s.pop_back();
        //cout << s << endl;

        // local vars
        string p1, p2;
        Op op{Op::None};

        // tokenize s
        vector<string> toks = strtok(s);
        if (toks[1] == "->") {
            // num -> wire
            if (is_number(toks[0]))
                op = Op::Const;
            else
                op = Op::Var;
            p1 = toks[0];
        }
        else if (toks[0] == "NOT") {
            op = Op::Not;
            p1 = toks[1];
        }
        else {
            assert(toks[3] == "->");
            op = str2op(toks[1]);
            p1 = toks[0];
            p2 = toks[2];
        }

        string name = toks.back();

        // Create/get parents
        Node* np1{nullptr};
        Node* np2{nullptr};
        if (op != Op::Const && !p1.empty()) {
            if (table.find(p1) == table.end()) {
                np1 = get_maybe_const_node(p1);
                table[p1] = np1;
            }
            else
                np1 = table[p1];
        }
        if (op != Op::Const && !p2.empty()) {
            if (table.find(p2) == table.end()) {
                np2 = get_maybe_const_node(p2);
                table[p2] = np2;
            }
            else
                np2 = table[p2];
        }

         // Create/get node
        Node* node{nullptr};
        if (table.find(name) == table.end()) { // first time seeing this name
            //cout << "New node " << name << endl;
            node = new Node(name, op);
            table[name] = node;
        }
        else
            node = table[name];
        node->set_parents(np1, np2);
        node->set_op(op);

        assert(op != Op::None);
        if (op == Op::Not || op == Op::Var) {
            assert(np1);
        }
        else if (op == Op::And || op == Op::Or || op == Op::Lshift || op == Op::Rshift) {
            assert(np1 && np2);
        }
        else if (op == Op::Const) {
            node->set_value(stoi(p1));
        }
    }

    for (auto it = table.begin(); it != table.end(); ++it) {
        cout << it->first << ": op" << to_string(static_cast<int>(it->second->get_op())) <<
            " = " << it->second->evaluate() << endl;
    }

    assert(table.find("a") != table.end());
    cout << "\na = " << table["a"]->evaluate() << endl;

    return 0;
}