#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

// 1. The entry for a single variable
struct SymbolEntry {
    std::string name;
    std::string type;    
    int line;            
    int scopeLevel;
};

// 2. The Symbol Table Manager
class SymbolTable {
public:
    SymbolTable() {
        scopes.push_back({}); // Global Scope
    }

    void enterScope() {
        scopes.push_back({});
    }

    void exitScope() {
        if (scopes.size() > 1) {
            scopes.pop_back();
        }
    }

    bool insert(const std::string& name, const std::string& type, int line) {
        auto& currentScope = scopes.back();
        
        if (currentScope.find(name) != currentScope.end()) {
            std::cerr << "Semantic Error (Line " << line << "): Redeclaration of '" << name << "'\n";
            return false;
        }
        
        int level = scopes.size() - 1;
        currentScope[name] = {name, type, line, level};
        
        // Save to history so we can print it later, even after scope is exited
        history.push_back({name, type, line, level});
        return true;
    }

    bool lookup(const std::string& name) {
        for (int i = scopes.size() - 1; i >= 0; --i) {
            if (scopes[i].find(name) != scopes[i].end()) {
                return true;
            }
        }
        return false;
    }

    void printToFile(std::ostream& out) {
        out << "Index | Name | Type | Scope Level\n";
        out << "----------------------------------\n";
        int index = 1;
        // Print from history, not from the active stack
        for (const auto& entry : history) {
            out << index++ << " | " 
                << entry.name << " | " 
                << entry.type << " | " 
                << entry.scopeLevel << "\n";
        }
    }

private:
    std::vector<std::unordered_map<std::string, SymbolEntry>> scopes;
    std::vector<SymbolEntry> history; // Remembers all declared variables
};