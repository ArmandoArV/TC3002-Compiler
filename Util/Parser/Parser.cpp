#include "Parser.h"
#include "../../TokenKind/TokenKind.h"
#include <iostream>

using namespace std;

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

void Parser::parse() {
    try {
        program();
        cout << "Parsing completed successfully!" << endl;
    } catch (const runtime_error& e) {
        cerr << "Parse error: " << e.what() << endl;
        throw; // Re-throw to allow main to handle it
    }
}

/* Helper Methods */
bool Parser::match(initializer_list<TokenKind> kinds) {
    for (auto kind : kinds) {
        if (check(kind)) {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(TokenKind kind) const {
    if (isAtEnd()) return false;
    return peek().kind == kind;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

Token Parser::peek() const {
    return tokens[current];
}

Token Parser::previous() const {
    return tokens[current - 1];
}

bool Parser::isAtEnd() const {
    return peek().kind == TokenKind::END_OF_FILE;
}

Token Parser::consume(TokenKind kind, const string& message) {
    if (check(kind)) return advance();
    error(peek(), message);
    throw runtime_error(message); // This line is redundant but keeps compiler happy
}

void Parser::error(const Token& token, const string& message) {
    string errorMsg = "[Line " + to_string(token.line) + ":" + to_string(token.column)
                    + "] Syntax Error: " + message;
    throw runtime_error(errorMsg);
}

/* Grammar Rules */
void Parser::program() {
    while (!isAtEnd()) {
        declaration();
    }
}

void Parser::declaration() {
    if (match({TokenKind::VAR})) {
        varDeclaration();
    } else {
        statement();
    }
}

void Parser::varDeclaration() {
    Token name = consume(TokenKind::IDENTIFIER, "Expected variable name");
    if (match({TokenKind::ASSIGN})) {
        expression();
    }
    consume(TokenKind::SEMICOLON, "Expected ';' after variable declaration");
}

void Parser::statement() {
    if (match({TokenKind::PRINTI, TokenKind::PRINTC, TokenKind::PRINTS, TokenKind::PRINTLN})) {
        printStatement();
    } else if (match({TokenKind::LBRACE})) {
        block();
    } else if (match({TokenKind::IF})) {
        ifStatement();
    } else if (match({TokenKind::LOOP})) {
        loopStatement();
    } else if (match({TokenKind::RETURN})) {
        returnStatement();
    } else {
        expressionStatement();
    }
}

void Parser::printStatement() {
    TokenKind printType = previous().kind;
    consume(TokenKind::LPAREN, "Expected '(' after print statement");

    expression();

    consume(TokenKind::RPAREN, "Expected ')' after expression");
    consume(TokenKind::SEMICOLON, "Expected ';' after statement");
}

void Parser::block() {
    while (!check(TokenKind::RBRACE) && !isAtEnd()) {
        declaration();
    }
    consume(TokenKind::RBRACE, "Expected '}' after block");
}

void Parser::ifStatement() {
    consume(TokenKind::LPAREN, "Expected '(' after 'if'");
    expression();
    consume(TokenKind::RPAREN, "Expected ')' after condition");

    statement();

    while (match({TokenKind::ELIF})) {
        consume(TokenKind::LPAREN, "Expected '(' after 'elif'");
        expression();
        consume(TokenKind::RPAREN, "Expected ')' after condition");
        statement();
    }

    if (match({TokenKind::ELSE})) {
        statement();
    }
}

void Parser::loopStatement() {
    consume(TokenKind::LPAREN, "Expected '(' after 'loop'");
    expression();
    consume(TokenKind::RPAREN, "Expected ')' after condition");
    statement();
}

void Parser::returnStatement() {
    if (!check(TokenKind::SEMICOLON)) {
        expression();
    }
    consume(TokenKind::SEMICOLON, "Expected ';' after return value");
}

void Parser::expressionStatement() {
    expression();
    consume(TokenKind::SEMICOLON, "Expected ';' after expression");
}

void Parser::expression() {
    assignment();
}

void Parser::assignment() {
    logicalOr();
    if (match({TokenKind::ASSIGN})) {
        assignment(); // Right-associative
    }
}

void Parser::logicalOr() {
    logicalAnd();
    while (match({TokenKind::OR})) {
        logicalAnd();
    }
}

void Parser::logicalAnd() {
    equality();
    while (match({TokenKind::AND})) {
        equality();
    }
}

void Parser::equality() {
    comparison();
    while (match({TokenKind::EQUAL, TokenKind::NOT_EQUAL})) {
        comparison();
    }
}

void Parser::comparison() {
    term();
    while (match({TokenKind::LESS, TokenKind::LESS_EQUAL, TokenKind::GREATER, TokenKind::GREATER_EQUAL})) {
        term();
    }
}

void Parser::term() {
    factor();
    while (match({TokenKind::PLUS, TokenKind::MINUS})) {
        factor();
    }
}

void Parser::factor() {
    unary();
    while (match({TokenKind::ASTERISK, TokenKind::SLASH, TokenKind::PERCENT})) {
        unary();
    }
}

void Parser::unary() {
    if (match({TokenKind::NOT, TokenKind::MINUS})) {
        unary();
    } else {
        primary();
    }
}

void Parser::primary() {
    if (match({TokenKind::FALSE})) return;
    if (match({TokenKind::TRUE})) return;
    if (match({TokenKind::LIT_INT})) return;
    if (match({TokenKind::LIT_STR})) return;

    if (match({TokenKind::IDENTIFIER})) {
        // Variable reference or function call
        if (match({TokenKind::LPAREN})) {
            // Function call
            if (!check(TokenKind::RPAREN)) {
                do {
                    expression();
                } while (match({TokenKind::COMMA}));
            }
            consume(TokenKind::RPAREN, "Expected ')' after arguments");
        }
        return;
    }

    if (match({TokenKind::LPAREN})) {
        expression();
        consume(TokenKind::RPAREN, "Expected ')' after expression");
        return;
    }

    error(peek(), "Expected expression");
}