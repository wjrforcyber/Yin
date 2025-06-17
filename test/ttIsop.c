#include "tt.h"
#include <assert.h>
#include <string.h>

int main()
{
    char tts[] = "11101000";
    truthTable *tt = readTT(tts, 1);
    assert(tt->varNum = 3);
    ziArray cubes;
    initArray(&cubes, 10);
    truthTable * res = isop(tt, tt->varNum, &cubes);
    printf("Size of the cube is %d\n",cubes.size);
    printf("Show the cubes: \n");
    for(int i = 0; i < cubes.size; i++)
    {
        cube * c = (cube*)fetchIndexArray(&cubes, i);
        printf("%lu, %lu, %d\n", c->mask, c->polarity, c->varNum);
    }
    
    printf("Show truth table:\n");
    showTT(res, BHEX);
    assert(cubes.size == 3);
    assert(clearTT(tt) == 0);
    assert(clearTT(res) == 0);
    assert(clearEachEntryArray(&cubes) == 0);
    return 0;
}