//
// Created by Michael Carr
//

#ifndef STATEMENTSINTERPRETER_PARSER_HPP
#define STATEMENTSINTERPRETER_PARSER_HPP


#include "Token.hpp"
#include "Tokenizer.hpp"
#include "SymTab.hpp"
#include "Expr.hpp"
#include "Statements.hpp"

#include<vector>
#include<iostream>
#include<map>


class Parser {
public:
    explicit Parser(Tokenizer &tokenizer) : tokenizer{tokenizer} {}

    Statements *fileInput();
    Statement *stmt();
    Statement *simple_stmt();
    PrintStatement *print_stmt();
    AssignmentStatement *assign_stmt();
    Statement *compoundStatement();

    Statements *suite();
    IfStatement *if_stmt();
    ForStatement *forStatement();

    ExprNode *test();
    std::vector<ExprNode*> testlist();
    ExprNode *orTest();
    ExprNode *andTest();
    ExprNode *notTest();
    ExprNode *comparison();

    Token comp_op();
    ExprNode *arith_expr();
    ExprNode *term();
    ExprNode *factor();
    ExprNode *atom();

private:
    Tokenizer &tokenizer;
    void die(std::string where, std::string message, Token &token);

};

#endif //STATEMENTSINTERPRETER_PARSER_HPP
