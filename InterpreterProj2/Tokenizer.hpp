//
// Created by Michael Carr
//

#ifndef STATEMENTSINTERPRETER_TOKENIZER_HPP
#define STATEMENTSINTERPRETER_TOKENIZER_HPP


#include <fstream>
#include <vector>
#include "Token.hpp"

class Tokenizer {

public:
    Tokenizer(std::ifstream &inStream);
    Token getToken();
    void ungetToken();
    void printProcessedTokens();

private:
    Token lastToken;
    bool ungottenToken;
    std::ifstream &inStream;
    std::vector<Token> _tokens;

private:
    std::string readName();
    double readNumber();
};
#endif //STATEMENTSINTERPRETER_TOKENIZER_HPP
