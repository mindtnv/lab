#pragma once
#include <string>
#include <memory>
#include "StringTable.h"

class SymbolTable;

class Operand {
public:
	Operand() = default;
	virtual std::string toString() const = 0;
};

class RValue : public Operand {
public:
	RValue() = default;
};

class StringOperand : public Operand {
public:
	StringOperand(int index, const StringTable* stringTable) : _index{ index }, _stringTable{ stringTable } {};
	std::string toString() const;
protected:
	int _index;
	const StringTable* _stringTable;
};

class LabelOperand : public Operand {
public:
	LabelOperand(int LabelID) : _LabelID{ LabelID } {};
	std::string toString() const;
protected:
	int _LabelID;
};

class MemoryOperand : public RValue {
public:
	MemoryOperand(int index, const SymbolTable* symbolTable) : _index{ index }, _symbolTable{ symbolTable } {};
	std::string toString() const;
protected:
	int _index;
	const SymbolTable* _symbolTable;
};

class NumberOperand : public RValue {
public:
	NumberOperand(int value) : _value{ value } {};
	std::string toString() const;
protected:
	int _value;
};


class Atom {
public:
	virtual std::string toString() const = 0;
};

class UnaryOpAtom : public Atom {
protected:
	std::string _name;
	std::shared_ptr<RValue> _operand;
	std::shared_ptr<MemoryOperand> _result;

public:
	UnaryOpAtom(const std::string& name, std::shared_ptr<RValue> operand, std::shared_ptr<MemoryOperand> result) : _name{ name }, _operand{ operand }, _result{ result } {};
	std::string toString() const;
};

class BinaryOpAtom : public Atom {
protected:
	std::string _name;
	std::shared_ptr<RValue> _left;
	std::shared_ptr<RValue> _right;
	std::shared_ptr<MemoryOperand> _result;

public:
	BinaryOpAtom(const std::string& name, std::shared_ptr<RValue> left, std::shared_ptr<RValue> right, std::shared_ptr<MemoryOperand> result) : _name{ name }, _left{ left }, _right{ right }, _result{ result } {};
	std::string toString() const;
};

class OutAtom : public Atom {
public:
	OutAtom(std::shared_ptr<Operand> value) : _value{ value } {};
	std::string toString() const;
protected:
	std::shared_ptr<Operand> _value;
};

class InAtom : public Atom {
public:
	InAtom(std::shared_ptr<MemoryOperand> result) : _result{ result } {};
	std::string toString() const;
protected:
	std::shared_ptr<MemoryOperand> _result;
};

class ConditonalJumpAtom : public Atom {
public:
	ConditonalJumpAtom(const std::string& condition, std::shared_ptr<RValue> left, std::shared_ptr<RValue> right, std::shared_ptr<LabelOperand> label) : _condition{ condition }, _left{ left }, _right{ right }, _label{ label } {};
	std::string toString() const;
protected:
	std::string _condition;
	std::shared_ptr<RValue> _left;
	std::shared_ptr<RValue> _right;
	std::shared_ptr<LabelOperand> _label;
};

class LabelAtom : public Atom {
public:
	LabelAtom(std::shared_ptr<LabelOperand> label) : _label{ label } {};
	std::string toString() const;
protected:
	std::shared_ptr<LabelOperand> _label;

};

class JumpAtom : public Atom {
public:
	JumpAtom(std::shared_ptr<LabelOperand> label) : _label{ label } {};
	std::string toString() const;
protected:
	std::shared_ptr<LabelOperand> _label;
};



