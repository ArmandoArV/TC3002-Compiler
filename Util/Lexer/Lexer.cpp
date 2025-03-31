#include "lexer.h"
#include <cctype>
#include <unordered_map>
#include <stdexcept>
#include <iostream>

const unordered_set<string> Lexer::keywords = {
    "and", "break", "dec", "elif", "else", "false", "if", "inc",
    "loop", "not", "or", "return", "true", "var",
    // API Functions
    "printi", "printc", "prints", "println",
    "readi", "reads",
    "new", "size", "add", "get", "set",
    "main"
};

Lexer::Lexer(const string& source) : source(source) {}

char Lexer::currentChar() const {
    return position < source.length() ? source[position] : '\0';
}

char Lexer::peekChar() const {
    return (position + 1) < source.length() ? source[position + 1] : '\0';
}

void Lexer::advance() {
    if (position >= source.length()) return;
    if (source[position] == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    position++;
}

void Lexer::skipWhitespace() {
    while (position < source.length()) {
        char c = currentChar();
        if (c == '\n') {
            line++;
            column = 1;
            position++;
        } else if (isspace(c)) {
            advance();
        } else {
            break;
        }
    }
}

Token Lexer::readNumber() {
    size_t startLine = line;
    size_t startColumn = column;
    string numStr;

    if (currentChar() == '-') {
        numStr += '-';
        advance();
    }

    while (isdigit(currentChar())) {
        numStr += currentChar();
        advance();
    }

    return {TokenKind::LIT_INT, numStr, startLine, startColumn};
}

Token Lexer::readIdentifier() {
    size_t startLine = line;
    size_t startColumn = column;
    string ident;

    while (isalnum(currentChar()) || currentChar() == '_') {
        ident += currentChar();
        advance();
    }

   if (keywords.count(ident)) {
    if (ident == "true" || ident == "false") {  // exact lowercase match
        return {TokenKind::LIT_BOOL, ident, startLine, startColumn};
    }
    return {TokenKindFromString(ident), ident, startLine, startColumn};
}
    return {TokenKind::IDENTIFIER, ident, startLine, startColumn};
}

Token Lexer::readString() {
    size_t startLine = line;
    size_t startColumn = column;
    string str;
    advance(); // Skip opening quote

    while (currentChar() != '"' && currentChar() != '\n') {
        if (currentChar() == '\\') {
            advance(); // Skip backslash
            switch (currentChar()) {
                case 'n': str += '\n'; break;
                case 'r': str += '\r'; break;
                case 't': str += '\t'; break;
                case '\\': str += '\\'; break;
                case '"': str += '"'; break;
                case '\'': str += '\''; break;
                case 'u': // Unicode escape
                    // Handle unicode escape sequence
                    break;
                default:
                    str += '\\';
                    str += currentChar();
            }
        } else {
            str += currentChar();
        }
        advance();
    }

    if (currentChar() != '"') {
        throw runtime_error("Unterminated string literal");
    }
    advance(); // Skip closing quote

    return {TokenKind::LIT_STR, str, startLine, startColumn};
}

Token Lexer::readChar() {
    size_t startLine = line;
    size_t startColumn = column;
    string ch;
    advance(); // Skip opening quote

    if (currentChar() == '\\') {
        advance(); // Skip backslash
        switch (currentChar()) {
            case 'n': ch = "\n"; break;
            case 'r': ch = "\r"; break;
            case 't': ch = "\t"; break;
            case '\\': ch = "\\"; break;
            case '"': ch = "\""; break;
            case '\'': ch = "'"; break;
            case 'u': // Unicode escape
                // Handle unicode escape sequence
                break;
            default:
                ch = "\\";
                ch += currentChar();
        }
        advance();
    } else {
        if (currentChar() == '\'') {
            throw runtime_error("Empty character literal");
        }
        ch = currentChar();
        advance();
    }

    if (currentChar() != '\'') {
        throw runtime_error("Unterminated character literal");
    }
    advance(); // Skip closing quote

    return {TokenKind::LIT_CHAR, ch, startLine, startColumn};
}

Token Lexer::readLineComment() {
    size_t startLine = line;
    size_t startColumn = column;
    string comment;
    advance(); advance(); // Skip initial //

    while (currentChar() != '\n' && currentChar() != '\0') {
        comment += currentChar();
        advance();
    }

    // // cout << "Detected LINE_COMMENT: " << comment << endl;
    return {TokenKind::LINE_COMMENT, comment, startLine, startColumn};
}

Token Lexer::readBlockComment() {
    size_t startLine = line;
    size_t startColumn = column;
    string comment;

    // Skip /*
    advance(); advance();

    while (position < source.length()) {
        if (currentChar() == '*' && peekChar() == '/') {
            advance(); advance();
            // Return with original starting line
            return {TokenKind::BLOCK_COMMENT, comment, startLine, startColumn};
        }
        comment += currentChar();
        advance();
    }
    throw runtime_error("Unterminated block comment");
}

TokenKind Lexer::TokenKindFromString(const string& str) {
    static const unordered_map<string, TokenKind> keywordMap = {
        {"and", TokenKind::AND}, {"break", TokenKind::BREAK},
        {"dec", TokenKind::DEC}, {"elif", TokenKind::ELIF},
        {"else", TokenKind::ELSE}, {"false", TokenKind::FALSE},
        {"if", TokenKind::IF}, {"inc", TokenKind::INC},
        {"loop", TokenKind::LOOP}, {"not", TokenKind::NOT},
        {"or", TokenKind::OR}, {"return", TokenKind::RETURN},
        {"true", TokenKind::TRUE}, {"var", TokenKind::VAR},
         // API Functions
        {"printi", TokenKind::PRINTI},
        {"printc", TokenKind::PRINTC},
        {"prints", TokenKind::PRINTS},
        {"println", TokenKind::PRINTLN},
        {"readi", TokenKind::READI},
        {"reads", TokenKind::READS},
        {"new", TokenKind::NEW},
        {"size", TokenKind::SIZE},
        {"add", TokenKind::ADD},
        {"get", TokenKind::GET},
        {"set", TokenKind::SET},
        {"main", TokenKind::MAIN},
         // Boolean literals
        {"true", TokenKind::LIT_BOOL},
        {"false", TokenKind::LIT_BOOL}
    };
    auto it = keywordMap.find(str);
    return it != keywordMap.end() ? it->second : TokenKind::IDENTIFIER;
}

vector<Token> Lexer::tokenize() {
    while (position < source.length()) {
        char c = currentChar();

        if (isspace(c)) {
            skipWhitespace();
            continue;
        }

        // Handle // comments
        if (c == '/' && peekChar() == '/') {
            tokens.push_back(readLineComment());
            continue;
        }

        // Handle /* comments
        if (c == '/' && peekChar() == '*') {
            // cout << "Block comment detected at " << line << ":" << column << endl;
            tokens.push_back(readBlockComment());
            continue;
        }

        if (isdigit(c) || (c == '-' && isdigit(peekChar()))) {
            tokens.push_back(readNumber());
            continue;
        }

        if (isalpha(c) || c == '_') {
            tokens.push_back(readIdentifier());
            continue;
        }

        if (c == '"') {
            tokens.push_back(readString());
            continue;
        }

        if (c == '\'') {
            tokens.push_back(readChar());
            continue;
        }

        // Handle operators
        switch (c) {
            case '=':
                if (peekChar() == '=') {
                    advance(); advance();
                    tokens.push_back({TokenKind::EQUAL, "==", line, column-1});
                } else {
                    advance();
                    tokens.push_back({TokenKind::ASSIGN, "=", line, column-1});
                }
                continue;
            case '!':
                if (peekChar() == '=') {
                    advance(); advance();
                    tokens.push_back({TokenKind::NOT_EQUAL, "!=", line, column-1});
                } else {
                    advance();
                    tokens.push_back({TokenKind::UNKNOWN, "!", line, column-1});
                }
                continue;
            case '<':
                if (peekChar() == '=') {
                    advance(); advance();
                    tokens.push_back({TokenKind::LESS_EQUAL, "<=", line, column-1});
                } else {
                    advance();
                    tokens.push_back({TokenKind::LESS, "<", line, column-1});
                }
                continue;
            case '>':
                if (peekChar() == '=') {
                    advance(); advance();
                    tokens.push_back({TokenKind::GREATER_EQUAL, ">=", line, column-1});
                } else {
                    advance();
                    tokens.push_back({TokenKind::GREATER, ">", line, column-1});
                }
                continue;
            case '+':
                advance();
                tokens.push_back({TokenKind::PLUS, "+", line, column-1});
                continue;
            case '-':
                advance();
                tokens.push_back({TokenKind::MINUS, "-", line, column-1});
                continue;
            case '*':
                advance();
                tokens.push_back({TokenKind::ASTERISK, "*", line, column-1});
                continue;
            case '/':
                if (peekChar() == '/') {
                    tokens.push_back(readLineComment());
                    continue;
                } else if (peekChar() == '*') {
                    tokens.push_back(readBlockComment());
                    continue;
                }
                advance();
                tokens.push_back({TokenKind::SLASH, "/", line, column-1});
                continue;
            case '%':
                advance();
                tokens.push_back({TokenKind::PERCENT, "%", line, column-1});
                continue;
            case '(':
                advance();
                tokens.push_back({TokenKind::LPAREN, "(", line, column-1});
                continue;
            case ')':
                advance();
                tokens.push_back({TokenKind::RPAREN, ")", line, column-1});
                continue;
            case '{':
                advance();
                tokens.push_back({TokenKind::LBRACE, "{", line, column-1});
                continue;
            case '}':
                advance();
                tokens.push_back({TokenKind::RBRACE, "}", line, column-1});
                continue;
            case '[':
                advance();
                tokens.push_back({TokenKind::LBRACKET, "[", line, column-1});
                continue;
            case ']':
                advance();
                tokens.push_back({TokenKind::RBRACKET, "]", line, column-1});
                continue;
            case ',':
                advance();
                tokens.push_back({TokenKind::COMMA, ",", line, column-1});
                continue;
            case ';':
                advance();
                tokens.push_back({TokenKind::SEMICOLON, ";", line, column-1});
                continue;
            case ':':
                advance();
                tokens.push_back({TokenKind::COLON, ":", line, column-1});
                continue;
            default:
                advance();
                tokens.push_back({TokenKind::UNKNOWN, string(1, c), line, column-1});
        }
    }

    tokens.push_back({TokenKind::END_OF_FILE, "", line, column});
    return tokens;
}

string Lexer::tokenKindToString(TokenKind kind) {
    static const unordered_map<TokenKind, string> kindMap = {
        // Keywords
        {TokenKind::AND, "AND"}, {TokenKind::BREAK, "BREAK"},
        {TokenKind::DEC, "DEC"}, {TokenKind::ELIF, "ELIF"},
        {TokenKind::ELSE, "ELSE"}, {TokenKind::FALSE, "FALSE"},
        {TokenKind::IF, "IF"}, {TokenKind::INC, "INC"},
        {TokenKind::LOOP, "LOOP"}, {TokenKind::NOT, "NOT"},
        {TokenKind::OR, "OR"}, {TokenKind::RETURN, "RETURN"},
        {TokenKind::TRUE, "TRUE"}, {TokenKind::VAR, "VAR"},

        // Literals
        {TokenKind::LIT_BOOL, "LIT_BOOL"}, {TokenKind::LIT_INT, "LIT_INT"},
        {TokenKind::LIT_CHAR, "LIT_CHAR"}, {TokenKind::LIT_STR, "LIT_STR"},

        // Identifiers
        {TokenKind::IDENTIFIER, "IDENTIFIER"},

        // Operators
        {TokenKind::ASSIGN, "ASSIGN"}, {TokenKind::PLUS, "PLUS"},
        {TokenKind::MINUS, "MINUS"}, {TokenKind::ASTERISK, "ASTERISK"},
        {TokenKind::SLASH, "SLASH"}, {TokenKind::PERCENT, "PERCENT"},
        {TokenKind::EQUAL, "EQUAL"}, {TokenKind::NOT_EQUAL, "NOT_EQUAL"},
        {TokenKind::LESS, "LESS"}, {TokenKind::LESS_EQUAL, "LESS_EQUAL"},
        {TokenKind::GREATER, "GREATER"}, {TokenKind::GREATER_EQUAL, "GREATER_EQUAL"},

        // Separators
        {TokenKind::LPAREN, "LPAREN"}, {TokenKind::RPAREN, "RPAREN"},
        {TokenKind::LBRACE, "LBRACE"}, {TokenKind::RBRACE, "RBRACE"},
        {TokenKind::LBRACKET, "LBRACKET"}, {TokenKind::RBRACKET, "RBRACKET"},
        {TokenKind::COMMA, "COMMA"}, {TokenKind::SEMICOLON, "SEMICOLON"},
        {TokenKind::COLON, "COLON"},

        // Comments
        {TokenKind::BLOCK_COMMENT, "BLOCK_COMMENT"}, {TokenKind::LINE_COMMENT, "LINE_COMMENT"},
        
        // Special
        {TokenKind::END_OF_FILE, "END_OF_FILE"}, {TokenKind::UNKNOWN, "UNKNOWN"},

        // API Functions
        {TokenKind::PRINTI, "PRINTI"},
        {TokenKind::PRINTC, "PRINTC"},
        {TokenKind::PRINTS, "PRINTS"},
        {TokenKind::PRINTLN, "PRINTLN"},
        {TokenKind::READI, "READI"},
        {TokenKind::READS, "READS"},
        {TokenKind::NEW, "NEW"},
        {TokenKind::SIZE, "SIZE"},
        {TokenKind::ADD, "ADD"},
        {TokenKind::GET, "GET"},
        {TokenKind::SET, "SET"},
        {TokenKind::MAIN, "MAIN"}
    };

    auto it = kindMap.find(kind);
    return it != kindMap.end() ? it->second : "UNKNOWN_TOKEN";
}

/*
 void Lexer::skipLineComment() {
    while (position < source.length() && currentChar() != '\n') {
        advance();
    }
}

void Lexer::skipBlockComment() {
    while (position < source.length()) {
        if (currentChar() == '*' && peekChar() == '/') {
            advance(); advance();
            return;
        }
        advance();
    }
    throw runtime_error("Unterminated block comment");
}
 */