cmake_minimum_required(VERSION 3.17)
project(Lab3 C)

set(CMAKE_C_STANDARD 99)

add_executable(Lab3 main.c TokenStream.c TokenStream.h LilString.h LilString.c Lexer.h Lexer.c Parser.c Parser.h Stack.h Stack.c)