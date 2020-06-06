//
// Created by Michael Carr
//
#include <iostream>
#include "SymTab.hpp"


void SymTab::setValueFor(const std::string& vName, TypeDescriptor* value) {
    symTab[vName] = value;
}

bool SymTab::isDefined(const std::string& vName) {
    return symTab.find(vName) != symTab.end();
}

TypeDescriptor* SymTab::getValue(const std::string& vName) {
    if(!isDefined(vName)) {
        exit(1);
    }
    else
        return symTab[vName];
}

void SymTab::print() {
    for(std::pair<std::string, TypeDescriptor*> v : symTab ) {
        std::cout << v.first << " = ";
        if (v.second->type() == TypeDescriptor::STRING)
            std::cout << '"';
        std::cout << *v.second;
        if (v.second->type() == TypeDescriptor::STRING)
            std::cout << '"';
        std::cout << std::endl;
    }
}