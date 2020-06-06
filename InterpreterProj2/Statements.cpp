//
// Created by Michael Carr
//

#include "Statements.hpp"

// Statement
Statement::Statement() = default;

// Statements
Statements::Statements() = default;
void Statements::addStatement(Statement *statement) { _statements.push_back(statement); }

void Statements::print() {
    for (auto s: _statements){
        s->print();
        std::endl (std::cout);
    }
}

void Statements::evaluate(SymTab &symTab) {
    for (auto s: _statements)
        s->evaluate(symTab);
}

//AssignmentStatement

AssignmentStatement::AssignmentStatement() : _lhsVariable{""}, _rhsExpression{nullptr} {}
AssignmentStatement::AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr):
        _lhsVariable{lhsVar}, _rhsExpression{rhsExpr} {}

void AssignmentStatement::evaluate(SymTab &symTab) {
    TypeDescriptor* rhs = rhsExpression()->evaluate(symTab);
    symTab.setValueFor(lhsVariable(), rhs);
}

std::string &AssignmentStatement::lhsVariable() {
    return _lhsVariable;
}

ExprNode *&AssignmentStatement::rhsExpression() {
    return _rhsExpression;
}

void AssignmentStatement::print() {
    std::cout << _lhsVariable << " = ";
    _rhsExpression->print();
    //std::cout << std::endl;
}

// ForLoop Statement

ForStatement::ForStatement() : _initialVariable{nullptr}, _conditionalExpression{nullptr}, _variableUpdate{nullptr}, _statement{nullptr} {}
ForStatement::ForStatement(AssignmentStatement *initVar, ExprNode *condExpr, AssignmentStatement *varUpdate, Statement *stmt) :
        _initialVariable{initVar}, _conditionalExpression{condExpr}, _variableUpdate{varUpdate}, _statement{stmt} {}

void ForStatement::evaluate(SymTab &symTab) {
    _initialVariable->evaluate(symTab);
    while(_conditionalExpression->evaluate(symTab)->_n > 0){
        _statement->evaluate(symTab);
        _variableUpdate->evaluate(symTab);
    }
}

AssignmentStatement *&ForStatement::initialVariable(){
    return _initialVariable;
}

ExprNode *&ForStatement::conditionalExpression() {
    return _conditionalExpression;
}

AssignmentStatement *&ForStatement::variableUpdate() {
    return _variableUpdate;
}

void ForStatement::print() {
    std::cout << "for (";
    _initialVariable->print();
    std::cout << "; ";
    _conditionalExpression->print();
    std::cout << "; ";
    _variableUpdate->print();
    std::cout << ") {" << std::endl;
    std::cout << std::setw(10);
    _statement->print();
    std::cout << std::setw(0);
    std::cout << "}";
}

//Print Statement

PrintStatement::PrintStatement() : _list{} {}
PrintStatement::PrintStatement(std::vector<ExprNode*> list) : _list{std::move(list)} {};

void PrintStatement::evaluate(SymTab &symTab){
    for(ExprNode* exprNode : _list){
        TypeDescriptor typeDescriptor = *exprNode->evaluate(symTab);
        std::cout << typeDescriptor << " ";
    }
    std::cout << std::endl;
}

std::vector<ExprNode*> &PrintStatement::list(){
    return _list;
}

void PrintStatement::print() {
    std::cout << "print ";
    for(ExprNode* exprNode : _list){
        exprNode->print();
        std::cout << ", ";
    }
    std::cout << std::endl;
}
