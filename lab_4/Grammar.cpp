#include <algorithm>
#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <sstream>
#include <map>
#include <utility>
#include <iomanip>
#include <stack>
#include "Grammar.h"


Grammar::Grammar() {};
Grammar::~Grammar() {};


bool Grammar::loadGrammar(std::istream& inputStream) {
	if (!inputStream) {
		throw std::invalid_argument("Empty grammar");
	}

	std::string line, item, leftPart, currentRightSymbol;
	bool isStartNonTerminalNotRead = true;

	while (std::getline(inputStream, line)) {
		std::istringstream ss(line);
		std::vector<std::string> rigthPart;
		ss >> leftPart >> currentRightSymbol;

		if (isStartNonTerminalNotRead) {
			isStartNonTerminalNotRead = false;
			startNonTerminal = leftPart;
		}

		while (ss >> currentRightSymbol) {
			rigthPart.push_back(currentRightSymbol);
			if ((currentRightSymbol[0] < 'A' || currentRightSymbol[0] > 'Z') && currentRightSymbol[0] != 'e') {
				terminals.insert(currentRightSymbol);
			}
		}

		grammar[leftPart].push_back(rigthPart);
	}

	initFIRSTWithTerminalsAndEpsilon();
	initFIRSTWithNonTerminals();
	calculateFOLLOW();

	terminals.insert("$");
	return (buildSAtable());
}


std::ostream& operator << (std::ostream& stream, const Grammar& g) {
	for (auto& i : g.grammar) {
		for (auto& j : i.second) {
			stream << i.first << " -> ";
			for (auto& k : j) {
				stream << k << " ";
			}
			stream << '\n';
		}
	}
	return stream;
}

void Grammar::initFIRSTWithTerminalsAndEpsilon() {
	for (auto& i : grammar) {
		for (auto& j : i.second) {
			for (auto& k : j) {
				if (k[0] == 'e') {
					std::set<std::string> temp;
					temp.insert("e");
					FIRSTForG[i.first] = temp;
				}
				else if (k[0] < 'A' || k[0] > 'Z') {
					std::set<std::string> temp;
					temp.insert(k);
					FIRSTForG[k] = temp;
				}
			}
		}
	}
}

void Grammar::printFIRSTForG(std::ostream& stream) {
	auto poutside = FIRSTForG.begin();

	while (poutside != FIRSTForG.end()) {
		stream << poutside->first << " = " << poutside->second;
		++poutside;
	}


}

std::set<std::string> Grammar::FIRST(const std::vector<std::string>& str) {
	std::set <std::string> result;

	if (str[0] == "e") {
		result.insert("e");
		return result;
	}

	if (str.size() == 1) {
		return FIRSTForG[str[0]];
	}

	bool noEpsilon = false;
	for (auto& symbol : str) {
		std::vector<std::string> t;
		t.push_back(symbol);
		auto currentResult = FIRST(t);
		result.insert(currentResult.begin(), currentResult.end());

		if (currentResult.find("e") == currentResult.end()) {
			noEpsilon = true;
			break;
		}

	}
	if (noEpsilon) {
		result.erase("e");
	}
	return result;
}

int Grammar::FIRSTForGLength() {
	int length = 0;
	for (auto& i : FIRSTForG) {
		length += i.second.size();
	}
	return length;
}

void Grammar::initFIRSTWithNonTerminals() {
	int currentLength = FIRSTForGLength();
	int newLength = currentLength;
	do {
		currentLength = newLength;
		for (auto& i : grammar) {
			for (auto& j : i.second) {
				auto currSet = FIRST(j);
				FIRSTForG[i.first].insert(currSet.begin(), currSet.end());
			}
		}
		newLength = FIRSTForGLength();
	} while (newLength != currentLength);
}

void Grammar::initFOLLOW() {
	FOLLOWForG[startNonTerminal].insert("$");
}

void Grammar::printFOLLOWForG(std::ostream& stream) {
	auto poutside = FOLLOWForG.begin();

	while (poutside != FOLLOWForG.end()) {
		stream << poutside->first << " = " << poutside->second;
		++poutside;
	}
}


int Grammar::FOLLOWForGLength() {
	int length = 0;
	for (auto& i : FOLLOWForG) {
		length += i.second.size();
	}
	return length;
}

std::vector<std::string> Grammar::getVectorLastPart(std::vector<std::string>::iterator start, std::vector<std::string>::iterator end) {
	std::vector<std::string> result;
	while (start != end) {
		result.push_back(*start);
		++start;
	}
	return result;
}

