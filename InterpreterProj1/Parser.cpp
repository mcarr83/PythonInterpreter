
#include<vector>
#include<iostream>

#include "Token.hpp"
#include "Parser.hpp"
#include "Statements.hpp"

// Parser functions
// Parser functions

void Parser::die(std::string where, std::string message, Token &token) {
    std::cout << where << " " << message << std::endl;
    token.print();
    std::cout << std::endl;
    std::cout << "\nThe following is a list of tokens that have been identified up to this point.\n";
    tokenizer.printProcessedTokens();
    exit(1);
}

Statements *Parser::statements() {
    // This function is called when we KNOW that we are about to parse
    // a series of assignment statements.

    // This function parses the grammar rules:

    // <statement> -> <statement> { <statement> }
    // <statement> -> <for-statement> | <assign-statement> | <print-statement>
    // <statement> -> Epsilon

    bool scope;
    Token tok = tokenizer.getToken();
    //if token is an open bracket
    if(tok.isOpenBrackets()){
        scope = true;
        //get another token
        tok = tokenizer.getToken();
        //if its the end of line, unget it
        if(!tok.eol())
            tokenizer.ungetToken();
    }
    else {
        scope = false;
        tokenizer.ungetToken();
    }

    //set pointer instance of statements
    Statements *stmts = new Statements();
    //get a token
    tok = tokenizer.getToken();
    //while the token is a name or keyword
    while (tok.isName() || tok.isKeyword()) {
        //check if its word is for
        if(tok.isForLoop()){
            ForLoop *forStmt = forLoop();
            stmts->addStatement(forStmt);
        }
        //or print
        else if(tok.isPrint()){
            PrintStatement *printStmt = printStatement();
            stmts->addStatement(printStmt);
        }
        else {
            //if its not a word or keyword unget
            tokenizer.ungetToken();
            AssignmentStatement *assignStmt = assignStatement();
            stmts->addStatement(assignStmt);
        }

        //get a token
        tok = tokenizer.getToken();
        //if its a close bracket and in scope
        if(tok.isCloseBrackets() && scope)
            //get out
            break;
        //if its the eol of a line
        /*if(!tok.eol())
            //die
            die("Parser::statements", "Expected end of line, instead got", tok);
        //get another token
        tok = tokenizer.getToken();*/

        //fixed for keep on grabbing a token if only a new line and throw it away
        while(tok.eol()){
            tok = tokenizer.getToken();
        }
    }
    //if the token is not in scope unget
    if(!scope)
        tokenizer.ungetToken();
    //return the statement
    return stmts;
}

AssignmentStatement *Parser::assignStatement() {
    // Parses the following grammar rule
    //
    // <assign-stmtement> -> <id> = <arith_expr>

    Token varName = tokenizer.getToken();
    if (!varName.isName())
        die("Parser::assignStatement", "Expected a name token, instead got", varName);

    Token assignOp = tokenizer.getToken();
    if (!assignOp.isAssignmentOperator())
        die("Parser::assignStatement", "Expected an equal sign, instead got", assignOp);

    //Was...
    //ExprNode *rightHandSideExpr = arith_expr();

    //Now...
    ExprNode *rightHandSideExpr = rel_expr();
    /*Token tok = tokenizer.getToken();
    if (!tok.isSemiColon())
        die("Parser::assignStatement", "Expected a semicolon, instead got", tok);*/

    return new AssignmentStatement(varName.getName(), rightHandSideExpr);
}

