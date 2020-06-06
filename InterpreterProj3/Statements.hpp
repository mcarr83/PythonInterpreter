//
// Created by Michael Carr
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
    int indentLevel = 0;
};


// Statements is a collection of Statement. For example, all statements in a function
// can be represented by an instance of Statements.

class Statements {
public:
    Statements();
    explicit Statements(int indentLevel);
    void addStatement(Statement *statement);
    void evaluate(SymTab &symTab);
    void print();

private:
    std::vector<Statement *> _statements;
    int _indentLevel = 0;
};

// AssignmentStatement represents the notion of an lValue having been assigned an rValue.
// The rValue is an expression.

class AssignmentStatement : public Statement {
public:
    AssignmentStatement();
    AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr);
    std::string &lhsVariable();
    ExprNode *&rhsExpression();
    void evaluate(SymTab &symTab) override;
    void print() override;

private:
    std::string _lhsVariable;
    ExprNode *_rhsExpression;
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

//IfStatement

class IfStatement : public Statement{
public:
    IfStatement();
    IfStatement(ExprNode* condition, Statements* stmts);
    void evaluate(SymTab &symTab) override;
    void print() override;
    void setElif(IfStatement* elif);

private:
    ExprNode* _condition;
    Statements* _stmts;
    IfStatement* _elif;
};

class Range {
    friend class ForStatement;
public:
    explicit Range(int _rangeValue);
    Range(int _initValue, int _rangeValue);
    Range(int _initValue, int _rangeValue, int _stepValue);
    bool condition();
    int next();

private:
    int initVal;
    int stepVal;
    int rangeVal;
    int count;
};

class ForStatement : public Statement{
public:
    ForStatement();
    ForStatement(std::string counterVar, Range *rangeStatement, Statements *stmts);
    void evaluate(SymTab &symTab) override;
    void print() override;

private:
    std::string _counter;
    Range *_rangeStatement;
    Statements *_statements;
};

#endif //STATEMENTSINTERPRETER_STATEMENTS_HPP
