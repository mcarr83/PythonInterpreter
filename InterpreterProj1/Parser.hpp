#ifndef __PARSER_HPP
#define __PARSER_HPP

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
    Parser(Tokenizer &tokenizer) : tokenizer{tokenizer} {}

    Statements *statements();

    AssignmentStatement *assignStatement();
    ForLoop *forLoop();
    PrintStatement *printStatement();

    ExprNode *rel_expr();
    ExprNode *rel_term();
    ExprNode *rel_primary();

    ExprNode *arith_expr();
    ExprNode *arith_term();
    ExprNode *arith_primary();

    std::string id();

private:
    Tokenizer &tokenizer;

    void die(std::string where, std::string message, Token &token);

};
#endif

