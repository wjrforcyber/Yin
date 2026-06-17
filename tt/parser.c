#include "parser.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum tokType {
    T_TILDE = 0, T_STAR, T_PLUS, T_LPAREN, T_RPAREN, T_IDENT, T_EOF,
};

typedef struct {
    enum tokType type;
    int varIndex;
} token;

static int isOpToken(enum tokType t) {
    return t == T_TILDE || t == T_STAR || t == T_PLUS;
}

// Lex src into a freshly malloc'd token array terminated by T_EOF.
// Returns NULL (and prints a reason) on an invalid character or empty input.
static token* lexExpr(char* src, int* count) {
    size_t len = strlen(src);
    token* toks = (token*)malloc(sizeof(token) * (len + 1));
    if (toks == NULL) {
        return NULL;
    }
    int varMap[26];
    for (int k = 0; k < 26; k++) {
        varMap[k] = -1;
    }
    int varCount = 0;
    size_t n = 0;
    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)src[i];
        if (isspace(c)) {
            continue;
        }
        token t;
        t.varIndex = 0;
        switch (c) {
            case '~':
                t.type = T_TILDE;
                break;
            case '*':
                t.type = T_STAR;
                break;
            case '+':
                t.type = T_PLUS;
                break;
            case '(':
                t.type = T_LPAREN;
                break;
            case ')':
                t.type = T_RPAREN;
                break;
            default:
                if (isalpha(c)) {
                    int off = tolower(c) - 'a';
                    if (varMap[off] == -1) {
                        if (varCount == 6) {
                            printf("Lex error: more than 6 variables.\n");
                            free(toks);
                            return NULL;
                        }
                        varMap[off] = varCount++;
                    }
                    t.type = T_IDENT;
                    t.varIndex = varMap[off];
                    break;
                }
                printf("Lex error: unknown character '%c'.\n", src[i]);
                free(toks);
                return NULL;
        }
        toks[n++] = t;
    }
    if (n == 0) {
        printf("Lex error: empty expression.\n");
        free(toks);
        return NULL;
    }
    toks[n].type = T_EOF;
    toks[n].varIndex = 0;
    *count = (int)(n + 1);
    return toks;
}

