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
    return 0;
    
    //NAND
    ziArray record_nand;
    char tts_nand[] = "0111";
    truthTable *ttNAND = readTT(tts_nand, 1);
    enum FUNC_TYPE type_unate_nand = isUnate(ttNAND, &record_nand);
    assert(type_unate_nand == NEG_UNATE);
    clearTT(tt);
    clearEachEntryArray(&record_nand);
    
    //XOR
    ziArray record_xor;
    char tts_xor[] = "0110";
    truthTable *ttXOR = readTT(tts_xor, 1);
    enum FUNC_TYPE type_unate_xor = isUnate(ttXOR, &record_xor);
    assert(type_unate_xor == BINATE);
    clearEachEntryArray(&record_xor);
    
    return 0;
}