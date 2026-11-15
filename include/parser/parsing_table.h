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


// --- SLR(1) Tables ---
// Action: map<State, map<Token, ActionString>>
// ActionString formats: "s3" (shift to state 3), "r2" (reduce rule 2), "acc"
using SLRActionTable = std::map<int, std::map<std::string, std::string>>;
using SLRGotoTable = std::map<int, std::map<std::string, int>>;

inline SLRActionTable getSLR1ActionTable() {
    SLRActionTable t;
    // State 0
    t[0]["main"] = "s2";
    // State 1 (Accept)
    t[1]["$"] = "acc";
    // State 2
    t[2]["shro"] = "s3";
    // State 3 (Expect statements or 'int')
    t[3]["int"] = "s6";
    // State 4
    t[4]["khatam"] = "s7";
    // State 5 (Reduce L -> D)
    t[5]["khatam"] = "r2";
    // State 6
    t[6]["ID"] = "s8";
    // State 7 (Reduce P -> main shro L khatam)
    t[7]["$"] = "r1";
    // State 8
    t[8][";"] = "s9";
    // State 9 (Reduce D -> int ID ;)
    t[9]["khatam"] = "r3";

    return t;
}

inline SLRGotoTable getSLR1GotoTable() {
    SLRGotoTable t;
    t[0]["P"] = 1;
    t[0]["main"] = 2; // Technically terminals aren't in GOTO, but for our parser logic we track it
    t[2]["shro"] = 3;
    t[3]["L"] = 4;
    t[3]["D"] = 5;
    t[3]["int"] = 6;
    t[4]["khatam"] = 7;
    t[6]["ID"] = 8;
    t[8][";"] = 9;
    return t;
}

// Helper to get RHS length for reduction
inline int getRuleLength(int ruleNum) {
    if (ruleNum == 1) return 4; // P -> main shro L khatam
    if (ruleNum == 2) return 1; // L -> D
    if (ruleNum == 3) return 3; // D -> int ID ;
    return 0;
}
// Helper to get LHS of rule
inline std::string getRuleLHS(int ruleNum) {
    if (ruleNum == 1) return "P";
    if (ruleNum == 2) return "L";
    if (ruleNum == 3) return "D";
    return "";
}