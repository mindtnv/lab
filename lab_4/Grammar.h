#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>

//#include <utitlity>

std::ostream& operator << (std::ostream& stream, const std::set<std::string>& terminals);

class Grammar {
private:
	std::map<std::string, std::vector<std::vector<std::string>>> grammar;
	std::map<std::string, std::set<std::string>> FIRSTForG;
	std::map<std::string, std::set<std::string>> FOLLOWForG;
	std::map<std::string, std::map < std::string, std::pair<std::vector<std::string>, std::string>>> SATable;

	std::string startNonTerminal;
	std::set<std::string> terminals;

	void initFIRSTWithTerminalsAndEpsilon();
	void initFIRSTWithNonTerminals();
	void initFOLLOW();
	void calculateFOLLOW();
	bool buildSAtable();


public:
	Grammar();
	~Grammar();
	bool loadGrammar(std::istream& stream);
	void printFIRSTForG(std::ostream& stream);
	void printFOLLOWForG(std::ostream& stream);
	void printSATable(std::ostream& stream);
	bool parse(std::vector<std::string> expression);
	std::set<std::string> FIRST(const std::vector<std::string>&);
	std::set<std::string> FOLLOW(const std::string&);

	int FIRSTForGLength();
	int FOLLOWForGLength();
	std::vector<std::string> getVectorLastPart(std::vector<std::string>::iterator start, std::vector<std::string>::iterator end);

	friend std::ostream& operator << (std::ostream& stream, const Grammar& g);
};