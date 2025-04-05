#ifndef TC3002_COMPILER_PARSER_H
#define TC3002_COMPILER_PARSER_H

#include "../../Token/Token.h"
#include <vector>
#include <memory>
#include <stdexcept>
#include <initializer_list>

class Parser {
private:
    std::vector<Token> tokens;
    size_t current = 0;

    // Helper methods
    bool match(std::initializer_list<TokenKind> kinds);
    bool check(TokenKind kind) const;
    Token advance();
    Token peek() const;
    Token previous() const;
    bool isAtEnd() const;

    // Error handling
    Token consume(TokenKind kind, const std::string& message);
    void error(const Token& token, const std::string& message);

    // Grammar rules
    void program();
    void declaration();
    void varDeclaration();
    void statement();
    void synchronize();
    
    // Statements
    void printStatement();
    void block();
    void ifStatement();
    void loopStatement();
    void returnStatement();
    void expressionStatement();
    
    // Expressions
    void expression();
    void assignment();
    void logicalOr();
    void logicalAnd();
    void equality();
    void comparison();
    void term();
    void factor();
    void unary();
    void primary();
    void skipComments() {
        while (check(TokenKind::LINE_COMMENT) || check(TokenKind::BLOCK_COMMENT)) {
            advance();
        }
    }

public:
    Parser(const std::vector<Token>& tokens);
    void parse();
};

#endif //TC3002_COMPILER_PARSER_H