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

/**
 * \ingroup tt_query
 * Test whether a function is jointly symmetric in two variable pairs.
 *
 * The function is unchanged when both transpositions are applied at once:
 * variable \p a0 is swapped with \p b0 AND variable \p a1 with \p b1
 * simultaneously. Equivalently, the two pairs {a0, a1} and {b0, b1} can be
 * exchanged. This captures symmetries that no single-pair swap reveals -- e.g.
 * f = (a + b*x)(c + d*x) is invariant under (a<->c, b<->d) together, but under
 * neither pair alone.
 *
 * All four indices must be pairwise distinct and in range; otherwise 0 is
 * returned (and a message is printed).
 *
 * \param tt Truth table to test.
 * \param a0 First variable of pair A (0..varNum-1).
 * \param b0 First variable of pair B (0..varNum-1).
 * \param a1 Second variable of pair A (0..varNum-1).
 * \param b1 Second variable of pair B (0..varNum-1).
 * \return 1 if invariant under the simultaneous swaps, 0 otherwise.
 *
 * \see isSymmetric2Vars for single-pair symmetry.
 *      Illustration of the permutation approach:
 *      https://wjrforcyber.github.io/pub/permutation.pdf
 */
int isSymmetric2Pairs(truthTable* tt, int a0, int b0, int a1, int b1);

/**
 * \ingroup tt_query
 * Test whether an arbitrary-length truth table (given as an array) is totally
 * symmetric.
 *
 * \p tt is the function's output column in unpacked form: entry \p i (an `int`,
 * read as `*(int*)fetchIndexArray(tt, i)`) is the output bit (0 or 1) for input
 * assignment \p i, and `tt->size` must be a power of two `2^varNum`. This lifts
 * the 6-variable limit of isSymmetric() — which packs the column into a single
 * word — by spreading it across the array.
 *
 * The array and its entries are neither modified nor freed; ownership stays
 * with the caller.
 *
 * \param tt Output-column array of length 2^varNum (one int per row).
 * \return 1 if totally symmetric, 0 otherwise (including NULL \p tt, an empty
 *         array, or a non-power-of-two length).
 */
int isSymmetricArray(ziArray* tt);

#endif /* YIN_SYMMETRIC_H */
