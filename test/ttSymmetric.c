#include "symmetric.h"
#include <assert.h>

int main(){
    //AND (2 vars): symmetric, output depends only on popcount
    char tts_and[] = "1000";
    truthTable *ttAnd = readTT(tts_and, 0);
    assert(isSymmetric(ttAnd) == 1);
    clearTT(ttAnd);

    //OR (2 vars): symmetric
    char tts_or[] = "1110";
    truthTable *ttOr = readTT(tts_or, 0);
    assert(isSymmetric(ttOr) == 1);
    clearTT(ttOr);

    //XOR (2 vars): symmetric, popcount parity determines output
    char tts_xor[] = "0110";
    truthTable *ttXor = readTT(tts_xor, 0);
    assert(isSymmetric(ttXor) == 1);
    clearTT(ttXor);

    //XNOR (2 vars): symmetric
    char tts_xnor[] = "1001";
    truthTable *ttXnor = readTT(tts_xnor, 0);
    assert(isSymmetric(ttXnor) == 1);
    clearTT(ttXnor);

    //3-input majority: symmetric
    char tts_maj[] = "11101000";
    truthTable *ttMaj = readTT(tts_maj, 0);
    assert(isSymmetric(ttMaj) == 1);
    clearTT(ttMaj);

    //3-input OR: symmetric
    char tts_or3[] = "11111110";
    truthTable *ttOr3 = readTT(tts_or3, 0);
    assert(isSymmetric(ttOr3) == 1);
    clearTT(ttOr3);

    //constant 0: symmetric
    char tts_zero[] = "0000";
    truthTable *ttZero = readTT(tts_zero, 0);
    assert(isSymmetric(ttZero) == 1);
    clearTT(ttZero);

    //constant 1: symmetric
    char tts_one[] = "1111";
    truthTable *ttOne = readTT(tts_one, 0);
    assert(isSymmetric(ttOne) == 1);
    clearTT(ttOne);

    //f = x0 (2 vars): NOT symmetric, f(01)=1 but f(10)=0, both popcount 1
    char tts_proj[] = "1010";
    truthTable *ttProj = readTT(tts_proj, 0);
    assert(isSymmetric(ttProj) == 0);
    clearTT(ttProj);

    //f = x0 AND NOT x1 (2 vars): NOT symmetric
    //f(00)=0 f(01)=0 f(10)=1 f(11)=0
    char tts_anotb[] = "0010";
    truthTable *ttAnotb = readTT(tts_anotb, 0);
    assert(isSymmetric(ttAnotb) == 0);
    clearTT(ttAnotb);

    //f = x0 XOR x1 XOR x2 (3 vars): symmetric (parity function)
    char tts_xor3[] = "10010110";
    truthTable *ttXor3 = readTT(tts_xor3, 0);
    assert(isSymmetric(ttXor3) == 1);
    clearTT(ttXor3);

    //partially symmetric tests: isSymmetric2Vars
    //f = x0 AND x1, x2 irrelevant (3 vars): symmetric in x0,x1, not in x0,x2
    //f(000)=0 f(001)=0 f(010)=0 f(011)=1 f(100)=0 f(101)=0 f(110)=0 f(111)=1
    char tts_and3[] = "10001000";
    truthTable *ttAnd3 = readTT(tts_and3, 0);
    assert(isSymmetric2Vars(ttAnd3, 0, 1) == 1);
    assert(isSymmetric2Vars(ttAnd3, 1, 0) == 1);
    assert(isSymmetric2Vars(ttAnd3, 0, 2) == 0);
    assert(isSymmetric2Vars(ttAnd3, 1, 2) == 0);
    clearTT(ttAnd3);

    //XOR (2 vars): symmetric in x0,x1
    truthTable *ttXor2 = readTT(tts_xor, 0);
    assert(isSymmetric2Vars(ttXor2, 0, 1) == 1);
    clearTT(ttXor2);

    //f = x0 (2 vars): not symmetric in x0,x1
    truthTable *ttProj2 = readTT(tts_proj, 0);
    assert(isSymmetric2Vars(ttProj2, 0, 1) == 0);
    clearTT(ttProj2);

    //3-input majority: symmetric in all pairs, including reversed indices
    truthTable *ttMaj2 = readTT(tts_maj, 0);
    assert(isSymmetric2Vars(ttMaj2, 0, 1) == 1);
    assert(isSymmetric2Vars(ttMaj2, 0, 2) == 1);
    assert(isSymmetric2Vars(ttMaj2, 1, 2) == 1);
    assert(isSymmetric2Vars(ttMaj2, 1, 0) == 1);
    assert(isSymmetric2Vars(ttMaj2, 2, 0) == 1);
    assert(isSymmetric2Vars(ttMaj2, 2, 1) == 1);
    assert(isSymmetric2Vars(ttMaj2, 0, 0) == 1);
    assert(isSymmetric2Vars(ttMaj2, 2, 2) == 1);
    clearTT(ttMaj2);

    // joint symmetry of two pairs: f = (a + b*x)(c + d*x) with a=0,b=1,x=2,c=3,d=4
    // factor pairs {a,b} and {c,d} are exchangeable, so (a,c)+(b,d) is symmetric,
    // but neither pair is symmetric alone.
    {
        char s[] = "11101010111000001110101000000000";
        truthTable *t = readTT(s, 0);
        assert(isSymmetric2Vars(t, 0, 3) == 0);   // a <-> c alone: no
        assert(isSymmetric2Vars(t, 1, 4) == 0);   // b <-> d alone: no
        assert(isSymmetric2Pairs(t, 0, 3, 1, 4) == 1);  // (a,c)+(b,d): yes
        assert(isSymmetric2Pairs(t, 3, 0, 4, 1) == 1);  // order-independent
        assert(isSymmetric2Pairs(t, 0, 1, 3, 4) == 0);  // (a,b)+(c,d): no
        clearTT(t);
    }

    // invalid inputs: out of range and non-distinct indices return 0
    {
        char s[] = "0110";  // 2 vars
        truthTable *t = readTT(s, 0);
        assert(isSymmetric2Pairs(t, 0, 1, 2, 3) == 0);  // index 2,3 out of range
        assert(isSymmetric2Pairs(t, 0, 1, 0, 1) == 0);  // not pairwise distinct
        clearTT(t);
    }

    return 0;
}
