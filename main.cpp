#include <iostream>
#include "./TokenKind/TokenKind.h"
using namespace std;
int main() {
    TokenKind kind(TokenKind::IF);  // Create a keyword token kind
    if (kind.isKeyword()) {         // Check if it's a keyword
        cout << kind.toString();  // Output "IF"
    }
    return 0;
}
