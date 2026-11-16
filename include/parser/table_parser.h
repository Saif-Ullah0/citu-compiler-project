#pragma once
#include "common/token.h"
#include "parser/parsing_table.h"
#include <stack>
#include <vector>
#include <string>

class TableParser {
public:
    TableParser(const std::vector<Token>& tokens, bool useLL1);
    bool parse();

private:
    std::vector<Token> tokens;
    Table table;
    std::stack<std::string> parseStack;

    void pushRule(const std::string& rule);
};