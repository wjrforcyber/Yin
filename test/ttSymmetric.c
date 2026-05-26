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

    return 0;
}
