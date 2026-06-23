#pragma once

#include "tt.h"

/**
 * \ingroup tt_query
 * Test whether a Boolean function is self-dual.
 *
 * A function is self-dual when \f$f(x) = \neg f(\neg x)\f$ for every input
 * assignment \f$x\f$ — equivalently, flipping every input bit and the output
 * bit leaves \f$f\f$ unchanged.
 *
 * \param tt Truth table to test (1..6 variables).
 * \return 1 if \p tt is self-dual; 0 otherwise, including when \p tt is NULL or
 *         its variable count is out of range.
 */
int isSelfDual(truthTable* tt);
