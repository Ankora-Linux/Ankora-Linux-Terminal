#pragma once
#include <map>
#include <vector>
#include <string>

struct Lexer {
    enum Type {
        yesop,
        noop
    };

    struct Out {
        Lexer::Type lexertype;      // TYPE
        std::string lexerdata;      // DATA
    };
};

std::map<int, std::vector<Lexer::Out>> lexerfunc(std::stringstream& code);
