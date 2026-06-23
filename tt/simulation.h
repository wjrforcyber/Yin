/**
 * \file simulation.h
 * \brief Random simulation-vector generation.
 */
#ifndef YIN_SIMULATION_H
#define YIN_SIMULATION_H

#include "tt.h"

/**
 * \defgroup tt_sim Simulation
 * \brief Generation of evenly-distributed simulation vectors.
 * @{
 */

/**
 * Generate an evenly-spaced set of simulation patterns.
 *
 * Produces \p sizeVec truth tables evenly distributed across the
 * \f$2^{(2^{varNum})}\f$ possible input patterns by stepping through the space
 * with stride \f$2^{(2^{varNum})} / sizeVec\f$. Each pattern is pushed as a
 * `truthTable*` into \p rSimVec.
 *
 * \param varNum Number of variables (0..6).
 * \param rSimVec Caller-initialized ziArray. The caller **must** call
 *                `initArray(rSimVec, ...)` first; this function only appends.
 * \param sizeVec Number of patterns to generate. Must be no greater than
 *                \f$2^{(2^{varNum})}\f$. If equal to it, a warning is printed
 *                (the full exhaustive set is generated).
 *
 * \par Ownership
 * Each appended `truthTable*` is owned by the caller; release \p rSimVec with
 * clearEachEntryArray() (which frees each entry's truth table and the array).
 */
void randSimEven(int varNum, ziArray* rSimVec, unsigned long sizeVec);

/** @} */ /* end tt_sim */

#endif /* YIN_SIMULATION_H */
