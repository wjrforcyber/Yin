/**
 * \file threshold.h
 * \brief Threshold-function test.
 */
#ifndef YIN_THRESHOLD_H
#define YIN_THRESHOLD_H

#include "tt.h"

/**
 * \ingroup tt_query
 * Test whether a function is a threshold function of a given weight.
 *
 * Returns 1 iff the function's value equals 1 exactly on the input assignments
 * whose Hamming weight (number of 1 bits) is at least \p iT, and 0 elsewhere.
 * For example, a 3-input majority function with \p iT = 2 is a threshold
 * function.
 *
 * \param tt Truth table to test.
 * \param iT Threshold weight.
 * \return 1 if \p tt matches the threshold function for weight \p iT, else 0.
 */
int isThreshold(truthTable* tt, int iT);

#endif /* YIN_THRESHOLD_H */
