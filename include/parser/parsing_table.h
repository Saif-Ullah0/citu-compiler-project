#pragma once
#include <string>
#include <map>

// Table format: table[Non-Terminal][Terminal] = "Rule Right-Hand-Side"
using Table = std::map<std::string, std::map<std::string, std::string>>;

inline Table getLL1Table() {
    Table t;
    
    // PROGRAM -> main shro STMT_LIST khatam
    t["PROGRAM"]["main"] = "main shro STMT_LIST khatam";
    
    // STMT_LIST -> STMT STMT_LIST | epsilon
    t["STMT_LIST"]["int"] = "STMT STMT_LIST";
    t["STMT_LIST"]["char"] = "STMT STMT_LIST";
    t["STMT_LIST"]["float"] = "STMT STMT_LIST";
    t["STMT_LIST"]["ID"] = "STMT STMT_LIST";
    t["STMT_LIST"]["shro"] = "STMT STMT_LIST";
    t["STMT_LIST"]["khatam"] = "epsilon";
    t["STMT_LIST"]["$"] = "epsilon";

    // STMT -> DECL | ASSIGN
    t["STMT"]["int"] = "DECL";
    t["STMT"]["char"] = "DECL";
    t["STMT"]["float"] = "DECL";
    t["STMT"]["ID"] = "ASSIGN";

    // DECL -> TYPE ID ;
    t["DECL"]["int"] = "TYPE ID ;";
    t["DECL"]["char"] = "TYPE ID ;";
    t["DECL"]["float"] = "TYPE ID ;";

    // TYPE -> int | char | float
    t["TYPE"]["int"] = "int";
    t["TYPE"]["char"] = "char";
    t["TYPE"]["float"] = "float";

    // ASSIGN -> ID = EXPR ;
    t["ASSIGN"]["ID"] = "ID = EXPR ;";

    // EXPR -> TERM EXPR_PRIME
    t["EXPR"]["ID"] = "TERM EXPR_PRIME";
    t["EXPR"]["NUM"] = "TERM EXPR_PRIME";
    t["EXPR"]["("] = "TERM EXPR_PRIME";

    // EXPR_PRIME -> + TERM EXPR_PRIME | epsilon
    t["EXPR_PRIME"]["+"] = "+ TERM EXPR_PRIME";
    t["EXPR_PRIME"]["ID"] = "epsilon";  // Follow set
    t["EXPR_PRIME"][";"] = "epsilon";
    t["EXPR_PRIME"]["$"] = "epsilon";

    // TERM -> FACTOR TERM_PRIME
    t["TERM"]["ID"] = "FACTOR TERM_PRIME";
    t["TERM"]["NUM"] = "FACTOR TERM_PRIME";
    t["TERM"]["("] = "FACTOR TERM_PRIME";

    // TERM_PRIME -> * FACTOR TERM_PRIME | epsilon
    t["TERM_PRIME"]["*"] = "* FACTOR TERM_PRIME";
    t["TERM_PRIME"]["+"] = "epsilon";
    t["TERM_PRIME"][";"] = "epsilon";
    t["TERM_PRIME"]["$"] = "epsilon";

    // FACTOR -> ID | NUM | ( EXPR )
    t["FACTOR"]["ID"] = "ID";
    t["FACTOR"]["NUM"] = "NUM";
    t["FACTOR"]["("] = "( EXPR )";

    return t;
}