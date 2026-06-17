#include "parser.h"
#include <assert.h>

static int sameTT(truthTable* a, truthTable* b) {
    return a->varNum == b->varNum && a->ttrep == b->ttrep &&
           a->maskSpec == b->maskSpec;
}

int main() {
    {
        exprNode* e = parseExpr("a * b");
        assert(e != NULL);
        assert(exprVarNum(e) == 2);
        truthTable* sim = simulateExpr(e);
        truthTable* ref = readTT("1000", 0);
        assert(sameTT(sim, ref));
        assert(clearTT(sim) == 0);
        assert(clearTT(ref) == 0);
        assert(clearExpr(e) == 0);
    }
    {
        exprNode* e = parseExpr("a + b");
        assert(e != NULL);
        truthTable* sim = simulateExpr(e);
        truthTable* ref = readTT("1110", 0);
        assert(sameTT(sim, ref));
        assert(clearTT(sim) == 0);
        assert(clearTT(ref) == 0);
        assert(clearExpr(e) == 0);
    }
    {
        exprNode* e = parseExpr("~a");
        assert(e != NULL);
        assert(exprVarNum(e) == 1);
        truthTable* sim = simulateExpr(e);
        truthTable* ref = readTT("01", 0);
        assert(sameTT(sim, ref));
        assert(clearTT(sim) == 0);
        assert(clearTT(ref) == 0);
        assert(clearExpr(e) == 0);
    }
    {
        exprNode* e = parseExpr("~(~a * (~b + c))");
        assert(e != NULL);
        assert(exprVarNum(e) == 3);
        truthTable* sim = simulateExpr(e);
        truthTable* ref = readTT("10101110", 0);
        assert(sameTT(sim, ref));
        assert(clearTT(sim) == 0);
        assert(clearTT(ref) == 0);
        assert(clearExpr(e) == 0);
    }
    {
        exprNode* e = parseExpr("a + b * c");
        assert(e != NULL);
        assert(exprVarNum(e) == 3);
        truthTable* sim = simulateExpr(e);
        truthTable* ref = readTT("11101010", 0);
        assert(sameTT(sim, ref));
        assert(clearTT(sim) == 0);
        assert(clearTT(ref) == 0);
        assert(clearExpr(e) == 0);
    }
    {
        exprNode* e = parseExpr("a * c");
        assert(e != NULL);
        assert(exprVarNum(e) == 2);
        truthTable* sim = simulateExpr(e);
        truthTable* ref = readTT("1000", 0);
        assert(sameTT(sim, ref));
        assert(clearTT(sim) == 0);
        assert(clearTT(ref) == 0);
        assert(clearExpr(e) == 0);
    }
    {
        exprNode* e = parseExpr("c * a");
        assert(e != NULL);
        assert(exprVarNum(e) == 2);
        truthTable* sim = simulateExpr(e);
        truthTable* ref = readTT("1000", 0);
        assert(sameTT(sim, ref));
        assert(clearTT(sim) == 0);
        assert(clearTT(ref) == 0);
        assert(clearExpr(e) == 0);
    }
    {
        exprNode* e = parseExpr("((a))");
        assert(e != NULL);
        truthTable* sim = simulateExpr(e);
        truthTable* ref = readTT("10", 0);
        assert(sameTT(sim, ref));
        assert(clearTT(sim) == 0);
        assert(clearTT(ref) == 0);
        assert(clearExpr(e) == 0);
    }

    {
        // at most 6 distinct variables: the boundary is accepted
        exprNode* e = parseExpr("a * b * c * d * e * f");
        assert(e != NULL);
        assert(exprVarNum(e) == 6);
        truthTable* sim = simulateExpr(e);
        assert(sim != NULL);
        assert(sim->varNum == 6);
        assert(sim->ttrep == (1UL << 63));
        assert(clearTT(sim) == 0);
        assert(clearExpr(e) == 0);
    }
    {
        // reused variables do not inflate the count
        exprNode* e = parseExpr("a * b * a");
        assert(e != NULL);
        assert(exprVarNum(e) == 2);
        truthTable* sim = simulateExpr(e);
        truthTable* ref = readTT("1000", 0);
        assert(sameTT(sim, ref));
        assert(clearTT(sim) == 0);
        assert(clearTT(ref) == 0);
        assert(clearExpr(e) == 0);
    }
    // a 7th distinct variable is rejected at lex time
    assert(parseExpr("a * b * c * d * e * f * g") == NULL);

    assert(parseExpr("ab") == NULL);
    assert(parseExpr("a b") == NULL);
    assert(parseExpr("a * ~b") == NULL);
    assert(parseExpr("~~a") == NULL);
    assert(parseExpr("a *") == NULL);
    assert(parseExpr("*a") == NULL);
    assert(parseExpr("a + b *") == NULL);
    assert(parseExpr("(a * b") == NULL);
    assert(parseExpr("a * b)") == NULL);
    assert(parseExpr("a & b") == NULL);
    assert(parseExpr("") == NULL);
    assert(parseExpr("()") == NULL);
    assert(parseExpr("a * * b") == NULL);
    return 0;
}
