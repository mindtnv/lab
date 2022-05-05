#pragma once
#include <string>
#include <vector>

class StringTable {
public:
	const std::string& operator [](const int index) const;
	int add(const std::string name);

	//friend std::ostream& operator<<(std::ostream& stream, const StringTable strTable);
private:
	std::vector<std::string> _strings;
};
