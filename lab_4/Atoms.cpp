#include <sstream>
#include "SymbolTable.h"
#include "Atoms.h"

std::string LabelOperand::toString() const {
	std::ostringstream iss;
	iss << "l" << _LabelID;
	return iss.str();
}
std::string MemoryOperand::toString() const {
	std::ostringstream iss;
	if ((*_symbolTable)[_index]._name.size() == 0) {
		iss << "tmp" << _index;
	}
	else {
		iss << (*_symbolTable)[_index]._name;
	}
	return iss.str();
}

std::string NumberOperand::toString() const {
	std::ostringstream iss;
	iss << "'" << _value << "'";
	return iss.str();
}

std::string UnaryOpAtom::toString() const {
	std::ostringstream oss;
	oss << "(" << _name << ',' << _operand->toString() << ",," << _result->toString() << ')';
	return oss.str();
}

std::string BinaryOpAtom::toString() const {
	std::ostringstream oss;
	oss << "(" << _name << ',' << _left->toString() << "," << _right->toString() << "," << _result->toString() << ")";
	return oss.str();
}


std::string OutAtom::toString() const {
	std::ostringstream oss;
	oss << "(OUT,,," << _value->toString() << ')';
	return oss.str();
}

std::string InAtom::toString() const {
	std::ostringstream oss;
	oss << "(IN,,," << _result->toString() << ')';
	return oss.str();
}

std::string ConditonalJumpAtom::toString() const {
	std::ostringstream oss;
	oss << "(" << _condition << ',' << _left->toString() << "," << _right->toString() << "," << _label->toString() << ")";
	return oss.str();
}

std::string LabelAtom::toString() const {
	std::ostringstream oss;
	oss << "(LBL,,," << _label->toString() << ')';
	return oss.str();
}
std::string JumpAtom::toString() const {
	std::ostringstream oss;
	oss << "(LBL,,," << _label->toString() << ')';
	return oss.str();
}