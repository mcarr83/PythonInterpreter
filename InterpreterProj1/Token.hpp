
#ifndef EXPRINTER_TOKEN_HPP
#define EXPRINTER_TOKEN_HPP
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

    //Added for for loops { }
    bool isOpenBrackets() const  { return _symbol == '{'; }
    bool isCloseBrackets() const { return _symbol == '}'; }

    void symbol(char c) { _symbol = c; }
    void symbol(std::string s) { _longSymbol = s;}
    char symbol() { return _symbol; }

    bool isSemiColon() const { return _symbol == ';'; }
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
               isDivisionOperator();
    }

    //Added isKeyWord
    bool isName() const                   { return (_name.length() > 0 && !isKeyword()); }
    std::string getName() const                  { return _name; }
    void setName(std::string n) { _name = n; }

    bool &isWholeNumber() { return _isWholeNumber; }
    bool isWholeNumber() const { return _isWholeNumber; }
    int getWholeNumber() const { return _wholeNumber; }
    void setWholeNumber(int n) {
        _wholeNumber = n;
        isWholeNumber() = true;
    }

    //Added for relational operators
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

    //Added for keywords
    bool isForLoop() const { return _name == "for";}
    bool isPrint() const { return _name == "print";}
    bool isKeyword() const {
        return isForLoop() || isPrint();
    }


    void print() const;

private:
    std::string _name;
    bool _eof, _eol;
    bool _isWholeNumber;
    char _symbol;
    std::string _longSymbol;
    int _wholeNumber;
};

#endif //EXPRINTER_TOKEN_HPP
