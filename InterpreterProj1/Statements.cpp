
#include "Statements.hpp"

// Statement
Statement::Statement() {}

// Statements

Statements::Statements() {}
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

// AssignmentStatement

AssignmentStatement::AssignmentStatement() : _lhsVariable{""}, _rhsExpression{nullptr} {}

AssignmentStatement::AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr):
        _lhsVariable{lhsVar}, _rhsExpression{rhsExpr} {}

void AssignmentStatement::evaluate(SymTab &symTab) {
    int rhs = rhsExpression()->evaluate(symTab);
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

// ForLoop

//Gets a local variable, finds the initial condition, evaluates the statement
//then updates the variable
ForLoop::ForLoop() : _initialVariable{nullptr}, _conditionalExpression{nullptr}, _variableUpdate{nullptr}, _statements{nullptr} {}

ForLoop::ForLoop(AssignmentStatement *initVar, ExprNode *condExpr, AssignmentStatement *varUpdate, Statements *stmts) :
        _initialVariable{initVar}, _conditionalExpression{condExpr}, _variableUpdate{varUpdate}, _statements{stmts} {}


void ForLoop::evaluate(SymTab &symTab) {
    _initialVariable->evaluate(symTab);
    while(_conditionalExpression->evaluate(symTab)){
        _statements->evaluate(symTab);
        _variableUpdate->evaluate(symTab);
    }
}

AssignmentStatement *&ForLoop::initialVariable(){
    return _initialVariable;
}

ExprNode *&ForLoop::conditionalExpression() {
    return _conditionalExpression;
}

AssignmentStatement *&ForLoop::variableUpdate() {
    return _variableUpdate;
}

//Special print function for loops only
void ForLoop::print() {
    std::cout << "for (";
    _initialVariable->print();
    std::cout << "; ";
    _conditionalExpression->print();
    std::cout << "; ";
    _variableUpdate->print();
    std::cout << ") {" << std::endl;
    std::cout << std::setw(10);
    _statements->print();
    std::cout << std::setw(0);
    std::cout << "}";
}

// Printstatement used to be able to use print keyword in
// printing a variable. Gets value and prints it ie print(a)
// or print(5)

PrintStatement::PrintStatement() : _variableName{""} {}

PrintStatement::PrintStatement(std::string var) : _variableName{var} {};

void PrintStatement::evaluate(SymTab &symTab){
    std::cout << symTab.getValueFor(_variableName) << std::endl;
}

std::string &PrintStatement::variableName(){
    return _variableName;
}

void PrintStatement::print() {
    std::cout << "print " << _variableName;
}