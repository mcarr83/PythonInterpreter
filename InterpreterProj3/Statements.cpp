//
// Created by Michael Carr
//

#include "Statements.hpp"
#include <utility>

//Statement
Statement::Statement() = default;

//Statements
Statements::Statements() : _indentLevel{0} {}
Statements::Statements(int indentLevel) : _indentLevel{indentLevel} {}

void Statements::addStatement(Statement *statement) { _statements.push_back(statement); }

void Statements::print() {
    for (auto s: _statements){
        std::cout << std::endl;
        for (int i = 0; i < _indentLevel; i++) std::cout << " ";
        s->indentLevel = _indentLevel;
        s->print();
    }
}

void Statements::evaluate(SymTab &symTab) {
    for (auto s: _statements)
        s->evaluate(symTab);
}

//AssignmentStatement
AssignmentStatement::AssignmentStatement() : _lhsVariable{""}, _rhsExpression{nullptr} {}

AssignmentStatement::AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr):
        _lhsVariable{std::move(lhsVar)}, _rhsExpression{rhsExpr} {}

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
}

//ForStatement with friend Range
Range::Range(int _rangeValue) : initVal{0}, rangeVal{_rangeValue}, stepVal{1}, count{0} {}
Range::Range(int _initValue, int _rangeValue) : initVal{_initValue}, rangeVal{_rangeValue}, stepVal{1}, count{_initValue} {}
Range::Range(int _initValue, int _rangeValue, int _stepValue) : initVal{_initValue}, rangeVal{_rangeValue}, stepVal{_stepValue}, count{_initValue} {}

bool Range::condition() {return count < rangeVal;}
int Range::next() { count += stepVal; return count;}

ForStatement::ForStatement() : _counter{""}, _rangeStatement{nullptr}, _statements{nullptr} {}

ForStatement::ForStatement(std::string counterVar, Range *rangeStatement, Statements *stmts) :
        _counter{std::move(counterVar)}, _rangeStatement{rangeStatement}, _statements{stmts} {}

void ForStatement::evaluate(SymTab &symTab) {
    auto* varDesc = new NumDescriptor(_rangeStatement->initVal);
    symTab.setValueFor(_counter, varDesc);
    _rangeStatement->count = _rangeStatement->initVal;
    while(_rangeStatement->condition()){
        _statements->evaluate(symTab);
        auto* varUpdate = new NumDescriptor(_rangeStatement->next());
        symTab.setValueFor(_counter, varUpdate);
    }
}

void ForStatement::print() {
    std::cout << "for " << _counter << " in range(" << _rangeStatement->initVal
              << ", " << _rangeStatement->rangeVal << ", " << _rangeStatement->stepVal << "):";
    _statements->print();
}

// PrintStatement
PrintStatement::PrintStatement() : _list{} {}

PrintStatement::PrintStatement(std::vector<ExprNode*> list) : _list{std::move(list)} {};

void PrintStatement::evaluate(SymTab &symTab){
    for(ExprNode* e : _list){
        TypeDescriptor typeDescriptor = *e->evaluate(symTab);
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

//IfStatement
IfStatement::IfStatement() : _condition{nullptr}, _stmts{nullptr}, _elif{nullptr} {}
IfStatement::IfStatement(ExprNode* condition, Statements* stmts) : _condition{condition}, _stmts{stmts}, _elif{nullptr} {}

void IfStatement::evaluate(SymTab &symTab) {
    if((_condition == nullptr) || (_condition->evaluate(symTab)->_n > 0))
        _stmts->evaluate(symTab);
    else
    if(_elif != nullptr)
        _elif->evaluate(symTab);
}

void IfStatement::print() {
    //if statement print
    std::cout << "if ";
    _condition->print();
    std::cout << ":";
    _stmts->print();
    IfStatement* next = _elif;
    while(next != nullptr) {
        if (next->_condition == nullptr){
            std::cout << std::endl;
            for (int i = 0; i < indentLevel; i++) std::cout << " ";
            std::cout << "else";
        } else {
            std::cout << std::endl;
            for (int i = 0; i < indentLevel; i++) std::cout << " ";
            std::cout << "elif ";
            next->_condition->print();
        }
        std::cout << ":";
        next->_stmts->print();
        next = next->_elif;
    }
}

void IfStatement::setElif(IfStatement *elif) {
    _elif = elif;
}
