#ifndef TOKEN_H
#define TOKEN_H

#include "../TokenKind/TokenKind.h"
#include <string>
using namespace std;
struct Token {
    TokenKind kind;
    std::string value;
    size_t line;
    size_t column;
};
#endif // TOKEN_H