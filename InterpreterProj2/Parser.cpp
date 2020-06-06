//
// Created by Michael Carr
//

#include<vector>
#include<iostream>
#include "Token.hpp"
#include "Parser.hpp"
#include "Statements.hpp"

// Parser functions

void Parser::die(std::string where, std::string message, Token &token) {
    std::cout << where << " " << message << std::endl;
    token.print();
    std::cout << std::endl;
    std::cout << "\nThe following is a list of tokens that have been identified up to this point.\n";
    tokenizer.printProcessedTokens();
    exit(1);
}

Statements *Parser::fileInput() {
    //file_input: {NEWLINE | stmt}* ENDMARKER

    //allocate for a new statement
    Statements *stmts = new Statements();

    //Grab a token
    Token tok = tokenizer.getToken();

    //while its not an end of file
    while (!tok.eof()) {
        //while its an end of line token, means many blank lines can come before the statement
        //keep grabbing tokens
        while (tok.eol())
            tok = tokenizer.getToken();
        tokenizer.ungetToken();

        //Set up a statement pointer
        Statement* stmtptr = stmt();
        stmts->addStatement(stmtptr);
        tok = tokenizer.getToken();
        while (tok.eol())
            tok = tokenizer.getToken();
    }
    return stmts;
}

Statement *Parser::stmt(){
    //stmt: simple_stmt | compound_stmt

    //get a token
    Token tok = tokenizer.getToken();

    //if it is keyword print or name, unget the token and head to simple_stmt
    if(tok.isPrint() || tok.isName()) {
        tokenizer.ungetToken();
        return simple_stmt();
    }

    //same as above but go to compound_stmt
    else if(tok.isForLoop()) {
        tokenizer.ungetToken();
        return compound_stmt();
    }

    //should never get here
    else
        return nullptr;
}

Statement *Parser::simple_stmt(){
    //simple_stmt: (print_stmt | assign_stmt) NEWLINE

    //Get a token
    Token tok = tokenizer.getToken();

    //If the token in the print keyword, unget, go to print_stmt
    if(tok.isPrint()){
        tokenizer.ungetToken();
        return print_stmt();
    }

    //If the token in the print keyword, unget, go to assign_stmt
    else if (tok.isName()){
        tokenizer.ungetToken();
        return assign_stmt();
    }
    else
        //It should never get here
        return nullptr;
}

Statement *Parser::compound_stmt() {
    //compound_stmt: for_stmt

    //Grab a token
    Token tok = tokenizer.getToken();

    //If the token in the print keyword, unget, go to for_stmt
    if(tok.isForLoop()){
        tokenizer.ungetToken();
        return for_stmt();
    }
    else
        //it should never get here
        return nullptr;
}

AssignmentStatement *Parser::assign_stmt() {
    //assign_stmt: ID ’=’ test

    //Get token, in this case a variable name
    Token varName = tokenizer.getToken();
    if (!varName.isName())
        die("Parser::assign_stmt", "Expected a var name token, instead got", varName);

    //Get token, in this case a assignment operator
    Token assignOp = tokenizer.getToken();
    if (!assignOp.isAssignmentOperator())
        die("Parser::assign_stmt", "Expected an equal sign, instead got", assignOp);

    //Set up right hand side expression
    ExprNode *rightHandSideExpr = test();

    //allocate memory and pass to AssignmentStatement class
    return new AssignmentStatement(varName.getName(), rightHandSideExpr);
}

ForStatement *Parser::for_stmt(){
    //for_stmt: ’for’ ’(’ assign_stmt ’;’ test ’;’ assign_stmt ’)’ ’{’ NEWLINE stmt ’}’ NEWLINE

    //Grab a token and make sure its a for
    Token tok = tokenizer.getToken();
    if(!tok.isForLoop())
        die("Parser::for_stmt", "Expected keyword for, instead got", tok);

    //grab another make sure that is a (
    tok = tokenizer.getToken();
    if(!tok.isOpenParen())
        die("Parser::for_stmt", "Expected an open parenthesis, instead got", tok);

    //keep grabbing a a token until you find the start to an assignment
    //in other words if a '\n\n\n\n' is found disregard
    tok = tokenizer.getToken();
    while(tok.eol())
        tok = tokenizer.getToken();
    tokenizer.ungetToken();

    AssignmentStatement *varInitialize = assign_stmt();

    //keep grabbing a token until you reach a semi colon
    tok = tokenizer.getToken();
    while(tok.eol())
        tok = tokenizer.getToken();

    if(!tok.isSemiColon())
        die("Parser::for_stmt", "Expected a semicolon, instead got", tok);

    //grab another token until an ExprNode of test is found
    tok = tokenizer.getToken();
    while(tok.eol())
        tok = tokenizer.getToken();
    tokenizer.ungetToken();

    ExprNode *testptr = test();

    //grab token until a semi colon is found
    tok = tokenizer.getToken();
    while(tok.eol())
        tok = tokenizer.getToken();

    if(!tok.isSemiColon())
        die("Parser::for_stmt", "Expected a semicolon, instead got", tok);

    //keep grabbing tokens until an Assignment statement is found
    tok = tokenizer.getToken();
    while(tok.eol())
        tok = tokenizer.getToken();
    tokenizer.ungetToken();

    AssignmentStatement *varUpdate = assign_stmt();

    //keeping grabbing tokens until a close parenthesis is found
    tok = tokenizer.getToken();
    while(tok.eol())
        tok = tokenizer.getToken();

    if(!tok.isCloseParen())
        die("Parser::for_stmt", "Expected a close parenthesis, instead got", tok);
    //tok = tokenizer.getToken();

    //keep grabbing tokens until an open brackets is found
    tok = tokenizer.getToken();
    while(tok.eol())
        tok = tokenizer.getToken();

    if(!tok.isOpenBrackets())
        die("Parser::for_stmt", "Expected a open bracket, instead got", tok);

    tok = tokenizer.getToken();

    //if we were dealing in C++ this statement would not be here
    if(!tok.eol())
        die("Parser::for_stmt", "Expected a new line, instead got", tok);

    //keep grabbing token until a statement is found
    while(tok.eol())
        tok = tokenizer.getToken();
    tokenizer.ungetToken();

    Statement* statement = stmt();

    //keep grabbing tokens until closed brackets is found
    tok = tokenizer.getToken();
    while(tok.eol())
        tok = tokenizer.getToken();

    if(!tok.isCloseBrackets())
        die("Parser::for_stmt", "Expected a closed bracket, instead got", tok);

    //return a allocated ForStatement to Statements
    return new ForStatement(varInitialize, testptr, varUpdate, statement);
}

