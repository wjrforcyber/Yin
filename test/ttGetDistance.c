#include "tt.h"
#include "assert.h"

int main(){
    char tts0[] = "10101100";
    char tts1[] = "10001001";
    truthTable *tt0 = readTT(tts0, 1);
    truthTable *tt1 = readTT(tts1, 1);
    assert(getDistance(tt0, tt1) == 3);
    clearTT(tt0);
    clearTT(tt1);
    return 0;
}