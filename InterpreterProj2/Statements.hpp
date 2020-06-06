//
// Created by admin on 3/13/2020.
//

#ifndef STATEMENTSINTERPRETER_STATEMENTS_HPP
#define STATEMENTSINTERPRETER_STATEMENTS_HPP

#include <iostream>
#include <iomanip>
#include <vector>

#include "Expr.hpp"
#include "SymTab.hpp"

// The Statement (abstract) class serves as a super class for all statements that
// are defined in the language. Ultimately, statements have to be evaluated.
// Therefore, this class defines evaluate, a pure-virtual function to make
// sure that all subclasses of Statement provide an implementation for this
// function.

class Statement {
public:
    Statement();

    virtual void print() = 0;
    virtual void evaluate(SymTab &symTab) = 0;

};


// Statements is a collection of Statement. For example, all statements in a function
// can be represented by an instance of Statements.

class Statements {
public:
    Statements();

    void addStatement(Statement *statement);
    void evaluate(SymTab &symTab);

    void print();

private:
    std::vector<Statement *> _statements;
};

// AssignmentStatement represents the notion of an lValue having been assigned an rValue.
// The rValue is an expression.

class AssignmentStatement : public Statement {
public:
    AssignmentStatement();
    AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr);

    std::string &lhsVariable();
    ExprNode *&rhsExpression();

    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    std::string _lhsVariable;
    ExprNode *_rhsExpression;
};

// ForStatement

class ForStatement : public Statement{
public:
    ForStatement();
    ForStatement(AssignmentStatement *initVar, ExprNode *condExpr, AssignmentStatement *varUpdate, Statement *stmt);

    AssignmentStatement *&initialVariable();
    ExprNode *&conditionalExpression();
    AssignmentStatement *&variableUpdate();
    Statement *statement();

    void evaluate(SymTab &symTab) override;
    void print() override;

private:
    AssignmentStatement *_initialVariable;
    ExprNode *_conditionalExpression;
    AssignmentStatement *_variableUpdate;
    Statement *_statement;
};

// PrintStatement

class PrintStatement : public Statement{
public:
    PrintStatement();
    explicit PrintStatement(std::vector<ExprNode*> list);

    std::vector<ExprNode*> &list();

    void evaluate(SymTab &symTab) override;
    void print() override;

private:
    std::vector<ExprNode*> _list;
};


#endif //STATEMENTSINTERPRETER_STATEMENTS_HPP
