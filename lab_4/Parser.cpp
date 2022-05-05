#include "Parser.h"

Parser::Parser(std::istream& stream) : _currentLabel{ 1 }, _scanner{ stream }, _currentToken{ LexemType::eof }, jumps{
	{LexemType::opeq, "EQ"},
	{LexemType::opne, "NE"},
	{LexemType::oplt, "LT"},
	{LexemType::opgt, "GT"},
	{LexemType::ople, "LE" },
}
{
	_currentToken = _scanner.getNextToken();
	_one = std::make_shared<NumberOperand>(1);
	_zero = std::make_shared<NumberOperand>(0);

}

void Parser::printAtoms(std::ostream& stream) {
	for (auto& atom : _atoms) {
		stream << atom->toString() << std::endl;
	}
}
void Parser::generateAtom(std::unique_ptr<Atom> atom) {
	_atoms.push_back(std::move(atom));
}

std::shared_ptr<LabelOperand> Parser::newLabel() {
	return std::make_shared<LabelOperand>(_currentLabel++);
}

void Parser::syntaxError(const std::string& message) {
	throw "Syntax error : " + message;
}


void Parser::lexicalError(const std::string& message) {
	throw "Syntax error : " + message;
}

void Parser::lexCheck() {
	if (_currentToken.getType() == LexemType::error) {
		lexicalError(_currentToken.getStr());
	}
}

std::shared_ptr<RValue> Parser::E() {
	return E7();
}

std::shared_ptr<RValue> Parser::E1() {
	lexCheck();
	if (_currentToken.getType() == LexemType::lpar) {
		_currentToken = _scanner.getNextToken();
		auto q = E();
		lexCheck();
		if (_currentToken.getType() != LexemType::rpar) { syntaxError("Unclosed ("); }
		_currentToken = _scanner.getNextToken();
		return q;
	}
	else if (_currentToken.getType() == LexemType::num || _currentToken.getType() == LexemType::chr) {
		auto q = std::make_shared<NumberOperand>(_currentToken.getValue());
		_currentToken = _scanner.getNextToken();
		return q;
	}
	else if (_currentToken.getType() == LexemType::opinc) {
		_currentToken = _scanner.getNextToken();
		auto q = _symbolTable.add(_currentToken.getStr());
		generateAtom(std::make_unique<BinaryOpAtom>("ADD", q, _one, q));
	}
	else if (_currentToken.getType() == LexemType::id) {
		auto r = _symbolTable.add(_currentToken.getStr());
		_currentToken = _scanner.getNextToken();
		return E1_(r);
	}

	syntaxError("Incorrect Expression");
}

std::shared_ptr<RValue> Parser::E1_(std::shared_ptr<RValue> p) {
	lexCheck();
	if (_currentToken.getType() == LexemType::opinc) {
		_currentToken = _scanner.getNextToken();
		auto r = _symbolTable.alloc();
		generateAtom(std::make_unique<UnaryOpAtom>("MOV", p, r));
		generateAtom(std::make_unique<BinaryOpAtom>("ADD", p, _one, std::dynamic_pointer_cast<MemoryOperand>(p)));
		return r;
	}
	return p;
}

std::shared_ptr<RValue> Parser::E2() {
	lexCheck();
	if (_currentToken.getType() == LexemType::opnot) {
		_currentToken = _scanner.getNextToken();
		auto q = E1();
		auto r = _symbolTable.alloc();
		generateAtom(std::make_unique<UnaryOpAtom>("NOT", q, r));
		return r;
	}

	return E1();
}

std::shared_ptr<RValue> Parser::E3() {
	lexCheck();
	auto q = E2();
	return E3_(q);
}

std::shared_ptr<RValue> Parser::E3_(std::shared_ptr<RValue> p) {
	lexCheck();
	if (_currentToken.getType() == LexemType::opmult) {
		_currentToken = _scanner.getNextToken();
		auto r = E2();
		auto s = _symbolTable.alloc();
		generateAtom(std::make_unique<BinaryOpAtom>("MULT", p, r, s));
		return E3_(s);
	}
	return p;
}

std::shared_ptr<RValue> Parser::E4() {
	lexCheck();
	auto q = E3();
	return E4_(q);
}
std::shared_ptr<RValue> Parser::E4_(std::shared_ptr<RValue> p) {
	lexCheck();
	if (_currentToken.getType() == LexemType::opplus) {
		_currentToken = _scanner.getNextToken();
		auto r = E3();
		auto s = _symbolTable.alloc();
		generateAtom(std::make_unique<BinaryOpAtom>("ADD", p, r, s));
		return E4_(s);
	}
	else if (_currentToken.getType() == LexemType::opminus) {
		_currentToken = _scanner.getNextToken();
		auto r = E3();
		auto s = _symbolTable.alloc();
		generateAtom(std::make_unique<BinaryOpAtom>("SUB", p, r, s));
		return E4_(s);
	}
	return p;
}

std::shared_ptr<RValue> Parser::E5() {
	lexCheck();
	auto q = E4();
	return E5_(q);
}
std::shared_ptr<RValue> Parser::E5_(std::shared_ptr<RValue> p) {
	lexCheck();
	if (
		_currentToken.getType() == LexemType::opeq ||
		_currentToken.getType() == LexemType::opne ||
		_currentToken.getType() == LexemType::oplt ||
		_currentToken.getType() == LexemType::opgt ||
		_currentToken.getType() == LexemType::ople
		) {
		std::string t = jumps[_currentToken.getType()];
		_currentToken = _scanner.getNextToken();
		auto r = E4();
		auto s = _symbolTable.alloc();
		auto l = newLabel();
		generateAtom(std::make_unique<UnaryOpAtom>("MOV", _one, s));
		generateAtom(std::make_unique<ConditonalJumpAtom>(t, p, r, l));
		generateAtom(std::make_unique<UnaryOpAtom>("MOV", _zero, s));
		generateAtom(std::make_unique<LabelAtom>(l));
		return s;
	}
	return p;
}

std::shared_ptr<RValue> Parser::E6() {
	lexCheck();
	auto q = E5();
	return E6_(q);
}
std::shared_ptr<RValue> Parser::E6_(std::shared_ptr<RValue> p) {
	lexCheck();
	if (_currentToken.getType() == LexemType::opand) {
		_currentToken = _scanner.getNextToken();
		auto r = E5();
		auto s = _symbolTable.alloc();
		generateAtom(std::make_unique<BinaryOpAtom>("AND", p, r, s));
		return E6_(s);
	}
	return p;
}

std::shared_ptr<RValue> Parser::E7() {
	lexCheck();
	auto q = E6();
	return E7_(q);
}

std::shared_ptr<RValue> Parser::E7_(std::shared_ptr<RValue> p) {
	lexCheck();
	if (_currentToken.getType() == LexemType::opor) {
		_currentToken = _scanner.getNextToken();
		auto r = E6();
		auto s = _symbolTable.alloc();
		generateAtom(std::make_unique<BinaryOpAtom>("OR", p, r, s));
		return E7_(s);
	}
	return p;
}