void Grammar::calculateFOLLOW() {
	initFOLLOW();

	bool isEpsilonFind = false;
	int currentLength = FOLLOWForGLength();
	int newLength = currentLength;

	do {
		currentLength = newLength;
		for (auto& pair : grammar) {
			for (auto& v : pair.second) {
				auto it = v.begin();
				while (it != v.end()) {
					if ((*it)[0] >= 'A' && (*it)[0] <= 'Z') {
						isEpsilonFind = false;
						if (it != v.end() - 1) { 
							auto lastPart = getVectorLastPart(it + 1, v.end());
							auto updSet = FIRST(lastPart);
							if (updSet.find("e") != updSet.end()) {
								isEpsilonFind = true;
								updSet.erase("e");
							}
							FOLLOWForG[(*it)].insert(updSet.begin(), updSet.end());
						}
						if (it == v.end() - 1 || isEpsilonFind) { 
							FOLLOWForG[(*it)].insert(FOLLOWForG[pair.first].begin(), FOLLOWForG[pair.first].end());
						}
					}
					++it;
				}
			}
		}
		newLength = FOLLOWForGLength();
	} while (newLength != currentLength);

}

std::set<std::string> Grammar::FOLLOW(const std::string& N) {
	return FOLLOWForG[N];
}

std::ostream& operator << (std::ostream& stream, const std::set<std::string>& terminals) {
	std::set<std::string>::iterator p = terminals.begin();
	stream << "[";

	while (p != terminals.end()) {
		stream << p->c_str();
		++p;
		if (p != terminals.end()) {
			stream << ", ";
		}
	}
	stream << "]" << std::endl;

	return stream;
}

bool Grammar::buildSAtable() {
	bool isGrammarNonConflict{ true };

	for (auto& rulesBlock : grammar) {
		for (auto& rule : rulesBlock.second) {
			auto first = FIRST(rule);
			std::vector<std::string> reversedRule(rule.rbegin(), rule.rend());
			std::pair<std::vector<std::string>, std::string> currentAction{ reversedRule, "R" };
			bool isEpsilonFind{ false };

			for (auto& terminal : first) {
				if (terminal == "e") isEpsilonFind = true;
				if (SATable[rulesBlock.first].find(terminal) != SATable[rulesBlock.first].end()) {
					isGrammarNonConflict = false;
				}
				SATable[rulesBlock.first][terminal] = currentAction;
			}
			if (isEpsilonFind) {
				auto follow = FOLLOW(rulesBlock.first);

				for (auto& terminal : follow) {
					SATable[rulesBlock.first][terminal] = currentAction;
				}
			}
		}
	}

	for (auto& terminal : terminals) {
		SATable[terminal][terminal] = std::pair<std::vector<std::string>, std::string>{ std::vector<std::string>{"Pop"}, "A" };
	}
	SATable["$"]["$"] = std::pair<std::vector<std::string>, std::string>{ std::vector<std::string>{"Accept"}, "A" };

	return isGrammarNonConflict;
}

void Grammar::printSATable(std::ostream& stream) {
	stream << "   ";
	for (auto& terminal : terminals) {
		stream << std::setw(10) << std::setiosflags(std::ios::left) << terminal;
	}
	stream << std::endl;
	for (size_t i = 0; i < terminals.size() * 10; ++i) {
		stream << '-';
	}
	stream << std::endl;
	for (auto& columnSymbol : SATable) {
		stream << std::endl << columnSymbol.first << " ";
		for (auto& terminal : terminals) {
			if (columnSymbol.second.find(terminal) != columnSymbol.second.end()) {
				int currentLenght{};

				for (auto& term : columnSymbol.second[terminal].first) {
					stream << term;
					currentLenght += term.length();
				}

				stream << ',' << columnSymbol.second[terminal].second;
				for (int i = 0; i < 8 - currentLenght; ++i) {
					stream << " ";
				}

			}
			else {
				stream << "          ";
			}
		}
	}
	stream << std::endl;
}

bool Grammar::parse(std::vector<std::string> expression) {
	std::stack<std::string> pushdownStack;
	pushdownStack.push("$");
	pushdownStack.push(startNonTerminal);

	auto expressionIterator = expression.begin();
	while (expressionIterator != expression.end()) {
		if (pushdownStack.top() == *expressionIterator) {
			pushdownStack.pop();
			++expressionIterator;
		}
		else if (SATable[pushdownStack.top()].find(*expressionIterator) != SATable[pushdownStack.top()].end()) { 
			std::string tmp = pushdownStack.top();
			pushdownStack.pop();

			if (SATable[tmp][*expressionIterator].first[0] != "e") {
				for (auto& ruleElement : SATable[tmp][*expressionIterator].first) {
					pushdownStack.push(ruleElement);
				}
			}
		}
		else {
			return false;
		}
	}

	while (true) {
		std::string currentTop = pushdownStack.top();
		pushdownStack.pop();

		if (currentTop == "$") return true;

		if (std::find(
			grammar[currentTop].begin(), 
			grammar[currentTop].end(), 
			std::vector<std::string>{"e"}) == grammar[currentTop].end()
			) 
			return false;

	}
}