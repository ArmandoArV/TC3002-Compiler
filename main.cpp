#include <iostream>
#include "./Util/Lexer/Lexer.h"
#include "./Util/FileUtils/FileUtils.h"

using namespace std;
void printToken(const Token& token) {
    cout << "[" << token.line << ":" << token.column << "] " 
              << Lexer::tokenKindToString(token.kind) << " '" << token.value << "'\n";
}

int main() {
    string filePath;
    cout << "Enter the path to the source file (.quetzal): ";
    getline(cin, filePath);

    try {
        string source = readFileContents(filePath);
        Lexer lexer(source);

        cout << "Tokenizing file: " << filePath << endl;
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