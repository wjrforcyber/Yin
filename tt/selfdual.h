#pragma once

#include "tt.h"

// Returns 1 if the Boolean function is self-dual, i.e. f(x) = ~f(~x) for every
// input assignment x (equivalently, flipping all inputs and the output leaves f
// unchanged). Supports truth tables with 1..6 variables. Returns 0 otherwise,
// including when tt is NULL or varNum is out of range.
int isSelfDual(truthTable* tt);
