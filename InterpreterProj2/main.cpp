/*
 * Name: Michael Carr
 * Project 3 but Python Project 2
 * 3/13/2020
 */

#include <iostream>
#include<vector>


#include "Token.hpp"
#include "Tokenizer.hpp"
#include "Parser.hpp"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " nameOfAnInputFile\n";
        exit(1);
    }

    std::ifstream inputStream;
    inputStream.open(argv[1], std::ios::in);
    if (!inputStream.is_open()) {
        std::cout << "Unable top open " << argv[1] << ". Terminating...";
        perror("Error when attempting to open the input file.");
        exit(2);
    }
    Tokenizer tokenizer(inputStream);
    Parser parser(tokenizer);
    Statements *statements = parser.fileInput();
    Token lastToken = tokenizer.getToken();
    while(!lastToken.eof()) {
        lastToken = tokenizer.getToken();
    }
    SymTab symTab;
    statements->print();
    statements->evaluate(symTab);
    std::cout << std::endl << "Symbol table contains the following variables.\n";
    symTab.print();

    return 0;
}
