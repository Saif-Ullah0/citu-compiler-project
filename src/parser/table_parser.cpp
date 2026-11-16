#include "parser/table_parser.h"
#include <iostream>
#include <sstream>

TableParser::TableParser(const std::vector<Token>& tokens, bool useLL1) : tokens(tokens) {
    if (useLL1) {
        table = getLL1Table();
    } else {
        // Fallback for now (SLR1 will be implemented next)
        table = getLL1Table();
    }
}

void TableParser::pushRule(const std::string& rule) {
    if (rule == "epsilon" || rule == "ε") return; // Don't push empty strings

    std::istringstream iss(rule);
    std::string symbol;
    std::vector<std::string> symbols;

    // Split the RHS by spaces
    while (iss >> symbol) {
        symbols.push_back(symbol);
    }

    // Push in REVERSE order so the leftmost symbol ends up on top of the stack
    for (int i = symbols.size() - 1; i >= 0; --i) {
        parseStack.push(symbols[i]);
    }
}

bool TableParser::parse() {
    int i = 0; // Input pointer
    parseStack.push("$");       // Bottom of stack
    parseStack.push("PROGRAM"); // Start symbol

    std::cout << "Stack\t\tInput\t\tAction\n";
    std::cout << "--------------------------------------------------\n";

    while (!parseStack.empty()) {
        std::string top = parseStack.top();
        std::string currentToken = tokens[i].lexeme;

        // Convert Lexer types to Parser grammar symbols
        if (tokens[i].type == TokenType::END_OF_FILE) {
            currentToken = "$";
        } else if (tokens[i].type == TokenType::IDENTIFIER) {
            currentToken = "ID";
        } else if (tokens[i].type == TokenType::INT_CONST) {
            currentToken = "NUM";
        }

        std::cout << top << "\t\t" << currentToken << "\t\t";

        // 1. SUCCESS CONDITION
        if (top == "$" && currentToken == "$") {
            std::cout << "ACCEPT\n";
            return true;
        }

        // 2. TERMINAL MATCHING
        if (top == currentToken) {
            std::cout << "Match " << top << "\n";
            parseStack.pop();
            i++;
        }
        // 3. NON-TERMINAL EXPANSION (Table Lookup)
        else {
            if (table.find(top) != table.end() && table[top].find(currentToken) != table[top].end()) {
                std::string rule = table[top][currentToken];
                std::cout << "Output " << top << " -> " << rule << "\n";
                parseStack.pop();
                pushRule(rule);
            } else {
                std::cout << "ERROR: No rule for [" << top << "] and [" << currentToken << "]\n";
                return false;
            }
        }
    }
    return false;
}