// New function
ExprNode *Parser::rel_expr(){
    // <rel-expr> -> <rel-term> {(==, !=) <rel-term>}

    //get the left node
    ExprNode *left = rel_term();
    //get a token
    Token tok = tokenizer.getToken();
    //while the token is == or !=
    while (tok.isEqualEqual() || tok.isNotEqual()) {
        //evaluate the left and right side of the relational expression
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = rel_term();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

//New function
ExprNode *Parser::rel_term(){
    // <rel-term> -> <rel-primary> {(>, >=, <, <=) <rel-primary>}
    // <rel-primary> -> <arith-expr>

    //get the left node
    ExprNode *left = rel_primary();
    Token tok = tokenizer.getToken();
    //get a token >= <= > <
    while (tok.isGreaterThan() || tok.isGreaterThanOrEqual() || tok.isLessThan() || tok.isLessThanOrEqual()) {
        //evaluate
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = rel_primary();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::rel_primary() {
    //<rel-primary> -> <arith-expr>
    ExprNode *p = arith_expr();
    return p;
}

ExprNode *Parser::arith_expr() {
    // This function parses the grammar rules:

    // <arith_expr> -> <term> { <add_op> <arith_term> }
    // <add_op> -> + | -

    // However, it makes the <add_op> left associative.

    ExprNode *left = arith_term();
    Token tok = tokenizer.getToken();
    while (tok.isAdditionOperator() || tok.isSubtractionOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = arith_term();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}


ExprNode *Parser::arith_term() {
    // This function parses the grammar rules:

    // <arith_term> -> <primary> { <mult_op> <arith_primary> }
    // <mult_op> -> * | / | %

    // However, the implementation makes the <mult-op> left associate.
    ExprNode *left = arith_primary();
    Token tok = tokenizer.getToken();

    while (tok.isMultiplicationOperator() || tok.isDivisionOperator() || tok.isModuloOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = arith_primary();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::arith_primary() {
    // This function parses the grammar rules:

    // <arith_primary> -> [0-9]+
    // <arith_primary> -> [_a-zA-Z]+
    // <arith_primary> -> (<arith_expr>)

    Token tok = tokenizer.getToken();

    if (tok.isWholeNumber() )
        return new WholeNumber(tok);
    else if( tok.isName() )
        return new Variable(tok);
    else if (tok.isOpenParen()) {
        ExprNode *p = arith_expr();
        Token token = tokenizer.getToken();
        if (!token.isCloseParen())
            die("Parser::arith_primary", "Expected close-parenthesis, instead got", token);
        return p;
    }
    die("Parser::arith_primary", "Unexpected token", tok);

    return nullptr;  // Will not reach this statement!
}

//New function
ForLoop *Parser::forLoop(){
    // <for-statement> -> for ( <assign-statement> ; <rel-expr> ; <assign-statement> ) { <statements> }

    //Get a token
    Token tok = tokenizer.getToken();
    //if the token is an open parenthesis
    if(!tok.isOpenParen())
        //stop
        die("Parser::forLoop", "Expected an open parenthesis, instead got", tok);

    // Intialize a pointer variable of assignment statments
    AssignmentStatement *varIntialize = assignStatement();

    //get a token
    tok = tokenizer.getToken();
    //if its a semicolon stop
    if(!tok.isSemiColon())
        die("Parser::forLoop", "Expected a semicolonm, instead got", tok);

    //Initialize a pointer of relation expression
    ExprNode *relExpr = rel_expr();

    //get a token
    tok = tokenizer.getToken();

    //if its a semicolon stop
    if(!tok.isSemiColon())
        die("Parser::forLoop", "Expected a semicolonm, instead got", tok);

    //Initialize a variable update point of assignment statements
    AssignmentStatement *varUpdate = assignStatement();

    //get a token
    tok = tokenizer.getToken();

    //check if is a close parenthesis
    if(!tok.isCloseParen())
        die("Parser::forLoop", "Expected a close parenthesis, instead got", tok);

    //get another token
    tok = tokenizer.getToken();

    //check if its an open bracket
    if(!tok.isOpenBrackets())
        die("Parser::forLoop", "Expected a open bracket, instead got", tok);

    //unget the token above semicolon check
    tokenizer.ungetToken();
    Statements *stmts = statements();

    //allocate memory for ForLoop and return all values to ForLoop in
    //Statements
    return new ForLoop(varIntialize, relExpr, varUpdate, stmts);
}

//New function
//Simple function to see if the print keyword is called
//and if it is allocate memory and return name to PrintStatement
//in Statements
PrintStatement *Parser::printStatement(){

    // <print-statement> -> print <id>
    Token tok = tokenizer.getToken();
    if(!tok.isName())
        die("Parser::printStatement", "Unexpected token", tok);

    std::string varName = tok.getName();

    return new PrintStatement(varName);
}
