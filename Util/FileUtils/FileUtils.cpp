#include "FileUtils.h"
#include <fstream>
#include <sstream>
using namespace std;
string readFileContents(const string& filePath) {
    ifstream file(filePath);
    if (!file) {
        throw runtime_error("Could not open file: " + filePath);
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}