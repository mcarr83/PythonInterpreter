//
// Created by Michael Carr
//
#include "TypeDescriptor.hpp"

//Operators

std::ostream& operator<<(std::ostream& out, TypeDescriptor type)
{
    switch(type.type()){

        case TypeDescriptor::NUMBER:
            out << type._n;
            return out;

        case TypeDescriptor::STRING:
            out << type._s;
            return out;

        default:
            std::cout << "Unknown type, exiting..." << std::endl;
            exit(3);
    }
}

TypeDescriptor* operator +(TypeDescriptor left, TypeDescriptor right){
    switch(left.type()){
        case TypeDescriptor::NUMBER:
            return new NumDescriptor(left._n + right._n);

        case TypeDescriptor::STRING: {
            std::string leftSide(left._s);
            std::string rightSide(right._s);
            return new StringDescriptor(leftSide + rightSide);}

        default:
            std::cout << "Unknown type, exiting..." << std::endl;
            exit(4);
    }
}

TypeDescriptor* operator -(TypeDescriptor left, TypeDescriptor right){
    switch(left.type()){

        case TypeDescriptor::NUMBER:
            return new NumDescriptor(left._n - right._n);

        case TypeDescriptor::STRING:
            std::cout << "Attempted subtraction on string" << std::endl;
            std::cout << '"' << left._s << "\" and \"" << right._s << '"' <<std::endl;
            exit(5);

        default:
            std::cout << "Unknown type, exiting..." << std::endl;
            exit(6);
    }
}

TypeDescriptor* operator *(TypeDescriptor left, TypeDescriptor right){
    switch(left.type()){

        case TypeDescriptor::NUMBER:
            return new NumDescriptor(left._n * right._n);

        case TypeDescriptor::STRING:
            std::cout << "Attempted multiplication on a string" << std::endl;
            std::cout << '"' << left._s << "\" and \"" << right._s << '"' <<std::endl;
            exit(7);

        default:
            std::cout << "Unknown type, exiting..." << std::endl;
            exit(8);
    }
}
TypeDescriptor* operator /(TypeDescriptor left, TypeDescriptor right){
    switch(left.type()){

        case TypeDescriptor::NUMBER:
            return new NumDescriptor(left._n / right._n);

        case TypeDescriptor::STRING:
            std::cout << "Attempted division on a string" << std::endl;
            std::cout << '"' << left._s << "\" and \"" << right._s << '"' <<std::endl;
            exit(9);

        default:
            std::cout << "Unknown type, exiting..." << std::endl;
            exit(10);
    }
}

TypeDescriptor* operator %(TypeDescriptor left, TypeDescriptor right){
    switch(left.type()){

        case TypeDescriptor::NUMBER:
            return new NumDescriptor((int)left._n % (int)right._n);

        case TypeDescriptor::STRING:
            std::cout << "Attempted mod on a string" << std::endl;
            std::cout << '"' << left._s << "\" and \"" << right._s << '"' <<std::endl;
            exit(11);

        default:
            std::cout << "Unknown type, exiting..." << std::endl;
            exit(12);
    }
}
/*
TypeDescriptor* operator = (TypeDescriptor left, TypeDescriptor right){
    switch(left.type()){
        case TypeDescriptor::NUMBER:{
            if (left._n == right._n)
                return new NumDescriptor(1.0);
            else
                return new NumDescriptor(0.0);
        }
        case TypeDescriptor::STRING:{
            std::string leftSide(left._s);
            std::string rightSide(right._s);
            return new NumDescriptor(leftSide = rightSide);}

        default:
            std::cout << "Unknown type for '=', exiting ..." << std::endl; //Should never get here.
            exit(13);
    }
}
*/

TypeDescriptor* operator ==(TypeDescriptor left, TypeDescriptor right){
    switch(left.type()){

        case TypeDescriptor::NUMBER:{

            if (left._n == right._n)
                return new NumDescriptor(1.0);
            else
                return new NumDescriptor(0.0);

        }

        case TypeDescriptor::STRING:{
            std::string leftSide(left._s);
            std::string rightSide(right._s);
            return new NumDescriptor(leftSide == rightSide);}

        default:
            std::cout << "Unknown type, exiting..." << std::endl;
            exit(14);
    }
}

TypeDescriptor* operator !=(TypeDescriptor left, TypeDescriptor right){
    switch(left.type()){

        case TypeDescriptor::NUMBER:{
            if (left._n == right._n)
                return new NumDescriptor(1.0);
            else
                return new NumDescriptor(0.0);
        }

        case TypeDescriptor::STRING:{
            std::string leftSide(left._s);
            std::string rightSide(right._s);
            return new NumDescriptor(leftSide != rightSide);}

        default:
            std::cout << "Unknown type, exiting..." << std::endl;
            exit(15);
    }
}

TypeDescriptor* operator >(TypeDescriptor left, TypeDescriptor right){
    switch(left.type()){
        case TypeDescriptor::NUMBER:{
            if (left._n == right._n)
                return new NumDescriptor(1.0);
            else
                return new NumDescriptor(0.0);
        }


        case TypeDescriptor::STRING:{
            std::string leftSide(left._s);
            std::string rightSide(right._s);
            return new NumDescriptor(leftSide > rightSide);}

        default:
            std::cout << "Unknown type, exiting..." << std::endl;
            exit(16);
    }
}

TypeDescriptor* operator <(TypeDescriptor left, TypeDescriptor right){
    switch(left.type()){
        case TypeDescriptor::NUMBER: {
            if (left._n == right._n)
                return new NumDescriptor(1.0);
            else
                return new NumDescriptor(0.0);
        }

        case TypeDescriptor::STRING:{
            std::string leftSide(left._s);
            std::string rightSide(right._s);
            return new NumDescriptor(leftSide < rightSide);}

        default:
            std::cout << "Unknown type, exiting..." << std::endl;
            exit(17);
    }
}

TypeDescriptor* operator >=(TypeDescriptor left, TypeDescriptor right){
    switch(left.type()){
        case TypeDescriptor::NUMBER:{
            if (left._n == right._n)
                return new NumDescriptor(1.0);
            else
                return new NumDescriptor(0.0);
        }

        case TypeDescriptor::STRING:{
            std::string leftSide(left._s);
            std::string rightSide(right._s);
            return new NumDescriptor(leftSide >= rightSide);}

        default:
            std::cout << "Unknown type, exiting..." << std::endl;
            exit(18);
    }
}

TypeDescriptor* operator <=(TypeDescriptor left, TypeDescriptor right){
    switch(left.type()){
        case TypeDescriptor::NUMBER:{
            if (left._n == right._n)
                return new NumDescriptor(1.0);
            else
                return new NumDescriptor(0.0);
        }

        case TypeDescriptor::STRING:{
            std::string leftSide(left._s);
            std::string rightSide(right._s);
            return new NumDescriptor(leftSide <= rightSide);}

        default:
            std::cout << "Unknown type, exiting..." << std::endl;
            exit(19);
    }
}
/*           _____  _
 *        .     ((___)
 *        .....-,((____)
 *        ...---((_____)
 *        .......((____)
 *        ---)--------/
 *        ---|   /
 *           |  |
 *           (__)
 */

