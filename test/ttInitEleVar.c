#include "tt.h"
#include <assert.h>

int main()
{
    truthTable tt;
    assert(initElementaryVar(&tt, 3, 0) == 0);
    assert(tt.ttrep == (maskTT[0] & mask[3]));
    assert(tt.elementary == 1);
    assert(tt.varNum == 3);
    return 0;
}