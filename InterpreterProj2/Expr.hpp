//
// Created by Michael Carr
//

#ifndef STATEMENTSINTERPRETER_EXPR_HPP
#define STATEMENTSINTERPRETER_EXPR_HPP

#include "Token.hpp"
#include "SymTab.hpp"

// Classes in this file define the internal representation of arithmetic expressions.


// An ExprNode serves as the base class (super class) for arithmetic expression.
// It forces the derived classes (subclasses) to implement two functions, print and
// evaluate.
class ExprNode {
public:
    explicit ExprNode(Token token);
    Token token();
    virtual void print() = 0;
    virtual TypeDescriptor* evaluate(SymTab &symTab) = 0;

private:
    Token _token;
};


// An InfixExprNode is useful to represent binary arithmetic operators.
class InfixExprNode: public ExprNode {  // An expression tree node.

public:
    explicit InfixExprNode(Token tk);
    ExprNode *&left();
    ExprNode *&right();
    virtual void print();
    virtual TypeDescriptor* evaluate(SymTab &symTab);

private:
    ExprNode *_left, *_right;
};

// Number is a leaf-node in an expression tree. It corresponds to
// a terminal in the production rules of the grammar that describes the
// syntax of arithmetic expressions.

class Number: public ExprNode {
public:
    explicit Number(Token token);
    virtual void print();
    virtual TypeDescriptor* evaluate(SymTab &symTab);
};

class String: public ExprNode {
public:
    explicit String(Token token);
    virtual void print();
    virtual TypeDescriptor* evaluate(SymTab &symTab);
};

// Variable is a leaf-node in an expression tree. It corresponds to
// a terminal in the production rules of the grammar that describes the
// syntax of arithmetic expressions.

class Variable: public ExprNode {
public:
    explicit Variable(Token token);
    virtual void print();
    virtual TypeDescriptor* evaluate(SymTab &symTab);
};

#endif //STATEMENTSINTERPRETER_EXPR_HPP
