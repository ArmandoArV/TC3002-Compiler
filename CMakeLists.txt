cmake_minimum_required(VERSION 3.26)
project(TC3002_Compiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(TC3002_Compiler main.cpp
        TokenKind/TokenKind.h
        Token/Token.h
        Util/Lexer/Lexer.cpp
        Util/Lexer/Lexer.h
        Util/FileUtils/FileUtils.cpp
        Util/FileUtils/FileUtils.h
)
