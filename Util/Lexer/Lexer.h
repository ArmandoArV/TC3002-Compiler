#ifndef LEXER_H
#define LEXER_H

#include "../../Token/Token.h"
#include <vector>
#include <string>
#include <unordered_set>

class Lexer {
private:
    string source;
    size_t position = 0;
    size_t line = 1;
    size_t column = 1;
    vector<Token> tokens;

    static const unordered_set<string> keywords;

    char currentChar() const;
    char peekChar() const;
    void advance();
    void skipWhitespace();
    void skipLineComment();
    void skipBlockComment();
    Token readLineComment();
    Token readBlockComment();
    Token readNumber();
    Token readIdentifier();
    Token readString();
    Token readChar();
    TokenKind TokenKindFromString(const string& str);
    
public:
    Lexer(const string& source);
    vector<Token> tokenize();
    static string tokenKindToString(TokenKind kind);
};

#endif