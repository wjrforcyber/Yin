/**
 * \file symmetric.h
 * \brief Total and partial symmetry tests for Boolean functions.
 */
#ifndef YIN_SYMMETRIC_H
#define YIN_SYMMETRIC_H

#include "tt.h"

/**
 * \ingroup tt_query
 * Test whether a function is totally symmetric.
 *
 * A totally symmetric function depends only on the Hamming weight of its
 * inputs, i.e. swapping any variables leaves the truth table unchanged.
 *
 * \param tt Truth table to test.
 * \return 1 if totally symmetric, 0 otherwise.
 */
int isSymmetric(truthTable* tt);

/**
 * \ingroup tt_query
 * Test whether a function is symmetric in two given variables.
 *
 * A function is symmetric in variables \p varIndex0 and \p varIndex1 if
 * swapping the two variables leaves the truth table unchanged.
 *
 * \param tt Truth table to test.
 * \param varIndex0 First variable index (0..varNum-1).
 * \param varIndex1 Second variable index (0..varNum-1).
 * \return 1 if symmetric in the two variables (or if the indices coincide,
 *         which prints a warning as a trivial case), 0 otherwise.
 *
 * \see Illustration of the permutation approach:
 *      https://wjrforcyber.github.io/pub/permutation.pdf
 */
int isSymmetric2Vars(truthTable* tt, int varIndex0, int varIndex1);

#endif /* YIN_SYMMETRIC_H */
