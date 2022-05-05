#include "SymbolTable.h"
#include <iomanip>
#include <algorithm>

typedef SymbolTable::TableRecord TSrec;

const TSrec& SymbolTable::operator[] (const int index)  const {
	if (index < 0 || index >= _records.size()) {
		throw "error";
	}
	return _records[index];
}
std::shared_ptr<MemoryOperand> SymbolTable::add(const std::string& name) {
	auto is_ok = [&name](TSrec& _s) {return _s._name == name; };
	auto result = std::find_if(_records.begin(), _records.end(), is_ok);

	if (result != _records.end()) {
		return std::make_shared<MemoryOperand>(result - _records.begin(), this);
	}
	_records.push_back(TSrec(name));
	return std::make_shared<MemoryOperand>(_records.size() - 1, this);
}
std::shared_ptr<MemoryOperand> SymbolTable::alloc() {
	_records.push_back(TSrec(""));
	return std::make_shared<MemoryOperand>(_records.size() - 1, this);
}

std::ostream& operator<<(std::ostream& stream, const SymbolTable& symbolTable) {
	stream << "Symbol Table\n";
	int line = 0;
	for (auto& item : symbolTable._records) {
		stream << std::setiosflags(std::ios::left) << std::setw(3) << line++;
		if (item._name.length() == 0) {
			stream << "TMP" << (line - 1) << std::endl;
		}
		else {
			stream << item._name << std::endl;
		}
	}
	return stream;
}