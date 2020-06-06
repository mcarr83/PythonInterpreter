//
// Created by Michael Carr
//
#include <iostream>
#include <string>
#include <algorithm> //used for find()
#include "Tokenizer.hpp"

std::string Tokenizer::readName() {
    // This function is called when it is known that
    // the first character in input is an alphabetic character.
    // The function reads and returns all characters of the name.

    std::string name;
    char c;
    while( inStream.get(c) && isalnum(c) ) {
        name += c;
    }
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
    return name;
}

double Tokenizer::readNumber() {
    // This function is called when it is known that
    // the first character in input is a digit.
    // The function reads and returns all remaining digits.

    double value = 0;
    std::string str;
    char c;

    //peek at the next character
    c = inStream.peek();
    while(isdigit(c) || (c == '.')) {
        inStream.get(c);
        str += c;
        c = inStream.peek();
    }
    //turns string to double before returning
    value = std::stod(str);
    return value;
}

Tokenizer::Tokenizer(std::ifstream &stream): ungottenToken{false}, inStream{stream}, lastToken{}, newLineParse{true} {stack.push_back(0);}

Token Tokenizer::getToken() {

    if(ungottenToken) {
        ungottenToken = false;
        return lastToken;
    }

    char c;
    int spaceCount = 0;
    while( inStream.get(c) && isspace(c) && c != '\n' )  // Skip spaces but not new-line chars.
        spaceCount++;

    //used to throw away comments from being evaluated
    if(c == '#') {
        while (inStream.get(c) && c != '\n')
            ;
    }
    if(inStream.bad()) {
        std::cout << "Error while reading the input stream in Tokenizer.\n";
        exit(1);
    }

    Token token;
    if( inStream.eof()) {
        //stack size is more than 1
        if(stack.size() > 1){
            stack.pop_back();
            token.isDedent() = true;
            inStream.unget();
        } else
            token.eof() = true;

    } else if( c == '\n' ) {  // will not ever be the case unless new-line characters are not supressed.
        token.eol() = true;
    }
    //Used for indents.
    else if(newLineParse && spaceCount != stack.back()){
        if (spaceCount > stack.back()) {
            inStream.unget();
            token.setIndent(spaceCount);
            stack.push_back(spaceCount);
        }
        else if (spaceCount < stack.back()) {
            auto it = std::find(stack.begin(), stack.end(), spaceCount);
            if (it == stack.end()) {
                std::cout << "Invalid dedent. " << spaceCount << " space count.\n";
                exit(99);
            } else {
                stack.pop_back();
                token.isDedent() = true;
                for (int i = spaceCount; i >= 0; i--)
                    inStream.unget();
            }
        }
    }else if( isdigit(c) ) {
        // put the digit back into the input stream so
        // we read the entire number in a function
        inStream.putback(c);
        token.setNumber( readNumber() );
    }

    else if( c == '=' ){
        //peek at next variable
        char d = inStream.peek();
        //Check to see if double equals
        if ( c == '=' && d == '='){
            char next;
            inStream.get(next);
            token.symbol("==");
        }
        else
            //if not == it is just =
            token.symbol(c);
    }
        //if <= >= or !=
    else if ((c == '<' || c == '>' || c == '!') && inStream.peek() == '='){
        char d;
        inStream.get(d);
        if (c == '<'){
            token.symbol("<=");
        }
        else if (c == '>'){
            token.symbol(">=");
        }
        else if (c == '!'){
            token.symbol("!=");
        }
    }

    else if( c == '+' || c == '-' || c == '*' || c == '/' || c == '%') {
        //added project 2
        char d = inStream.peek();
        if (c == '/' && d == '/'){
            inStream.get(c);
            token.symbol("//");
        }
        else
            token.symbol(c);
    }

    else if( c == '(' || c == ')' || c == '{' || c == '}' || c == ';' ||  c == ',' || c == '<' || c == '>' || c == ':')
        token.symbol(c);

        //Project 2 added
        //used to get string tied to a variable
    else if( c == '"') {
        std::string str;
        while (inStream.peek() != '"') {
            inStream.get(c);
            //If a new line or return character throw it away
            while (c == '\n' || c == '\r'){
                inStream.get(c);
            }
            str += c;
        }
        inStream.get(c);
        token.setString(str);
    }

    else if( c == '\'') {
        std::string str;
        while (inStream.peek() != '\'') {
            inStream.get(c);
            str += c;
        }
        token.setString(str);

    }

    else if(isalpha(c)) {  // an identifier?
        // put c back into the stream so we can read the entire name in a function.
        inStream.putback(c);
        token.setName( readName() );
    }
    else {
        std::cout << "Unknown character in input. ->" << c << "<-" << std::endl;
        exit(1);
    }

    newLineParse = token.eol() || token.isDedent();
    _tokens.push_back(token);
    return lastToken = token;
}

void Tokenizer::ungetToken() {
    ungottenToken = true;
}

void Tokenizer::printProcessedTokens() {
    for(auto & _token : _tokens) {
        _token.print();
        std::cout << std::endl;
    }
}
int Tokenizer::getIndent() { return stack.back(); }
