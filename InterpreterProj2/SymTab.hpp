//
// Created by Michael Carr
//

#ifndef STATEMENTSINTERPRETER_SYMTAB_HPP
#define STATEMENTSINTERPRETER_SYMTAB_HPP

#include <string>
#include <map>
#include "TypeDescriptor.hpp"

// This is a flat and integer-based symbol table. It allows for variables to be
// initialized, determines if a give variable has been defined or not, and if
// a variable has been defined, it returns its value.

class SymTab {
public:
    void setValueFor(const std::string& vName, TypeDescriptor* value);
    bool isDefined(const std::string& vName);
    TypeDescriptor* getValue(const std::string& vName);
    void print();

private:
    std::map<std::string, TypeDescriptor*> symTab;
};
#endif //STATEMENTSINTERPRETER_SYMTAB_HPP
