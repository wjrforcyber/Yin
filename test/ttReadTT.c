#include "tt.h"
#include <assert.h>
#include <string.h>

int main()
{
    char tts[] = "10101100--01";
    char tts_W[] = "10101DEW";
    truthTable *tt = readTT(tts, 1);
    truthTable *ttW = readTT(tts_W, 1);
    assert(tt != NULL);
    assert(ttW == NULL);
    assert(evalTTVal(tt, 0xAC1) == 0);
    assert(evalTTMask(tt, 0xFF3) == 0);
    assert(clearTT(tt) == 0);
    assert(clearTT(ttW) == 0);
    return 0;
}