#include "lexer.hpp"
#include <map>
#include <sstream>
#include <vector>
#include <string>

std::map<int, std::vector<std::string>> lexerfunc(std::stringstream& code) {
    std::map<int, std::vector<std::string>> lexerlayer1;
    std::vector<std::string> tokens;
    std::string token;
    int line = 0;

    std::string kod = code.str();

    for (size_t size = 0; size < kod.size(); size++) {
        char i = kod[size];

        if (i == ';') {
            if (!token.empty()) {
                tokens.push_back(token);
                token = "";
            }
            lexerlayer1[line] = tokens;
            line++;
            tokens.clear();
        } else {
            if (i != ' ' && i != '\n' && i != '\t') token += i;
            else {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token = "";
                }
            }
        }
    }

    if (!token.empty()) {
        tokens.push_back(token);
        token = "";
        lexerlayer1[line] = tokens;
        line++;
    }

    return lexerlayer1;
}
