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
        Statement* s = stmt();
        stmts->addStatement(s);
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
    Statement* stmt;
    if(tok.isPrint() || tok.isName()) {
        tokenizer.ungetToken();
        stmt = simple_stmt();
    }

    //same as above but go to compound_stmt
    else if(tok.isForLoop() || tok.isIf()) {
        tokenizer.ungetToken();
        stmt = compoundStatement();
    }

    //should never get here
    else
        stmt = nullptr;
    return stmt;
}

Statement *Parser::simple_stmt(){
    //simple_stmt: (print_stmt | assign_stmt) NEWLINE

    //Get a token
    Token tok = tokenizer.getToken();
    Statement* stmt;
    //If the token in the print keyword, unget, go to print_stmt
    if(tok.isPrint()){
        tokenizer.ungetToken();
        stmt = print_stmt();
    }
    else if (tok.isName()){
        tokenizer.ungetToken();
        stmt = assign_stmt();
    }
    else
        //it should never get here
        stmt = nullptr;
    tok = tokenizer.getToken();
    if(!tok.eol())
        die("Parser::simple_stmt", "Expected a newline token, instead got", tok);
    return stmt;
}

Statement *Parser::compoundStatement() {
    //compound_stmt: for_stmt

    //Grab a token
    Token tok = tokenizer.getToken();

    //If the token in the print keyword, unget, go to for_stmt
    if(tok.isIf()){
        tokenizer.ungetToken();
        return if_stmt();
    }
    else if(tok.isForLoop()){
        tokenizer.ungetToken();
        return forStatement();
    } else
        //It should never get here
        return nullptr;
}

PrintStatement *Parser::print_stmt(){
    Token tok = tokenizer.getToken();
    if(!tok.isPrint())
        die("Parser::print_stmt", "Expected keyword 'print', instead got", tok);
    tok = tokenizer.getToken();
    std::vector<ExprNode*> list;
    if(!tok.eol()){
        tokenizer.ungetToken();
        list = testlist();
    }
    return new PrintStatement(list);
}

AssignmentStatement *Parser::assign_stmt() {
    //assign_stmt: ID ’=’ test

    //Get token, in this case a variable name
    Token varName = tokenizer.getToken();
    if (!varName.isName())
        die("Parser::assign_stmt", "Expected a name token, instead got", varName);

    //Get token, in this case a assignment operator
    Token assignOps = tokenizer.getToken();
    if (!assignOps.isAssignmentOperator())
        die("Parser::assign_stmt", "Expected an equal sign, instead got", assignOps);

    //Set up right hand side expression
    ExprNode *rightHandSideExpr = test();

    //allocate memory and pass to AssignmentStatement class
    return new AssignmentStatement(varName.getName(), rightHandSideExpr);
}

IfStatement *Parser::if_stmt() {
    //if_stmt: 'if' test ':' suite {'elif' test ':' suite}* ['else' ':' suite]

    //Get an if keyword
    Token tok = tokenizer.getToken();
    if(!tok.isIf())
        die("Parser::if_stmt", "Expected keyword 'if', instead got", tok);

    ExprNode *rightHandSideExpr = test();

    //Get another token
    tok = tokenizer.getToken();
    if(!tok.isColon())
        die("Parser::if_stmt", "Expected a colon, instead got", tok);

    Statements* stmts = suite();

    auto* ifStmt = new IfStatement(rightHandSideExpr, stmts);

    tok = tokenizer.getToken();
    IfStatement *prev = ifStmt;

    //If an else if and if go until no more
    if(tok.isElif()) {
        while (tok.isElif()) {
            rightHandSideExpr = test();
            tok = tokenizer.getToken();
            //check if there is a colon
            if (!tok.isColon())
                die("Parser::if_stmt", "Expected a colon, instead got", tok);
            stmts = suite();
            //using if statement because else and if would be evaluated and function the same way
            auto *elif = new IfStatement(rightHandSideExpr, stmts);
            prev->setElif(elif);
            prev = elif;
            tok = tokenizer.getToken();
        }
    }
    //else
    if(tok.isElse()){
        tok = tokenizer.getToken();
        if (!tok.isColon())
            die("Parser::if_stmt", "Expected a colon, instead got", tok);
        stmts = suite();
        auto* elseStmt = new IfStatement(nullptr, stmts);
        prev->setElif(elseStmt);
    } else
        tokenizer.ungetToken();

    return ifStmt;
}

Statements *Parser::suite() {
    //suite: NEWLINE INDENT stmt+ DEDENT

    Token tok = tokenizer.getToken();
    //check there is a newline
    if(!tok.eol())
        die("Parser::suite", "Expected newline token, instead got", tok);
    while(tok.eol())
        tok = tokenizer.getToken();

    //check if there is an indent/tabs
    if(!tok.isIndent())
        die("Parser::suite", "Expected indent, instead got", tok);

    auto* stmts = new Statements(tokenizer.getIndent());
    stmts->addStatement(stmt());

    //While there is a statement of name
    tok = tokenizer.getToken();
    while(tok.isStatement() || tok.isName()){
        tokenizer.ungetToken();
        stmts->addStatement(stmt());
        tok = tokenizer.getToken();
    }

    //skip redundant lines
    while(tok.eol())
        tok = tokenizer.getToken();

    if(!tok.isDedent())
        die("Parser::suite", "Expected dedent, instead got", tok);
    return stmts;
}

