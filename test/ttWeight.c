#include "tt.h"
#include "assert.h"

int main(){
    char tts[] = "10101100";
    truthTable *tt = readTT(tts, 1);
    assert(getWeight(tt) == 4);
    clearTT(tt);
    return 0;
}
