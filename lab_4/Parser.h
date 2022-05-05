#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <string>

#include "Scanner.h"
#include "Atoms.h"
#include "StringTable.h"
#include "SymbolTable.h"

class Parser {
private:
	std::vector<std::unique_ptr<Atom>> _atoms;
	StringTable _stringTable;
	SymbolTable _symbolTable;
	Scanner _scanner;
	Token _currentToken;
	int _currentLabel;
	std::shared_ptr<NumberOperand> _one;
	std::shared_ptr<NumberOperand> _zero;
	std::map<LexemType, std::string> jumps;

	void lexCheck();
	std::shared_ptr<LabelOperand> newLabel();

	std::shared_ptr<RValue> E1();
	std::shared_ptr<RValue> E1_(std::shared_ptr<RValue> p);
	std::shared_ptr<RValue> E2();
	std::shared_ptr<RValue> E3();
	std::shared_ptr<RValue> E3_(std::shared_ptr<RValue> p);
	std::shared_ptr<RValue> E4();
	std::shared_ptr<RValue> E4_(std::shared_ptr<RValue> p);
	std::shared_ptr<RValue> E5();
	std::shared_ptr<RValue> E5_(std::shared_ptr<RValue> p);
	std::shared_ptr<RValue> E6();
	std::shared_ptr<RValue> E6_(std::shared_ptr<RValue> p);
	std::shared_ptr<RValue> E7();
	std::shared_ptr<RValue> E7_(std::shared_ptr<RValue> p);
public:
	Parser(std::istream& stream);
	void printAtoms(std::ostream& stream);
	void generateAtom(std::unique_ptr<Atom> atom);
	void syntaxError(const std::string& message);
	void lexicalError(const std::string& message);

	std::shared_ptr<RValue> E();

};