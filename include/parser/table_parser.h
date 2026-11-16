#pragma once
#include "common/token.h"
#include "parser/parsing_table.h"
#include <stack>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

class TableParser {
public:
    TableParser(const std::vector<Token>& tokens, bool useLL1);
    
    bool parse(std::ostream& out);
    bool parseSLR1(std::ostream& out);

private:
    std::vector<Token> tokens;
    Table table;
    std::stack<std::string> parseStack;

    void pushRule(const std::string& rule);
};