static exprNode* newVarNode(int idx) {
    exprNode* node = (exprNode*)malloc(sizeof(exprNode));
    node->type = VAR_NODE;
    node->varIndex = idx;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static exprNode* newNotNode(exprNode* child) {
    exprNode* node = (exprNode*)malloc(sizeof(exprNode));
    node->type = NOT_NODE;
    node->varIndex = -1;
    node->left = child;
    node->right = NULL;
    return node;
}

static exprNode* newAndNode(exprNode* lhs, exprNode* rhs) {
    exprNode* node = (exprNode*)malloc(sizeof(exprNode));
    node->type = AND_NODE;
    node->varIndex = -1;
    node->left = lhs;
    node->right = rhs;
    return node;
}

static exprNode* newOrNode(exprNode* lhs, exprNode* rhs) {
    exprNode* node = (exprNode*)malloc(sizeof(exprNode));
    node->type = OR_NODE;
    node->varIndex = -1;
    node->left = lhs;
    node->right = rhs;
    return node;
}

static exprNode* parseOr(token* toks, int* pos);

static exprNode* parsePrimary(token* toks, int* pos) {
    token t = toks[*pos];
    if (t.type == T_IDENT) {
        exprNode* v = newVarNode(t.varIndex);
        (*pos)++;
        return v;
    }
    if (t.type == T_LPAREN) {
        (*pos)++;
        exprNode* e = parseOr(toks, pos);
        if (e == NULL) {
            return NULL;
        }
        if (toks[*pos].type != T_RPAREN) {
            printf("Parse error: unmatched '('.\n");
            clearExpr(e);
            return NULL;
        }
        (*pos)++;
        return e;
    }
    printf("Parse error: expected a variable or '('.\n");
    return NULL;
}

static exprNode* parseNot(token* toks, int* pos) {
    if (toks[*pos].type == T_TILDE) {
        (*pos)++;
        exprNode* child = parsePrimary(toks, pos);
        if (child == NULL) {
            return NULL;
        }
        return newNotNode(child);
    }
    return parsePrimary(toks, pos);
}

static exprNode* parseAnd(token* toks, int* pos) {
    exprNode* node = parseNot(toks, pos);
    if (node == NULL) {
        return NULL;
    }
    while (toks[*pos].type == T_STAR) {
        (*pos)++;
        exprNode* rhs = parseNot(toks, pos);
        if (rhs == NULL) {
            clearExpr(node);
            return NULL;
        }
        node = newAndNode(node, rhs);
    }
    return node;
}

static exprNode* parseOr(token* toks, int* pos) {
    exprNode* node = parseAnd(toks, pos);
    if (node == NULL) {
        return NULL;
    }
    while (toks[*pos].type == T_PLUS) {
        (*pos)++;
        exprNode* rhs = parseAnd(toks, pos);
        if (rhs == NULL) {
            clearExpr(node);
            return NULL;
        }
        node = newOrNode(node, rhs);
    }
    return node;
}

exprNode* parseExpr(char* src) {
    if (src == NULL) {
        printf("Parse error: NULL input.\n");
        return NULL;
    }
    int count = 0;
    token* toks = lexExpr(src, &count);
    if (toks == NULL) {
        return NULL;
    }
    for (int i = 0; i + 1 < count; i++) {
        if (isOpToken(toks[i].type) && isOpToken(toks[i + 1].type)) {
            printf("Parse error: consecutive operators; use brackets.\n");
            free(toks);
            return NULL;
        }
    }
    int pos = 0;
    exprNode* root = parseOr(toks, &pos);
    if (root == NULL) {
        free(toks);
        return NULL;
    }
    if (toks[pos].type != T_EOF) {
        printf("Parse error: trailing tokens (missing operator?).\n");
        clearExpr(root);
        free(toks);
        return NULL;
    }
    free(toks);
    return root;
}

int clearExpr(exprNode* root) {
    if (root == NULL) {
        return 0;
    }
    clearExpr(root->left);
    clearExpr(root->right);
    free(root);
    return 0;
}

int exprVarNum(exprNode* root) {
    if (root == NULL) {
        return 0;
    }
    if (root->type == VAR_NODE) {
        return root->varIndex + 1;
    }
    int lhs = exprVarNum(root->left);
    int rhs = exprVarNum(root->right);
    return lhs > rhs ? lhs : rhs;
}

static truthTable* evalNode(exprNode* node, int varNum) {
    switch (node->type) {
        case VAR_NODE: {
            truthTable* v = initTT(varNum);
            initElementaryVar(v, varNum, node->varIndex);
            return v;
        }
        case NOT_NODE: {
            truthTable* child = evalNode(node->left, varNum);
            truthTable* res = notCopy(child);
            clearTT(child);
            return res;
        }
        case AND_NODE: {
            truthTable* lhs = evalNode(node->left, varNum);
            truthTable* rhs = evalNode(node->right, varNum);
            truthTable* res = andCopy(lhs, rhs);
            clearTT(lhs);
            clearTT(rhs);
            return res;
        }
        case OR_NODE: {
            truthTable* lhs = evalNode(node->left, varNum);
            truthTable* rhs = evalNode(node->right, varNum);
            truthTable* res = orCopy(lhs, rhs);
            clearTT(lhs);
            clearTT(rhs);
            return res;
        }
        default:
            return NULL;
    }
}

truthTable* simulateExpr(exprNode* root) {
    if (root == NULL) {
        return NULL;
    }
    int varNum = exprVarNum(root);
    if (varNum > 6) {
        printf("Sim error: expression spans more than 6 variables.\n");
        return NULL;
    }
    return evalNode(root, varNum);
}
