.SUFFIXES: .o .cpp .x

CFLAGS = -ggdb -std=c++17
objects = TypeDescriptor.o Token.o Tokenizer.o Parser.o Expr.o SymTab.o Statements.o main.o

statement.x: $(objects)
	g++ $(CFLAGS) -o statement.x $(objects)

.cpp.o:
	g++ $(CFLAGS) -c $< -o $@

TypeDescriptor.o: TypeDescriptor.hpp
Token.o:  Token.cpp Token.hpp
Tokenizer.o: Tokenizer.cpp Tokenizer.hpp
Expr.o: Expr.cpp Expr.hpp Token.hpp SymTab.hpp
SymTab.o: SymTab.cpp SymTab.hpp TypeDescriptor.hpp
Parser.o: Parser.cpp Token.hpp Parser.hpp Tokenizer.hpp SymTab.hpp Expr.hpp Statements.hpp
Statements.o: Statements.cpp Statements.hpp Expr.hpp Token.hpp SymTab.hpp
main.o: main.cpp Token.hpp Tokenizer.hpp Parser.hpp SymTab.hpp Expr.hpp Statements.hpp

clean:
	rm -fr *.o *~ *.x
