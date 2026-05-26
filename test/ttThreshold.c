#include "threshold.h"
#include <assert.h>

int main(){
    //3-input majority, threshold 2: popcount>=2 outputs 1
    //f(000)=0 f(001)=0 f(010)=0 f(011)=1 f(100)=0 f(101)=1 f(110)=1 f(111)=1
    char tts_maj[] = "11101000";
    truthTable *ttMaj = readTT(tts_maj, 0);
    assert(isThreshold(ttMaj, 2) == 1);
    clearTT(ttMaj);

    //3-input majority, threshold 1: not a match (single-1 rows are 0)
    truthTable *ttMaj2 = readTT(tts_maj, 0);
    assert(isThreshold(ttMaj2, 1) == 0);
    clearTT(ttMaj2);

    //3-input OR with all weights, threshold 1: any input is 1 means output 1
    //f(000)=0 f(001)=1 f(010)=1 f(011)=1 f(100)=1 f(101)=1 f(110)=1 f(111)=1
    char tts_or3[] = "11111110";
    truthTable *ttOr3 = readTT(tts_or3, 0);
    assert(isThreshold(ttOr3, 1) == 1);
    clearTT(ttOr3);

    //2-input AND, threshold 2: popcount>=2 outputs 1
    //f(00)=0 f(01)=0 f(10)=0 f(11)=1
    char tts_and[] = "1000";
    truthTable *ttAnd = readTT(tts_and, 0);
    assert(isThreshold(ttAnd, 2) == 1);
    clearTT(ttAnd);

    //2-input AND is NOT threshold 1 (f(01)=0 but popcount(01)=1>=1)
    truthTable *ttAnd2 = readTT(tts_and, 0);
    assert(isThreshold(ttAnd2, 1) == 0);
    clearTT(ttAnd2);

    //XOR is never a threshold function
    char tts_xor[] = "0110";
    truthTable *ttXor = readTT(tts_xor, 0);
    assert(isThreshold(ttXor, 1) == 0);
    assert(isThreshold(ttXor, 2) == 0);
    clearTT(ttXor);

    //constant 1 (2 vars), threshold 0: popcount>=0 always true
    char tts_one[] = "1111";
    truthTable *ttOne = readTT(tts_one, 0);
    assert(isThreshold(ttOne, 0) == 1);
    clearTT(ttOne);

    //constant 0 (2 vars), threshold 3: popcount>=3 never true for 2 vars
    char tts_zero[] = "0000";
    truthTable *ttZero = readTT(tts_zero, 0);
    assert(isThreshold(ttZero, 3) == 1);
    clearTT(ttZero);

    return 0;
}
