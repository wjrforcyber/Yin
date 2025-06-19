#include <assert.h>
#include "simulation.h"

int main(){
    ziArray rSimVec;
    initArray(&rSimVec, 10);

    randSimEven(4, &rSimVec, 1 << 5);

    assert(((truthTable *)fetchIndexArray(&rSimVec, 2))->ttrep == ((1 << (1 << 4))/32 * 2));
    assert(((truthTable *)fetchIndexArray(&rSimVec, 6))->ttrep == ((1 << (1 << 4))/32 * 6));

    clearEachEntryArray(&rSimVec);
    return 0;
}
