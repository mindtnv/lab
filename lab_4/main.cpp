#include <iostream>
#include <fstream>
#include "Scanner.h"
#include "parser.h"
#include "Grammar.h"

void first() {
	std::ifstream ifile("grammar.txt");
	Grammar grammar = Grammar();
	grammar.loadGrammar(ifile);
	std::cout << grammar.FIRST({ "E" });
	std::cout << grammar.FIRST({ "E'" });
	std::cout << grammar.FIRST({ "T" });
	std::cout << grammar.FIRST({ "T'" });
	std::cout << grammar.FIRST({ "F" });
	std::cout << grammar.FIRST({ ")" });
	std::cout << grammar.FIRST({ "id" });
}

void follow() {
	std::ifstream ifile("grammar.txt");
	Grammar grammar = Grammar();
	grammar.loadGrammar(ifile);
	std::cout << grammar.FOLLOW("E");
	std::cout << grammar.FOLLOW("E'");
	std::cout << grammar.FOLLOW("T");
	std::cout << grammar.FOLLOW("T'");
	std::cout << grammar.FOLLOW("F");
}

void table() {
	std::ifstream ifile("grammar.txt");
	Grammar grammar = Grammar();
	grammar.loadGrammar(ifile);
	grammar.printSATable(std::cout);
}

void parser() {
	std::ifstream ifile("program.txt");
	Parser parser{ ifile };
	parser.E();
	parser.printAtoms(std::cout);
}

int main() {
	// first();
	// follow();
	// table();
	parser();
}