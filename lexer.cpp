#include "lexer.hpp"
#include <map>
#include <sstream>
#include <vector>
#include <string>

enum Mode {
    normal,
    string,
    comment1,
    comment2
};

static std::vector<char> sytnax_operator = {
    '!',
    '+',
    '-',
    '*',
    '/',
    '=',
    ':',
};

static bool isOperator(char& op) {
    bool equal = false;
    for (const char& sytnax : sytnax_operator) if (sytnax == op) { equal = true; break; }
    return equal;
}

std::map<int, std::vector<Lexer::Out>> lexerfunc(std::stringstream& code) {
    std::map<int, std::vector<std::string>> lexerlayer1;
    std::vector<std::string> tokens;
    std::string token;
    int line = 0;

    std::string kod = code.str();
    Mode mode = Mode::normal;

    for (size_t size = 0; size < kod.size(); size++) {
        char i = kod[size];

        if (i == '\"' && (mode != Mode::comment1 && mode != Mode::comment2)) {
            if (mode == Mode::string) {  
                token += i;
                if (!token.empty()) {
                    tokens.push_back(token);
                    token = "";
                }
                mode = Mode::normal;
            } else {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token = "";
                }
                token += i;
                mode = Mode::string;
            }
            continue;
        }

        if (mode == Mode::comment1 || mode == Mode::comment2) {
            if (i == '\n' && mode == Mode::comment1) mode = Mode::normal;
            else if (i == '*' && mode == Mode::comment2) {
                size++;
                if (kod[size] == '/' && kod.size() > 1 + size) mode = Mode::normal;
                else size--;
            }
            continue;
        } else if (mode == Mode::string) {
            if (i == '\\' && kod.size() > 1 + size) {
                size++;
                token += i;
                token += kod[size];
            } else token += i;
        } else if (mode == Mode::normal) {
            if (i == ';') {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token = "";
                }
                lexerlayer1[line] = tokens;
                line++;
                tokens.clear();
            } else {
                if (mode != Mode::comment1 && mode != Mode::comment2) {
                    if (i == '/' && kod.size() > 1 + size) {
                        size++;
                        if (kod[size] == '/') mode = Mode::comment1;
                        else if (kod[size] == '*') mode = Mode::comment2;
                        else size--;
                        continue;
                    }
                } 

                if (i != ' ' && i != '\n' && i != '\t') token += i;
                else {
                    if (!token.empty()) {
                        tokens.push_back(token);
                        token = "";
                    }
                }
            }
        }
    }

    if (!token.empty()) {
        if (mode == Mode::string) token += '\"';
        tokens.push_back(token);
        token = "";
        lexerlayer1[line] = tokens;
        line++;
    }



    tokens.clear();
    std::map<int, std::vector<Lexer::Out>> lexerlayer2;
    line = 0;

    for (size_t size = 0; size < lexerlayer1.size(); size++) {
        tokens = lexerlayer1[size];
        std::vector<Lexer::Out> tokensout;

        for (const std::string& i : tokens) {
            Lexer::Type type = Lexer::Type::noop;
            std::string k;

            for (size_t idx = 0; idx < i.size(); idx++) {
                Lexer::Type newtype;
                char j = i[idx];

                if (isOperator(j)) newtype = Lexer::Type::yesop;
                else newtype = Lexer::Type::noop;

                if (type != newtype) {
                    if (!k.empty()) {
                        tokensout.push_back({.lexertype = type, .lexerdata = k});
                        k = "";
                    }
                    type = newtype;
                }

                k += j;
            }

            if (!k.empty()) tokensout.push_back({.lexertype = type, .lexerdata = k});
        }

        lexerlayer2[line] = tokensout;
        line++;
    }

    return lexerlayer2;
}
