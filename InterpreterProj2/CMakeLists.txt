cmake_minimum_required(VERSION 3.15)
project(statementsInterpreter)

set(CMAKE_CXX_STANDARD 17)

add_executable(statementsInterpreter main.cpp Expr.cpp Expr.hpp Parser.cpp Parser.hpp Statements.cpp Statements.hpp SymTab.cpp SymTab.hpp Token.cpp Token.hpp Tokenizer.cpp Tokenizer.hpp TypeDescriptor.cpp TypeDescriptor.hpp)