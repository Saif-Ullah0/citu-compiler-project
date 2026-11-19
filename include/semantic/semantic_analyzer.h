#pragma once
#include "common/token.h"
#include "semantic/symbol_table.h"
#include <vector>

class SemanticAnalyzer {
public:
    SemanticAnalyzer(const std::vector<Token>& tokens);
    bool analyze(std::ostream& out);

private:
    std::vector<Token> tokens;
    SymbolTable symbolTable;
    int currentPos;
    
    // Helper to get current token
    Token current();
    void advance();
};