cmake_minimum_required(VERSION 3.26)
project(TC3002_Compiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(TC3002_Compiler main.cpp
        Util/Lexer.cpp
        TokenKind/TokenKind.cpp
        TokenKind/TokenKind.h)
