//
// Created by Michael Carr
//

#ifndef STATEMENTSINTERPRETER_TOKEN_HPP
#define STATEMENTSINTERPRETER_TOKEN_HPP

#include<string>

class Token {

public:
    Token();

    bool &eof()  { return _eof; }
    bool &eol()  { return _eol; }

    bool eof() const { return _eof; }
    bool eol() const  { return _eol; }

    bool isOpenParen() const  { return _symbol == '('; }
    bool isCloseParen() const { return _symbol == ')'; }

    //Added for for loops, not used but will stay in
    bool isOpenBrackets() const  { return _symbol == '{'; }
    bool isCloseBrackets() const { return _symbol == '}'; }

    void symbol(char c) { _symbol = c; }
    void symbol(std::string s) { _longSymbol = s;}
    char symbol() { return _symbol; }

    bool isAssignmentOperator() const              { return _symbol == '='; }
    bool isMultiplicationOperator() const { return _symbol == '*'; }
    bool isAdditionOperator() const       { return _symbol == '+'; }
    bool isSubtractionOperator() const    { return _symbol == '-'; }
    bool isModuloOperator() const         { return _symbol == '%'; }
    bool isDivisionOperator() const       { return _symbol == '/'; }

    bool isArithmeticOperator() const {
        return isMultiplicationOperator() ||
               isAdditionOperator() ||
               isSubtractionOperator() ||
               isModuloOperator() ||
               isDivisionOperator() ||
               isIntDivisionOperator();
    }

    //Project 1
    bool isName() const                   { return ((_name.length() > 0) && (!isKeyword())); }
    std::string getName() const                  { return _name; }
    void setName(std::string n) { _name = std::move(n); }

    //All renamed to take more than one type of number
    bool &isNumber() { return _isNumber; }
    bool isNumber() const { return _isNumber; }
    double getNumber() const { return _number; }
    void setNumber(double n) {
        _number = n;
        _isNumber = true;
    }

    //Added for Project 2
    bool isString() const {return _isString;}
    std::string getString() const { return _string;}
    void setString(std::string string) {
        _string = std::move(string);
        _isString = true;
    }

    bool isEqualEqual() const { return _longSymbol == "=="; }
    bool isNotEqual() const {return _longSymbol == "!="; }
    bool isGreaterThan() const {return _symbol == '>'; }
    bool isLessThan() const {return _symbol == '<'; }
    bool isGreaterThanOrEqual() const {return _longSymbol == ">="; }
    bool isLessThanOrEqual() const {return _longSymbol == "<="; }
    bool isRelationalOperator() const {
        return isEqualEqual() ||
               isNotEqual() ||
               isGreaterThan() ||
               isLessThan() ||
               isGreaterThanOrEqual() ||
               isLessThanOrEqual();
    }

    //Project 2
    bool isIntDivisionOperator() const    { return _longSymbol == "//"; }
    bool isComma() const { return _symbol == ','; }
    bool isForLoop() const { return _name == "for";}
    bool isPrint() const { return _name == "print";}

    void print() const;

    //Project 3
    bool isRange() const { return _name == "range";}
    bool isColon() const { return _symbol == ':'; }
    bool isIf() const { return _name == "if";}
    bool isElif() const { return _name == "elif";}
    bool isElse() const { return _name == "else";}
    bool isIn() const { return _name == "in";}
    bool isOr() const { return _name == "or";}
    bool isAnd() const { return _name == "and";}
    bool isNot() const { return _name == "not";}
    bool isKeyword() const {
        return isStatement() || isRange() || isElif() || isElse() || isIn() || isOr() || isAnd() || isNot();
    }
    bool isStatement() const {
        return isForLoop() || isIf() || isPrint();
    }

    void setIndent(int n) { _numIndents = n; _isIndent = true;}
    bool isIndent() const { return _isIndent; }
    bool isDedent() const { return _isDedent; }
    bool &isIndent()  { return _isIndent; }
    bool &isDedent()  { return _isDedent; }



private:
    std::string _name;
    bool _eof, _eol;
    bool _isNumber;
    bool _isString;
    char _symbol;
    std::string _longSymbol;
    std::string _string;
    double _number;
    int _numIndents{};
    bool _isIndent;
    bool _isDedent;
};


#endif //STATEMENTSINTERPRETER_TOKEN_HPP
