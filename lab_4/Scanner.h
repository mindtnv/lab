#pragma once

#include <string>

enum class LexemType { num, chr, str, id, lpar, rpar, lbrace, rbrace, lbracket, rbracket, semicolon, comma, colon, opassign, opplus, opminus, opmult, opinc, opeq, opne, oplt, opgt, ople, opnot, opor, opand, kwint, kwchar, kwif, kwelse, kwswitch, kwcase, kwwhile, kwfor, kwreturn, kwin, kwout, eof, error };

class Token {
public:
	Token(LexemType type);
	Token(int value);
	Token(LexemType type, const std::string& str);
	Token(char c);
	Token();
	LexemType getType();
	int getValue();
	std::string getStr();

	friend std::ostream& operator<<(std::ostream& stream, Token& token);

private:
	LexemType _type;
	int _value;
	std::string _str;
};

class Scanner {
public:
	Scanner(std::istream& stream);

	Token getNextToken();
private:
	std::istream& _inputStream;
	std::string _buffer;
	int _bufferPosition;
	int _bufferLength;
	char _savedChar;
	char _symb;
	int _state;

	char readChar();
	char peekChar();
	void returnChar(char c);
	bool isDigit(char c);
};