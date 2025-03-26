#include "lexer.h"
#include <cctype>
#include <unordered_map>
#include <stdexcept>

const std::unordered_set<std::string> Lexer::keywords = {
    "and", "break", "dec", "elif", "else", "false", "if", "inc",
    "loop", "not", "or", "return", "true", "var"
};

Lexer::Lexer(const std::string& source) : source(source) {}

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
    while (position < source.length() && isspace(currentChar())) {
        advance();
    }
}

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
    throw std::runtime_error("Unterminated block comment");
}

Token Lexer::readNumber() {
    size_t startLine = line;
    size_t startColumn = column;
    std::string numStr;

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
    std::string ident;

    while (isalnum(currentChar()) || currentChar() == '_') {
        ident += currentChar();
        advance();
    }

    if (keywords.count(ident)) {
        if (ident == "true" || ident == "false") {
            return {TokenKind::LIT_BOOL, ident, startLine, startColumn};
        }
        return {TokenKindFromString(ident), ident, startLine, startColumn};
    }
    return {TokenKind::IDENTIFIER, ident, startLine, startColumn};
}

Token Lexer::readString() {
    size_t startLine = line;
    size_t startColumn = column;
    std::string str;
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
        throw std::runtime_error("Unterminated string literal");
    }
    advance(); // Skip closing quote

    return {TokenKind::LIT_STR, str, startLine, startColumn};
}

Token Lexer::readChar() {
    size_t startLine = line;
    size_t startColumn = column;
    std::string ch;
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
            throw std::runtime_error("Empty character literal");
        }
        ch = currentChar();
        advance();
    }

    if (currentChar() != '\'') {
        throw std::runtime_error("Unterminated character literal");
    }
    advance(); // Skip closing quote

    return {TokenKind::LIT_CHAR, ch, startLine, startColumn};
}

TokenKind Lexer::TokenKindFromString(const std::string& str) {
    static const std::unordered_map<std::string, TokenKind> keywordMap = {
        {"and", TokenKind::AND}, {"break", TokenKind::BREAK},
        {"dec", TokenKind::DEC}, {"elif", TokenKind::ELIF},
        {"else", TokenKind::ELSE}, {"false", TokenKind::FALSE},
        {"if", TokenKind::IF}, {"inc", TokenKind::INC},
        {"loop", TokenKind::LOOP}, {"not", TokenKind::NOT},
        {"or", TokenKind::OR}, {"return", TokenKind::RETURN},
        {"true", TokenKind::TRUE}, {"var", TokenKind::VAR}
    };
    auto it = keywordMap.find(str);
    return it != keywordMap.end() ? it->second : TokenKind::IDENTIFIER;
}

std::vector<Token> Lexer::tokenize() {
    while (position < source.length()) {
        char c = currentChar();

        if (isspace(c)) {
            skipWhitespace();
            continue;
        }

        if (c == '/' && peekChar() == '/') {
            advance(); advance();
            skipLineComment();
            continue;
        }

        if (c == '/' && peekChar() == '*') {
            advance(); advance();
            skipBlockComment();
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
                tokens.push_back({TokenKind::UNKNOWN, std::string(1, c), line, column-1});
        }
    }

    tokens.push_back({TokenKind::END_OF_FILE, "", line, column});
    return tokens;
}

string Lexer::tokenKindToString(TokenKind kind) {
    static const std::unordered_map<TokenKind, std::string> kindMap = {
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

        // Special
        {TokenKind::END_OF_FILE, "END_OF_FILE"}, {TokenKind::UNKNOWN, "UNKNOWN"}
    };

    auto it = kindMap.find(kind);
    return it != kindMap.end() ? it->second : "UNKNOWN_TOKEN";
}