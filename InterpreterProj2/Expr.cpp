//
// Created by Michael Carr
//

#include<iostream>
#include <utility>
#include "Expr.hpp"

// ExprNode
ExprNode::ExprNode(Token token): _token{std::move(token)} {}

Token ExprNode::token() { return _token; }

// InfixExprNode functions
InfixExprNode::InfixExprNode(Token tk) : ExprNode{tk}, _left(nullptr), _right(nullptr) {}

ExprNode *&InfixExprNode::left() { return _left; }

ExprNode *&InfixExprNode::right() { return _right; }
TypeDescriptor* InfixExprNode::evaluate(SymTab &symTab) {
    // Evaluates an infix expression using a post-order traversal of the expression tree.
    TypeDescriptor* rValue = right()->evaluate(symTab);
    TypeDescriptor* lValue = left()->evaluate(symTab);
    // std::cout << "InfixExprNode::evaluate: " << lValue << " " << token().symbol() << " " << rValue << std::endl;
    if(lValue->type() != rValue->type()){
        std::cout << "InfixExprNode::evaluate: attempted operation on incompatible types\n" << lValue->type() << " " << rValue->type();
        token().print();
        std::cout << std::endl;
        exit(18);
    }

    else if(token().isAdditionOperator()){
        return *lValue + *rValue;
    }
    else if(token().isSubtractionOperator()){
        return *lValue - *rValue;
    }
    else if(token().isMultiplicationOperator())
        return *lValue * *rValue;
    else if(token().isDivisionOperator())
        return *lValue / *rValue; // division by zero?
    else if(token().isIntDivisionOperator()){
        TypeDescriptor* n = *lValue / *rValue;
        n->_n = (int)n->_n;
        return n;
    }
    else if(token().isModuloOperator())
        return *lValue % *rValue;

    //All relational operators had to be redone to not return ints. :-(
    else if(token().isEqualEqual())
        return *lValue == *rValue;
    else if(token().isNotEqual())
        return *lValue != *rValue;
    else if(token().isGreaterThan())
        return *lValue > *rValue;
    else if(token().isLessThan())
        return *lValue < *rValue;
    else if(token().isGreaterThanOrEqual())
        return *lValue >= *rValue;
    else if(token().isLessThanOrEqual()){
        return *lValue <= *rValue;
    }
    else {
        std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator or operands\n";
        token().print();
        std::cout << std::endl;
        exit(15);
    }
}

void InfixExprNode::print() {
    _left->print();
    token().print();
    _right->print();
}

// Number
Number::Number(Token token): ExprNode{std::move(token)} {}

void Number::print() {
    token().print();
}

TypeDescriptor* Number::evaluate(SymTab &symTab) {
    // std::cout << "WholeNumber::evaluate: returning " << token().getWholeNumber() << std::endl;
    return new NumDescriptor(token().getNumber());
}

//String
String::String(Token token): ExprNode{std::move(token)} {}

void String::print() {
    std::cout << '"';
    token().print();
    std::cout << '"';
}

TypeDescriptor* String::evaluate(SymTab &symTab) {
    return new StringDescriptor(token().getString());;
}

//Variable
Variable::Variable(Token token): ExprNode{std::move(token)} {}

void Variable::print() {
    token().print();
}

TypeDescriptor* Variable::evaluate(SymTab &symTab) {
    if( ! symTab.isDefined(token().getName())) {
        std::cout << "Use of undefined variable, " << token().getName() << std::endl;
        exit(20);
    }
    // std::cout << "Variable::evaluate: returning " << symTab.getValueFor(token().getName()) << std::endl;
    return symTab.getValue(token().getName());
}