ForStatement *Parser::forStatement(){
    //for_stmt: ’for’ ID ’in’ ’range’ ’(’ testlist ’)’ ’:’ suite

    Token tok = tokenizer.getToken();
    //if the keyword is for
    if(!tok.isForLoop())
        die("Parser::forStatement", "Expected keyword for, instead got", tok);

    tok = tokenizer.getToken();

    //if the keyword is a string
    if(!tok.isName())
        die("Parser::forStatement", "Expected variable name, instead got", tok);
    std::string var = tok.getName();

    tok = tokenizer.getToken();

    //if the keyword is in
    if(!tok.isIn())
        die("Parser::forStatement", "Expected keyword in, instead got", tok);

    tok = tokenizer.getToken();

    //if the keyword is range
    if(!tok.isRange())
        die("Parser::forStatement", "Expected keyword range, instead got", tok);
    Token rangeToken = tok;

    tok = tokenizer.getToken();

    //check if there is a (
    if(!tok.isOpenParen())
        die("Parser::forStatement", "Expected open parenthesis, instead got", tok);

    //this vector will serve to get a single, double or triple numeric range
    std::vector<ExprNode*> rangeNumber = testlist();
    for(ExprNode *expr : rangeNumber){
        Token temp = expr->token();
        if(!temp.isNumber())
            die("Parser::forStatement", "Expected numeric values as range, instead got", temp);
        else if (temp.getNumber() < 0)
            die("Parser::forStatement", "Expected non-negative values, instead got", temp);

    }
    Range *range = nullptr;

    //if only 1, 2, or 3 arguments passed
    if(rangeNumber.size() == 1 || rangeNumber.size() == 2 || rangeNumber.size() == 3) {
        if (rangeNumber.size() == 1)
            range = new Range(rangeNumber[0]->token().getNumber());

            //if only two range argument
        else if (rangeNumber.size() == 2)
            range = new Range(rangeNumber[0]->token().getNumber(), rangeNumber[1]->token().getNumber());

            //if only three range argument
        else if (rangeNumber.size() == 3)
            range = new Range(rangeNumber[0]->token().getNumber(), rangeNumber[1]->token().getNumber(),
                              rangeNumber[2]->token().getNumber());
        else
            exit(98);
    }
    //if zero or over 3 arguments are passed
    else
        die("Parser::forStatement", "Invalid number of range arguments", rangeToken);

    tok = tokenizer.getToken();

    //check if there is a )
    if(!tok.isCloseParen())
        die("Parser::forStatement", "Expected closed parenthesis, instead got", tok);

    tok = tokenizer.getToken();

    //check to make sure there is a :
    if(!tok.isColon())
        die("Parser::forStatement", "Expected a colon, instead got", tok);

    Statements* stmts = suite();
    auto* forStmt = new ForStatement(var, range, stmts);
    return forStmt;
}

std::vector<ExprNode*> Parser::testlist(){
    //testlist: test {’,’ test}*
    std::vector<ExprNode*> list;
    ExprNode* testList = test();
    list.push_back(testList);
    Token tok = tokenizer.getToken();
    while (tok.isComma()){
        list.push_back(test());
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return list;
}

ExprNode *Parser::test(){
    //test: or_test
    return orTest();
}

ExprNode *Parser::orTest(){
    //or_test: and_test {’or’ and_test}*
    ExprNode* expr = andTest();
    Token tok = tokenizer.getToken();
    //check if there is an or keyword, same as arith_expr
    while(tok.isOr()){
        auto* inExpr = new InfixExprNode(tok);
        inExpr->left() = expr;
        inExpr->right() = andTest();
        expr = inExpr;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return expr;
}
ExprNode *Parser::andTest() {
    //and_test: not_test {’and’ not_test}*
    ExprNode* expr = notTest();
    Token tok = tokenizer.getToken();
    //check if there is an and keyword, same as arith_expr
    while(tok.isAnd()){
        auto* inExpr = new InfixExprNode(tok);
        inExpr->left() = expr;
        inExpr->right() = notTest();
        expr = inExpr;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return expr;
}

ExprNode *Parser::notTest() {
    //not_test: ’not’ not_test | comparison
    Token tok = tokenizer.getToken();
    if(tok.isNot()){
        auto* inExpr = new InfixExprNode(tok);
        inExpr->right() = notTest();
        return inExpr;
    }
    else {
        tokenizer.ungetToken();
        return comparison();
    }
}

ExprNode *Parser::comparison(){
    //comparison: arith_expr {comp_op arith_expr}*

    ExprNode* left = arith_expr();

    //Grab a token and see if it's a Relational Operator
    Token tok = tokenizer.getToken();
    while(tok.isRelationalOperator()){
        tokenizer.ungetToken();
        auto *p = new InfixExprNode(comp_op());
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
        auto *p = new InfixExprNode(tok);
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
        auto *p = new InfixExprNode(tok);
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
        auto* node = new InfixExprNode(tok);

        //need two tokens
        Token t;
        t.setNumber(0);
        node->left() = new Number(t);
        ExprNode* fac = factor();

        //check data types
        if(typeid(fac) == typeid(String))
            die("Parser::factor", "Invalid use of negative sign on string", tok);
        node->right() = fac;
        return node;
    } else{
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
        ExprNode* t = test();
        tok = tokenizer.getToken();
        if (!tok.isCloseParen())
            die("Parser::atom", "Expected close-parenthesis, instead got", tok);
        return t;
    }
    else //Will not reach this statement!
        return nullptr;
}
