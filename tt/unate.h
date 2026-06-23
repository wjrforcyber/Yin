/**
 * \file unate.h
 * \brief Unateness classification of Boolean functions.
 */
#ifndef YIN_UNATE_H
#define YIN_UNATE_H

#include "tt.h"

/** Per-variable and overall unateness classification. */
enum FUNC_TYPE {
    POS_UNATE = 0, ///< Positive unate (monotone non-decreasing).
    NEG_UNATE,     ///< Negative unate (monotone non-increasing).
    BINATE,        ///< Neither positive nor negative unate in the variable.
    IRRELEVANT,    ///< Function does not depend on the variable.
    UNKNOW         ///< Sentinel for an internal error during detection.
};

/**
 * \ingroup tt_query
 * Classify each variable's unateness and the function's overall unateness.
 *
 * For every variable the function is tested for positive unateness
 * (output never decreases when the variable rises 0->1) and negative unateness
 * (output never increases). Per-variable results are appended to \p res, and
 * the function's overall type is derived from them.
 *
 * \param tt Truth table to classify.
 * \param res Uninitialized ziArray. This function calls `initArray(res, 10)`
 *            internally; **do not pre-initialize it**. On success \p res holds
 *            `tt->varNum` entries, each a `malloc`'d `enum FUNC_TYPE*` giving
 *            the variable's classification at its index.
 * \return The function's overall type: #POS_UNATE if positive unate in every
 *         relevant variable, #NEG_UNATE likewise, #BINATE if any variable is
 *         binate, #IRRELEVANT if independent of all variables, or #UNKNOW on an
 *         internal error.
 *
 * \par Ownership
 * Caller must release \p res with clearEachEntryArray() (which frees each
 * `enum FUNC_TYPE*` entry and the array storage).
 */
enum FUNC_TYPE isUnate(truthTable* tt, ziArray* res);

#endif /* YIN_UNATE_H */
