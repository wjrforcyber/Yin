#pragma once

#include "tt.h"

enum NODE_TYPE { VAR_NODE = 0, NOT_NODE, AND_NODE, OR_NODE };

typedef struct exprNode_ exprNode;
struct exprNode_ {
    enum NODE_TYPE type;
    int varIndex;
    exprNode* left;
    exprNode* right;
};

// Precedence high -> low: ~ , * , +. IDENT is one ASCII letter (a-z / A-Z).
// Variables are mapped to indices 0, 1, 2, ... in order of first appearance,
// so an expression is treated as a complete function of its distinct
// variables (no unused slots). At most 6 distinct variables are supported.
// Rejected (returns NULL, prints a reason): implicit operators ("ab"), any two
// adjacent operators ("a * ~b", "~~a") -- use brackets to combine, unmatched
// parentheses, unknown characters, and empty input.
exprNode* parseExpr(char* src);

// Highest variable index referenced by the tree, plus one (1..6).
int exprVarNum(exprNode* root);

// Exact symbolic simulation over all input assignments. Returns a freshly
// allocated truthTable* (free with clearTT), or NULL on error.
truthTable* simulateExpr(exprNode* root);

// Recursively free an AST built by parseExpr. Returns 0.
int clearExpr(exprNode* root);
