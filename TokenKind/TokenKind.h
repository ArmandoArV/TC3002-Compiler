//
// Created by arman on 3/25/2025.
//

#ifndef TC3002_COMPILER_TOKENKIND_H
#define TC3002_COMPILER_TOKENKIND_H


#include <string>
#include <unordered_set>
using namespace std;
class TokenKind {
public:
    // Enumeration of all possible token types
    enum Type {
        // Keywords
        AND, BREAK, DEC, ELIF, ELSE, FALSE, IF, INC, LOOP, NOT, OR,
        RETURN, TRUE, VAR,

        // Literals
        LIT_BOOL, LIT_INT, LIT_CHAR, LIT_STR,

        // Identifiers
        IDENTIFIER,

        // Operators
        ASSIGN,         // =
        PLUS,           // +
        MINUS,          // -
        ASTERISK,       // *
        SLASH,          // /
        PERCENT,        // %
        EQUAL,          // ==
        NOT_EQUAL,      // !=
        LESS,           // <
        LESS_EQUAL,     // <=
        GREATER,        // >
        GREATER_EQUAL,  // >=

        // Separators
        LPAREN,         // (
        RPAREN,         // )
        LBRACE,         // {
        RBRACE,         // }
        LBRACKET,       // [
        RBRACKET,       // ]
        COMMA,          // ,
        SEMICOLON,      // ;
        COLON,          // :

        // Comments
        LINE_COMMENT,   // //
        BLOCK_COMMENT,  // /* ... */

        // Whitespace
        SPACE,
        TAB,
        NEWLINE,

        // End of file
        END_OF_FILE,

        // Error
        UNKNOWN
    };

    // Constructor
    explicit TokenKind(Type type);

    // Get the type
    Type getType() const;

    // Token category checks
    bool isKeyword() const;
    bool isLiteral() const;
    bool isOperator() const;
    bool isSeparator() const;
    bool isWhitespace() const;
    bool isComment() const;

    // String representation
    string toString() const;

    // Static helper methods
    static bool isKeywordString(const string& str);
    static TokenKind::Type keywordStringToType(const string& str);

private:
    Type type;

    // Static sets for quick lookups
    static const unordered_set<string> keywordStrings;
    static const unordered_set<Type> keywordTypes;
    static const unordered_set<Type> operatorTypes;
    static const unordered_set<Type> separatorTypes;
};

#endif //TC3002_COMPILER_TOKENKIND_H
