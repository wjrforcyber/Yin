#include <assert.h>
#include "unate.h"

int main(){
    //AND
    ziArray record_and;
    char tts[] = "1000";
    truthTable *tt = readTT(tts, 1);
    enum FUNC_TYPE type_unate_and = isUnate(tt, &record_and);
    assert(type_unate_and == POS_UNATE);
    clearTT(tt);
    clearEachEntryArray(&record_and);
    
    //OR
    ziArray record_or;
    char tts_or[] = "1110";
    truthTable *ttOr = readTT(tts_or, 1);
    enum FUNC_TYPE type_unate_or = isUnate(ttOr, &record_or);
    printf("%dth FUNC_type\n", type_unate_or);
    assert(type_unate_or == POS_UNATE);
    clearTT(ttOr);
    clearEachEntryArray(&record_or);
    
    //NAND
    ziArray record_nand;
    char tts_nand[] = "0111";
    truthTable *ttNAND = readTT(tts_nand, 1);
    enum FUNC_TYPE type_unate_nand = isUnate(ttNAND, &record_nand);
    assert(type_unate_nand == NEG_UNATE);
    clearTT(ttNAND);
    clearEachEntryArray(&record_nand);
    
    //XOR
    ziArray record_xor;
    char tts_xor[] = "0110";
    truthTable *ttXOR = readTT(tts_xor, 1);
    enum FUNC_TYPE type_unate_xor = isUnate(ttXOR, &record_xor);
    assert(type_unate_xor == BINATE);
    clearTT(ttXOR);
    clearEachEntryArray(&record_xor);

    //f = x0 (2 vars): x0 POS_UNATE, x1 does not affect output -> overall POS_UNATE
    ziArray record_proj;
    char tts_proj[] = "1010";
    truthTable *ttProj = readTT(tts_proj, 0);
    enum FUNC_TYPE type_unate_proj = isUnate(ttProj, &record_proj);
    assert(type_unate_proj == POS_UNATE);
    clearTT(ttProj);
    clearEachEntryArray(&record_proj);

    //f = NOT x0 (2 vars): x0 NEG_UNATE, x1 does not affect output -> overall NEG_UNATE
    ziArray record_not;
    char tts_not[] = "0101";
    truthTable *ttNot = readTT(tts_not, 0);
    enum FUNC_TYPE type_unate_not = isUnate(ttNot, &record_not);
    assert(type_unate_not == NEG_UNATE);
    clearTT(ttNot);
    clearEachEntryArray(&record_not);

    //f = x0 AND x1, x2 does not affect output (3 vars) -> overall POS_UNATE
    ziArray record_and3;
    char tts_and3[] = "10001000";
    truthTable *ttAnd3 = readTT(tts_and3, 0);
    enum FUNC_TYPE type_unate_and3 = isUnate(ttAnd3, &record_and3);
    assert(type_unate_and3 == POS_UNATE);
    clearTT(ttAnd3);
    clearEachEntryArray(&record_and3);

    //f = x0 XOR x1, x2 does not affect output (3 vars) -> overall BINATE
    ziArray record_xor3;
    char tts_xor3[] = "01100110";
    truthTable *ttXor3 = readTT(tts_xor3, 0);
    enum FUNC_TYPE type_unate_xor3 = isUnate(ttXor3, &record_xor3);
    assert(type_unate_xor3 == BINATE);
    clearTT(ttXor3);
    clearEachEntryArray(&record_xor3);
    
    return 0;
}
