//
// Created by arman on 3/25/2025.
//

#include "TokenKind.h"
#include <unordered_map>
using namespace std;

// Initialize static sets
const unordered_set<string> TokenKind::keywordStrings = {
    "and", "break", "dec", "elif", "else", "false", "if", "inc", 
    "loop", "not", "or", "return", "true", "var"
};

const unordered_set<TokenKind::Type> TokenKind::keywordTypes = {
    AND, BREAK, DEC, ELIF, ELSE, FALSE, IF, INC, LOOP, 
    NOT, OR, RETURN, TRUE, VAR
};

const unordered_set<TokenKind::Type> TokenKind::operatorTypes = {
    ASSIGN, PLUS, MINUS, ASTERISK, SLASH, PERCENT,
    EQUAL, NOT_EQUAL, LESS, LESS_EQUAL, GREATER, GREATER_EQUAL
};

const unordered_set<TokenKind::Type> TokenKind::separatorTypes = {
    LPAREN, RPAREN, LBRACE, RBRACE, LBRACKET, RBRACKET,
    COMMA, SEMICOLON, COLON
};

// Constructor
TokenKind::TokenKind(Type type) : type(type) {}

// Get the type
TokenKind::Type TokenKind::getType() const {
    return type;
}

// Check if this is a keyword
bool TokenKind::isKeyword() const {
    return keywordTypes.find(type) != keywordTypes.end();
}

// Check if this is a literal
bool TokenKind::isLiteral() const {
    return type == LIT_BOOL || type == LIT_INT || 
           type == LIT_CHAR || type == LIT_STR;
}

// Check if this is an operator
bool TokenKind::isOperator() const {
    return operatorTypes.find(type) != operatorTypes.end();
}

// Check if this is a separator
bool TokenKind::isSeparator() const {
    return separatorTypes.find(type) != separatorTypes.end();
}

// Check if this is whitespace
bool TokenKind::isWhitespace() const {
    return type == SPACE || type == TAB || type == NEWLINE;
}

// Check if this is a comment
bool TokenKind::isComment() const {
    return type == LINE_COMMENT || type == BLOCK_COMMENT;
}

// Static method to check if a string is a keyword
bool TokenKind::isKeywordString(const string& str) {
    return keywordStrings.find(str) != keywordStrings.end();
}

// Static method to convert keyword string to Type
TokenKind::Type TokenKind::keywordStringToType(const string& str) {
    static const unordered_map<string, Type> keywordMap = {
        {"and", AND},
        {"break", BREAK},
        {"dec", DEC},
        {"elif", ELIF},
        {"else", ELSE},
        {"false", FALSE},
        {"if", IF},
        {"inc", INC},
        {"loop", LOOP},
        {"not", NOT},
        {"or", OR},
        {"return", RETURN},
        {"true", TRUE},
        {"var", VAR}
    };

    auto it = keywordMap.find(str);
    if (it != keywordMap.end()) {
        return it->second;
    }
    return UNKNOWN;
}

// Convert to string representation
string TokenKind::toString() const {
    static const unordered_map<Type, string> typeToString = {
        // Keywords
        {AND, "AND"},
        {BREAK, "BREAK"},
        {DEC, "DEC"},
        {ELIF, "ELIF"},
        {ELSE, "ELSE"},
        {FALSE, "FALSE"},
        {IF, "IF"},
        {INC, "INC"},
        {LOOP, "LOOP"},
        {NOT, "NOT"},
        {OR, "OR"},
        {RETURN, "RETURN"},
        {TRUE, "TRUE"},
        {VAR, "VAR"},

        // Literals
        {LIT_BOOL, "LIT_BOOL"},
        {LIT_INT, "LIT_INT"},
        {LIT_CHAR, "LIT_CHAR"},
        {LIT_STR, "LIT_STR"},

        // Identifier
        {IDENTIFIER, "IDENTIFIER"},

        // Operators
        {ASSIGN, "ASSIGN"},
        {PLUS, "PLUS"},
        {MINUS, "MINUS"},
        {ASTERISK, "ASTERISK"},
        {SLASH, "SLASH"},
        {PERCENT, "PERCENT"},
        {EQUAL, "EQUAL"},
        {NOT_EQUAL, "NOT_EQUAL"},
        {LESS, "LESS"},
        {LESS_EQUAL, "LESS_EQUAL"},
        {GREATER, "GREATER"},
        {GREATER_EQUAL, "GREATER_EQUAL"},

        // Separators
        {LPAREN, "LPAREN"},
        {RPAREN, "RPAREN"},
        {LBRACE, "LBRACE"},
        {RBRACE, "RBRACE"},
        {LBRACKET, "LBRACKET"},
        {RBRACKET, "RBRACKET"},
        {COMMA, "COMMA"},
        {SEMICOLON, "SEMICOLON"},
        {COLON, "COLON"},

        // Comments
        {LINE_COMMENT, "LINE_COMMENT"},
        {BLOCK_COMMENT, "BLOCK_COMMENT"},

        // Whitespace
        {SPACE, "SPACE"},
        {TAB, "TAB"},
        {NEWLINE, "NEWLINE"},

        // Special
        {END_OF_FILE, "END_OF_FILE"},
        {UNKNOWN, "UNKNOWN"}
    };

    auto it = typeToString.find(type);
    if (it != typeToString.end()) {
        return it->second;
    }
    return "UNKNOWN";
}