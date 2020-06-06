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
    Statement *compound_stmt();

    AssignmentStatement *assign_stmt();
    ForStatement *for_stmt();
    PrintStatement *print_stmt();

    ExprNode *test();
    ExprNode *comparison();
    std::vector<ExprNode*> testlist();

    ExprNode *arith_expr();
    ExprNode *term();
    ExprNode *factor();
    ExprNode *atom();

    Token comp_op();

private:
    Tokenizer &tokenizer;
    void die(std::string where, std::string message, Token &token);

};

#endif //STATEMENTSINTERPRETER_PARSER_HPP
