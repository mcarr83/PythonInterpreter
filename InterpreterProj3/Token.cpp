//
// Created by Michael Carr
//
#include <iostream>
#include "Token.hpp"

Token::Token(): _name{""}, _eof{false}, _eol{false}, _symbol{'\0'}, _isNumber{false}, _longSymbol{""}, _string{""}, _number(0), _isString{false}, _isIndent{false},  _isDedent{false} {}

void Token::print() const {
    if( eol() ) std::cout << "EOL" ;
    else if( eof() )                    std::cout << "EOF" ;
    else if( isOpenParen() )  std::cout << "(" ;
    else if( isCloseParen() )  std::cout << ")" ;
    else if(isOpenBrackets() )  std::cout << "{" ;
    else if(isCloseBrackets() )  std::cout << "}" ;
    else if( isAssignmentOperator() )  std::cout << " = " ;
    else if( isMultiplicationOperator() )  std::cout << " * " ;
    else if( isAdditionOperator() )  std::cout << " + " ;
    else if( isSubtractionOperator() )  std::cout << " - " ;
    else if( isModuloOperator() )  std::cout << " % " ;
    else if( isDivisionOperator() )  std::cout << " / " ;

    //Project 1
    else if(isEqualEqual() ) std::cout << " == ";
    else if(isNotEqual() ) std::cout << " != ";
    else if (isGreaterThan() ) std::cout << " > ";
    else if (isLessThan() ) std::cout << " < ";
    else if (isGreaterThanOrEqual() ) std::cout << " >= ";
    else if (isLessThanOrEqual() ) std::cout << " <= ";
    else if( isName() || isKeyword())  std::cout << getName();
    else if( isNumber() ) std::cout << getNumber();

    //Project 2
    else if( isComma() ) std::cout << ", " ;
    else if( isIntDivisionOperator() ) std::cout << " // ";
    else if( isString() ) std::cout << getString();

    //Project 3
    else if( isColon() )  std::cout << ":" ;
    else if( isIndent() ){ std::cout << "INDENT";}
    else if ( isDedent()){ std::cout << "DEDENT";}


    else std::cout << "Uninitialized token.\n";
}
