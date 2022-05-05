#pragma once
#include <string>
#include <vector>
#include "Atoms.h"

class SymbolTable {
public:
	struct TableRecord {
	public:
		TableRecord(const std::string name) : _name{ name } {};
		std::string _name;
	};
	const TableRecord& operator [](const int index) const;
	std::shared_ptr<MemoryOperand> add(const std::string& name);
	std::shared_ptr<MemoryOperand> alloc();
	friend std::ostream& operator<<(std::ostream& stream, const SymbolTable& sybmolTable);
protected:
	std::vector<TableRecord> _records;
};