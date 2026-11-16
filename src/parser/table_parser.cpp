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

bool TableParser::parse(std::ostream& out) {
    int i = 0; // Input pointer
    parseStack.push("$");       // Bottom of stack
    parseStack.push("PROGRAM"); // Start symbol

    out << "Stack\t\tInput\t\tAction\n";
    out << "--------------------------------------------------\n";

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

        out << top << "\t\t" << currentToken << "\t\t";

        // 1. SUCCESS CONDITION
        if (top == "$" && currentToken == "$") {
            out << "ACCEPT\n";
            return true;
        }

        // 2. TERMINAL MATCHING
        if (top == currentToken) {
            out << "Match " << top << "\n";
            parseStack.pop();
            i++;
        }
        // 3. NON-TERMINAL EXPANSION (Table Lookup)
        else {
            if (table.find(top) != table.end() && table[top].find(currentToken) != table[top].end()) {
                std::string rule = table[top][currentToken];
                out << "Output " << top << " -> " << rule << "\n";
                parseStack.pop();
                pushRule(rule);
            } else {
                out << "ERROR: No rule for [" << top << "] and [" << currentToken << "]\n";
                return false;
            }
        }
    }
    return false;
}

bool TableParser::parseSLR1(std::ostream& out) {
    SLRActionTable actionTable = getSLR1ActionTable();
    SLRGotoTable gotoTable = getSLR1GotoTable();

    std::stack<int> stateStack;
    stateStack.push(0); // Start at state 0
    int i = 0;

    out << "StateStack\tInput\t\tAction\n";
    out << "--------------------------------------------------\n";

    while (true) {
        int currentState = stateStack.top();
        std::string currentToken = tokens[i].lexeme;

        // Map tokens
        if (tokens[i].type == TokenType::END_OF_FILE) currentToken = "$";
        else if (tokens[i].type == TokenType::IDENTIFIER) currentToken = "ID";
        else if (tokens[i].type == TokenType::INT_CONST) currentToken = "NUM";

        out << currentState << "\t\t" << currentToken << "\t\t";

        if (actionTable[currentState].find(currentToken) == actionTable[currentState].end()) {
            out << "ERROR: No action for state " << currentState << " and " << currentToken << "\n";
            return false;
        }

        std::string action = actionTable[currentState][currentToken];

        if (action == "acc") {
            out << "ACCEPT\n";
            return true;
        } 
        else if (action[0] == 's') { // Shift
            int nextState = std::stoi(action.substr(1));
            out << "Shift to state " << nextState << "\n";
            stateStack.push(nextState);
            i++; // Advance input
        } 
        else if (action[0] == 'r') { // Reduce
            int ruleNum = std::stoi(action.substr(1));
            int popCount = getRuleLength(ruleNum);
            std::string lhs = getRuleLHS(ruleNum);
            
            out << "Reduce by rule " << ruleNum << " (" << lhs << ")\n";
            
            for (int p = 0; p < popCount; p++) {
                stateStack.pop();
            }
            
            int topState = stateStack.top();
            int gotoState = gotoTable[topState][lhs];
            stateStack.push(gotoState);
        }
    }
}