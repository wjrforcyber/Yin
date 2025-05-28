#include "tt.h"
#include <assert.h>

int main()
{
    truthTable tt;
    assert(initElementaryVar(&tt, 3, 0) == 0);
    assert(tt.ttrep == (maskTT[0] & mask[3]));
    assert(tt.elementary == 1);
    assert(tt.varNum == 3);

    truthTable tt0;
    assert(initElementaryVar(&tt0, 4, 1) == 0);
    showTT(&tt0, HEX);
    assert(tt0.ttrep == (maskTT[1] & mask[4]));
    return 0;
}