PrintStatement *Parser::print_stmt(){
    //print_stmt: ’print’ [ testlist ]

    //Grab a token and make sure it's print
    Token tok = tokenizer.getToken();
    if(!tok.isPrint())
        die("Parser::print_stmt", "Expected keyword 'print', instead got", tok);

    //get a vector of stuff to print
    tok = tokenizer.getToken();
    std::vector<ExprNode*> list;
    if(!tok.eol()){
        tokenizer.ungetToken();
        list = testlist();
    }

    return new PrintStatement(list);
}

std::vector<ExprNode*> Parser::testlist(){
    //testlist: test {’,’ test}*

    std::vector<ExprNode*> t_list;
    ExprNode *tester = test();
    t_list.push_back(tester);
    Token tok = tokenizer.getToken();
    while (tok.isComma()){
        t_list.push_back(test());
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return t_list;
}

ExprNode *Parser::test(){
    //test: comparision

    return comparison();
}

ExprNode *Parser::comparison(){
    //comparison: arith_expr {comp_op arith_expr}*

    ExprNode* left = arith_expr();

    //Grab a token and see if it's a Relational Operator
    Token tok = tokenizer.getToken();
    while(tok.isRelationalOperator()){
        //evaluate
        tokenizer.ungetToken();
        InfixExprNode *p = new InfixExprNode(comp_op());
        p->left() = left;
        p->right() = arith_expr();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

Token Parser::comp_op(){
    //comp_op: ’<’|’>’|’==’|’>=’|’<=’|’<>’|’!=’

    Token tok = tokenizer.getToken();
    if(!tok.isRelationalOperator())
        die("Parser::comp_op", "Expected relational operator, instead got", tok);
    return tok;
}

ExprNode *Parser::arith_expr() {
    //arith_expr: term {(’+’|’-’) term}*

    //where addition and subtraction is evaluated
    ExprNode *left = term();
    Token tok = tokenizer.getToken();
    while (tok.isAdditionOperator() || tok.isSubtractionOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = term();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}


ExprNode *Parser::term() {
    //term: factor {(’*’|’/’|’%’|’//’) factor}*

    //very similar to project 1 but had to be rewritten
    ExprNode *left = factor();
    Token tok = tokenizer.getToken();

    while (tok.isMultiplicationOperator() || tok.isDivisionOperator() || tok.isModuloOperator() || tok.isIntDivisionOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = factor();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::factor() {
    //factor: {’-’} factor | atom

    //get and token and see if it's a subtraction operator
    Token tok = tokenizer.getToken();
    if(tok.isSubtractionOperator()){
        //evaluate
        InfixExprNode* p = new InfixExprNode(tok);

        //need two tokens with one set to zero
        Token t;
        t.setNumber(0);
        p->left() = new Number(t);
        ExprNode* fact = factor();

        //check data types
        if(typeid(fact) == typeid(String))
            die("Parser::factor", "Invalid use of negative sign on string", tok);
        p->right() = fact;
        return p;
    }
    else{
        tokenizer.ungetToken();
        return atom();
    }
}

ExprNode *Parser::atom(){
    //atom: ID | NUMBER | STRING+ | ’(’ test ’)’

    //Same as arith_primary from Project 1
    Token tok = tokenizer.getToken();

    if(tok.isName())
        return new Variable(tok);
    else if(tok.isNumber())
        return new Number(tok);
    else if(tok.isString())
        return new String(tok);
    else if (tok.isOpenParen()) {
        ExprNode* p = test();
        tok = tokenizer.getToken();
        if (!tok.isCloseParen())
            die("Parser::atom", "Expected close-parenthesis, instead got", tok);
        return p;
    }
    else //Will not reach this statement!
        return nullptr;
}