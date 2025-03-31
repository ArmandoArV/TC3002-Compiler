#include <iostream>
#include <unordered_map>
#include "./Util/Lexer/Lexer.h"
#include "./Util/FileUtils/FileUtils.h"
#include "./Util/Parser/Parser.h"

using namespace std;

// Prints individual token with line/column info
void printToken(const Token& token) {
    cout << "[" << token.line << ":" << token.column << "] "
         << Lexer::tokenKindToString(token.kind)
         << " '" << token.value << "'\n";
}

// Generates token statistics report
void printTokenStatistics(const vector<Token>& tokens) {
    unordered_map<TokenKind, int> tokenCounts;
    int totalTokens = 0;

    // Count non-whitespace, non-comment tokens
    for (const auto& token : tokens) {
        if (token.kind != TokenKind::SPACE &&
            token.kind != TokenKind::LINE_COMMENT &&
            token.kind != TokenKind::BLOCK_COMMENT) {
            tokenCounts[token.kind]++;
            totalTokens++;
        }
    }

    // Print report
    cout << "\n=== Token Analysis ===\n";
    cout << "Total significant tokens: " << totalTokens << "\n";
    cout << "Distinct token types: " << tokenCounts.size() << "\n\n";
    cout << "Token frequency:\n";
    cout << "----------------\n";

    for (const auto& [kind, count] : tokenCounts) {
        cout << Lexer::tokenKindToString(kind) << ": " << count << "\n";
    }
}

int main() {
    // Get input file
    string filePath;
    cout << "Quetzal Compiler\n";
    cout << "================\n";
    cout << "Enter source file path: ";
    getline(cin, filePath);

    try {
        // Phase 1: Lexical Analysis
        cout << "\n[1/2] Lexical Analysis\n";
        cout << "----------------------\n";

        string source = readFileContents(filePath);
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        // Display token stream
        cout << "Token stream:\n";
        cout << "-------------\n";
        for (const auto& token : tokens) {
            printToken(token);
        }

        // Show token statistics
        printTokenStatistics(tokens);

        // Phase 2: Syntax Analysis
        cout << "\n[2/2] Syntax Analysis\n";
        cout << "----------------------\n";

        Parser parser(tokens);
        parser.parse();

        cout << "\n✓ Compilation successful!\n";
        cout << "No syntax errors found in " << filePath << "\n";

    } catch (const exception& e) {
        // Error handling
        cerr << "\n✗ Compilation Failed!\n";
        cerr << "Error: " << e.what() << endl;
        cerr << "Aborting compilation.\n";
        return 1;
    }

    return 0;
}
/*
int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <source_file.quetzal>\n";
        return 1;
    }

    try {
        string source = readFileContents(argv[1]);
        Lexer lexer(source);

        cout << "Tokenizing file: " << argv[1] << endl;
        auto tokens = lexer.tokenize();

        cout << "\n=== Token Stream ===\n";
        for (const auto& token : tokens) {
            printToken(token);
        }

        cout << "\n=== Token Analysis ===\n";

    } catch (const exception& e) {
        cerr << "\nERROR: " << e.what() << endl;
        return 1;
    }

    return 0;
}
*/