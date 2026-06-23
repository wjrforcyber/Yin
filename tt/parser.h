/**
 * \file parser.h
 * \brief Boolean-expression parsing and exact simulation.
 *
 * Parses ASCII Boolean expressions into an AST and symbolically simulates them
 * over all input assignments to produce a truth table. The grammar is:
 *
 *   - Operators, highest precedence first: `~` (NOT), `*` (AND), `+` (OR).
 *   - Identifiers are a single ASCII letter (`a`-`z`, case-insensitive).
 *   - Parentheses `()` group sub-expressions.
 *
 * Variables are mapped to indices 0, 1, 2, ... in order of first appearance, so
 * an expression is treated as a complete function of its distinct variables
 * (no unused slots). At most 6 distinct variables are supported.
 */
#ifndef YIN_PARSER_H
#define YIN_PARSER_H

#include "tt.h"

/** AST node kind. */
enum NODE_TYPE { VAR_NODE = 0, NOT_NODE, AND_NODE, OR_NODE };

/** Opaque node of a parsed Boolean-expression AST. */
typedef struct exprNode_ exprNode;

/**
 * \brief One node of a Boolean-expression AST.
 *
 * \struct exprNode_
 */
struct exprNode_ {
    enum NODE_TYPE type;  ///< Node kind.
    int varIndex;         ///< For #VAR_NODE: the variable index. -1 otherwise.
    exprNode* left;       ///< NOT child / AND,OR left operand (NULL for VAR).
    exprNode* right;      ///< AND,OR right operand (NULL for VAR, NOT).
};

/**
 * \defgroup tt_parser Expression parser
 * \brief Parse a Boolean expression into an AST and simulate it.
 * @{
 */

/**
 * Parse a Boolean expression into an AST.
 *
 * \param src NUL-terminated expression. Whitespace is ignored.
 * \return The AST root, or NULL on a syntax error. Rejected (NULL, with a
 *         reason printed to stdout): implicit operators such as `"ab"`;
 *         any two adjacent operators such as `"a * ~b"` or `"~~a"` (use
 *         brackets to combine instead); unmatched parentheses; unknown
 *         characters; empty input; NULL input; and more than 6 distinct
 *         variables.
 *
 * \par Ownership
 * Caller owns the AST and must release it with clearExpr().
 */
exprNode* parseExpr(char* src);

/**
 * Count the variables spanned by an AST.
 * \param root AST root (may be NULL).
 * \return Highest variable index referenced by the tree, plus one (1..6), or
 *         0 if \p root is NULL.
 */
int exprVarNum(exprNode* root);

/**
 * Symbolically simulate an AST over all input assignments.
 *
 * Evaluates every node as a truth-table operation and returns the resulting
 * complete truth table of `exprVarNum(root)` variables.
 *
 * \param root AST root from parseExpr().
 * \return A freshly allocated truth table, or NULL if \p root is NULL or spans
 *         more than 6 variables.
 *
 * \par Ownership
 * Caller owns the result and must release it with clearTT().
 */
truthTable* simulateExpr(exprNode* root);

/**
 * Recursively free an AST built by parseExpr().
 * \param root AST root; NULL is accepted as a no-op.
 * \return Always 0.
 */
int clearExpr(exprNode* root);

/** @} */ /* end tt_parser */

#endif /* YIN_PARSER_H */
