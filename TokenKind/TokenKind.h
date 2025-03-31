//
// Created by arman on 3/25/2025.
//

#ifndef TC3002_COMPILER_TOKENKIND_H
#define TC3002_COMPILER_TOKENKIND_H


#include <string>
#include <unordered_set>
using namespace std;
enum class TokenKind {
    // Keywords
    AND, BREAK, DEC, ELIF, ELSE, FALSE, IF, INC, LOOP, NOT, OR,
    RETURN, TRUE, VAR,

    // Literals
    LIT_BOOL, LIT_INT, LIT_CHAR, LIT_STR,

    // Identifiers
    IDENTIFIER,

    // Operators
    ASSIGN, PLUS, MINUS, ASTERISK, SLASH, PERCENT,
    EQUAL, NOT_EQUAL, LESS, LESS_EQUAL, GREATER, GREATER_EQUAL,

    // Separators
    LPAREN, RPAREN, LBRACE, RBRACE, LBRACKET, RBRACKET,
    COMMA, SEMICOLON, COLON,

    // Comments
    LINE_COMMENT, BLOCK_COMMENT,

    // Whitespace
    SPACE, TAB, NEWLINE,

    // API Functions
    PRINTI,   // printi
    PRINTC,   // printc
    PRINTS,   // prints
    PRINTLN,  // println
    READI,    // readi
    READS,    // reads
    NEW,      // new
    SIZE,     // size
    ADD,      // add
    GET,      // get
    SET,      // set
    MAIN,     // main


    // Special
    END_OF_FILE, UNKNOWN
};
#endif