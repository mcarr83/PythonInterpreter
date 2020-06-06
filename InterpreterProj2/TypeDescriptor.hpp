//
// Created by Michael Carr
//

#ifndef STATEMENTSINTERPRETER_TYPEDESCRIPTOR_HPP
#define STATEMENTSINTERPRETER_TYPEDESCRIPTOR_HPP


#include <string>
#include <iostream>
#include <utility>
#include <vector>

struct TypeDescriptor {
    enum types {NUMBER, STRING};
    explicit TypeDescriptor(types type, double num = 0, std::string str = ""): _type{type}, _n{num}, _s{std::move(str)} {}
    types type() {return _type;}
    double _n;
    std::string _s;
    //trivial destructor
    virtual ~TypeDescriptor() = default;

private:
    types _type;
};

struct NumDescriptor: public TypeDescriptor {
    explicit NumDescriptor(double value):  TypeDescriptor(NUMBER, value, ""), _n{value} {}
    double _n;

private:
    types _type = NUMBER;
};

struct StringDescriptor: public TypeDescriptor {
    explicit StringDescriptor(const std::string& value):  TypeDescriptor(STRING, 0, value), _s{value} {}
    std::string _s;

private:
    types _type = STRING;
};

//Operator Prototypes

std::ostream& operator << (std::ostream& out, TypeDescriptor type);
TypeDescriptor *operator + (TypeDescriptor left, TypeDescriptor right);
TypeDescriptor *operator - (TypeDescriptor left, TypeDescriptor right);
TypeDescriptor *operator * (TypeDescriptor left, TypeDescriptor right);
TypeDescriptor *operator / (TypeDescriptor left, TypeDescriptor right);
TypeDescriptor *operator % (TypeDescriptor left, TypeDescriptor right);
TypeDescriptor *operator > (TypeDescriptor left, TypeDescriptor right);
TypeDescriptor *operator < (TypeDescriptor left, TypeDescriptor right);
TypeDescriptor *operator >= (TypeDescriptor left, TypeDescriptor right);
TypeDescriptor *operator <= (TypeDescriptor left, TypeDescriptor right);
//TypeDescriptor *operator = (TypeDescriptor left, TypeDescriptor right);
TypeDescriptor *operator == (TypeDescriptor left, TypeDescriptor right);
TypeDescriptor *operator != (TypeDescriptor left, TypeDescriptor right);



#endif //STATEMENTSINTERPRETER_TYPEDESCRIPTOR_HPP
