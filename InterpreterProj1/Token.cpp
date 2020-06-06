
#include <iostream>
#include "Token.hpp"


Token::Token(): _name{""}, _eof{false}, _eol{false}, _symbol{'\0'}, _isWholeNumber{false} {}

void Token::print() const {
    if( eol() ) std::cout << "EOL\n" ;
    else if( eof() )                    std::cout << "EOF" ;
    else if( isOpenParen() )  std::cout << "(" ;
    else if( isCloseParen() )  std::cout << ")" ;
    else if(isOpenBrackets() )  std::cout << "{" ;
    else if(isCloseBrackets() )  std::cout << "}" ;
    else if( isAssignmentOperator() )  std::cout << " = " ;
    else if( isSemiColon() )  std::cout << ";" ;
    else if( isMultiplicationOperator() )  std::cout << " * " ;
    else if( isAdditionOperator() )  std::cout << " + " ;
    else if( isSubtractionOperator() )  std::cout << " - " ;
    else if( isModuloOperator() )  std::cout << " % " ;
    else if( isDivisionOperator() )  std::cout << " / " ;
    else if(isEqualEqual() ) std::cout << " == ";
    else if(isNotEqual() ) std::cout << " != ";
    else if (isGreaterThan() ) std::cout << " > ";
    else if (isLessThan() ) std::cout << " < ";
    else if (isGreaterThanOrEqual() ) std::cout << " >= ";
    else if (isLessThanOrEqual() ) std::cout << " <= ";
    else if( isName() || isKeyword())  std::cout << getName();
    else if( isWholeNumber() ) std::cout << getWholeNumber();
    else std::cout << "Uninitialized token.\n";
}


