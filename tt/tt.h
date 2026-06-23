/**
 * \file tt.h
 * \brief Core truth-table representation and operations for the Yin library.
 *
 * Defines the packed bit-level truth table (`truthTable`), the cube/implicant
 * type (`cube`), and the construction, Boolean, and inspection routines
 * operating on them. Truth tables hold at most 6 variables, packed into a
 * single `unsigned long`.
 */
#ifndef YIN_TT_H
#define YIN_TT_H

#include "array.h"

/** Packed truth-table word. Bit \f$i\f$ is the function value at input
 *  assignment \f$i\f$ (see \ref truthTable). */
typedef unsigned long tt;

/** Opaque, packed Boolean truth table. */
typedef struct truthTable_ truthTable;

/** Sum-of-products literal cube (product term). */
typedef struct cube_ cube;

/** Display bases accepted by showTT(). Only BDEC and BHEX are rendered. */
enum BASE { BBIN, BOCT, BDEC, BHEX };

/*
 * Internal lookup tables used by cofactor extraction, elementary-variable
 * construction, and the partial-symmetry test. Declared `static` so each
 * translation unit gets its own copy; they are implementation detail and not
 * part of the documented API. Sized for exactly 6 variables.
 */
static tt maskTT[] = {0xAAAAAAAAAAAAAAAA, 0xCCCCCCCCCCCCCCCC,
                      0xF0F0F0F0F0F0F0F0, 0xFF00FF00FF00FF00,
                      0xFFFF0000FFFF0000, 0xFFFFFFFF00000000};

static tt maskTTNeg[] = {0x5555555555555555, 0x3333333333333333,
                         0x0F0F0F0F0F0F0F0F, 0x00FF00FF00FF00FF,
                         0x0000FFFF0000FFFF, 0x00000000FFFFFFFF};

static tt mask[] = {0x0000000000000001, 0x0000000000000003, 0x000000000000000F,
                    0x00000000000000FF, 0x000000000000FFFF, 0x00000000FFFFFFFF,
                    0xFFFFFFFFFFFFFFFF};

/**
 * \struct truthTable_
 * \brief Packed truth table of at most 6 Boolean variables.
 *
 * The function value for input assignment \f$i \in [0, 2^{varNum})\f$ is stored
 * in bit \f$i\f$ of #truthTable_::ttrep, with **variable 0 as the least
 * significant bit**. The leftmost character of a readTT() string therefore
 * corresponds to the most significant bit of #truthTable_::ttrep.
 */
struct truthTable_ {
    int varNum;           ///< Number of variables (0..6).
    int elementary;       ///< 1 if this is an elementary (projection) variable.
    unsigned long ttrep;  ///< Packed function values; bit i = f(input i).
    unsigned long
        maskSpec;  ///< Bit i = 1 where the value is specified, 0 = don't-care.
};

/**
 * \struct cube_
 * \brief Product term (conjunction of literals) used by ISOP cover generation.
 *
 * For literal index \f$j\f$ (variable \f$j\f$, with variable 0 as LSB):
 * #cube_::mask bit \f$j\f$ marks whether the literal is present, and
 * #cube_::polarity bit \f$j\f$ gives its polarity (1 = positive, 0 = negated).
 */
struct cube_ {
    int varNum;              ///< Number of variables.
    unsigned long polarity;  ///< Per-literal polarity (bit j: 1 = positive).
    unsigned long mask;      ///< Per-literal presence (bit j: 1 = present).
};

/**
 * \defgroup tt_core Core representation
 * \brief Construction, display, and lifetime management of truth tables.
 * @{
 */

/**
 * Allocate an empty truth table with the given variable count.
 *
 * The returned table has #truthTable_::elementary set to 0 and both
 * #truthTable_::ttrep and #truthTable_::maskSpec cleared.
 *
 * \param varNum Number of variables; must be in 0..6.
 * \return A freshly `malloc`'d truth table, or NULL if \p varNum exceeds 6.
 *
 * \par Ownership
 * Caller owns the result and must release it with clearTT().
 */
truthTable* initTT(int varNum);

/**
 * Allocate an empty cube matching a truth table's variable count and mask.
 *
 * \param tt Truth table to derive the cube's varNum and mask shape from.
 * \return A freshly `malloc`'d cube with both polarity and mask cleared.
 *
 * \par Ownership
 * There is no dedicated cube destructor; free the result with `free()` (or
 * reuse clearTT(), which is a typed `free`). Cubes accumulated by isop() into a
 * ziArray are likewise owned by the caller.
 */
cube* initCube(truthTable* tt);

/**
 * Configure a truth table as an elementary (projection) variable.
 *
 * After this call #truthTable_::elementary is 1, #truthTable_::ttrep holds the
 * alternating mask for variable \p ith masked to \p varNum bits, and
 * #truthTable_::maskSpec covers the full \f$2^{varNum}\f$ range.
 *
 * \param a Table to configure in place.
 * \param varNum Number of variables; must be in 0..6.
 * \param ith Variable index to project (0..varNum-1).
 * \return 0 on success, 1 if \p varNum exceeds 6 (table left unchanged).
 */
int initElementaryVar(truthTable* a, int varNum, int ith);

/**
 * Parse a truth table from a string specification.
 *
 * \param tts String of `'0'`, `'1'`, and `'-'` (don't-care). Its length must be
 *            a power of two in 2..64. Characters are consumed **most
 *            significant bit first**: the first char maps to the top row of the
 *            truth table (highest bit of #truthTable_::ttrep). Examples:
 *            `"0110"` is 2-input XOR, `"1000"` is 2-input AND.
 * \param verbose If non-zero, print the parsed table via showTT(BHEX).
 * \return A freshly `malloc`'d truth table, or NULL on an unknown character
 *         (in which case any partial allocation is already released).
 *
 * \par Ownership
 * Caller owns the result and must release it with clearTT().
 */
truthTable* readTT(char* tts, int verbose);

/**
 * Test whether the packed representation equals a given value.
 * \param a Table to inspect.
 * \param val Value to compare against #truthTable_::ttrep.
 * \return 0 if #truthTable_::ttrep == \p val, 1 otherwise.
 */
int evalTTVal(truthTable* a, unsigned int val);

/**
 * Test whether the specified-bit mask equals a given value.
 * \param a Table to inspect.
 * \param val Value to compare against #truthTable_::maskSpec.
 * \return 0 if #truthTable_::maskSpec == \p val, 1 otherwise.
 */
int evalTTMask(truthTable* a, unsigned int val);

/**
 * Print a truth table's packed value to stdout.
 *
 * Only the #BDEC and #BHEX bases produce numeric output; #BBIN and #BOCT emit
 * only the heading line.
 *
 * \param a Table to display.
 * \param b Output base.
 */
void showTT(truthTable* a, enum BASE b);

/**
 * Release a truth table.
 * \param a Table previously obtained from this API. Passing a cube* is also
 *          valid since this is a typed `free`.
 * \return Always 0.
 */
int clearTT(truthTable* a);

/** @} */ /* end tt_core */

/**
 * \defgroup tt_logic Boolean operations
 * \brief Cofactoring, logical connectives, and constants on truth tables.
 *
 * All functions in this group return freshly allocated truth tables except
 * notInplace(), which mutates its argument. Operands must share the same
 * variable count and specified-bit mask; mismatches print a warning and the
 * result is computed from the first operand's shape.
 * @{
 */

/**
 * Cofactor of \p a with variable \p index fixed to 0.
 * \param a Source table.
 * \param index Variable index to fix (0..varNum-1).
 * \return Freshly allocated cofactor (caller frees with clearTT()).
 */
truthTable* cofactor0(truthTable* a, int index);

/**
 * Cofactor of \p a with variable \p index fixed to 1.
 * \param a Source table.
 * \param index Variable index to fix (0..varNum-1).
 * \return Freshly allocated cofactor (caller frees with clearTT()).
 */
truthTable* cofactor1(truthTable* a, int index);

/**
 * Logical NOT, allocating a new table.
 * \param tt Source table.
 * \return New table whose #truthTable_::ttrep is `~ttrep & maskSpec`.
 */
truthTable* notCopy(truthTable* tt);

/**
 * Logical NOT, mutating the table in place.
 * \param tt Table to complement; #truthTable_::ttrep is replaced by
 *           `~ttrep & maskSpec`.
 */
void notInplace(truthTable* tt);

/**
 * Bitwise AND (product) of two tables.
 * \param tt0 First operand.
 * \param tt1 Second operand (same varNum and maskSpec as \p tt0).
 * \return New table with `tt0->ttrep & tt1->ttrep`.
 */
truthTable* andCopy(truthTable* tt0, truthTable* tt1);

/**
 * Bitwise OR (sum) of two tables.
 * \param tt0 First operand.
 * \param tt1 Second operand (same varNum and maskSpec as \p tt0).
 * \return New table with `tt0->ttrep | tt1->ttrep`.
 */
truthTable* orCopy(truthTable* tt0, truthTable* tt1);

/**
 * Bitwise XOR of two tables.
 * \param tt0 First operand.
 * \param tt1 Second operand (same varNum and maskSpec as \p tt0).
 * \return New table with `tt0->ttrep ^ tt1->ttrep`.
 */
truthTable* xorCopy(truthTable* tt0, truthTable* tt1);

/**
 * Constant-0 table matching \p tt's shape.
 * \param tt Template for variable count and mask.
 * \return New table with #truthTable_::ttrep cleared.
 */
truthTable* zero(truthTable* tt);

/**
 * Constant-1 table matching \p tt's shape.
 * \param tt Template for variable count and mask.
 * \return New table with #truthTable_::ttrep set to all ones within maskSpec.
 */
truthTable* one(truthTable* tt);

/** @} */ /* end tt_logic */

/**
 * \defgroup tt_query Inspection
 * \brief Structural queries over a truth table.
 * @{
 */

/**
 * Classify a table as constant, ignoring don't-cares.
 * \param tt Table to inspect.
 * \return 0 if all specified bits are 0, 1 if all specified bits are 1, -1
 *         otherwise.
 */
int isConst(truthTable* tt);

/**
 * Test whether the function depends on a given variable.
 * \param tt Table to inspect.
 * \param index Variable index (0..varNum-1).
 * \return Non-zero if cofactoring on \p index changes the function.
 */
int hasVar(truthTable* tt, int index);

/**
 * Hamming weight (number of 1 bits) of a table.
 * \param tt Table to inspect.
 * \return Population count of #truthTable_::ttrep.
 */
int getWeight(truthTable* tt);

/**
 * Hamming distance between two tables.
 * \param tt0 First table.
 * \param tt1 Second table (same varNum as \p tt0).
 * \return Number of differing bits, or -1 if the variable counts differ.
 */
int getDistance(truthTable* tt0, truthTable* tt1);

/** @} */ /* end tt_query */

/**
 * \defgroup tt_isop Irredundant sum-of-products
 * \brief ISOP cover generation (Minato-Morreale algorithm) and cube helpers.
 * @{
 */

/**
 * Append or modify a literal within a cube.
 * \param cube Cube to mutate.
 * \param varCur Variable index of the literal.
 * \param polarity 1 for a positive literal, 0 for a negated literal.
 */
void addLit(cube* cube, int varCur, int polarity);

/**
 * Generate an irredundant sum-of-products cover via the Minato-Morreale
 * algorithm, recursing on variables from \p varIndex down to 0.
 *
 * \param tt On-set truth table to cover.
 * \param dc Don't-care set; positions set here may be included or excluded.
 * \param varIndex One plus the highest variable index to consider
 *                 (typically `tt->varNum`).
 * \param cubes Caller-owned ziArray that receives each product term as a
 *              `cube*`. Only the cubes newly appended during this call carry
 *              the literal for the recursion variable; literals for the
 *              remaining variables are added by the recursive calls.
 * \return A truth table reproducing the generated cover, with don't-cares
 *         resolved wherever the cover is defined.
 *
 * \par Ownership
 * The returned table is freshly allocated (clearTT() it). The caller owns
 * every `cube*` appended to \p cubes and must free each entry.
 */
truthTable* isop(truthTable* tt, truthTable* dc, int varIndex, ziArray* cubes);

/** @} */ /* end tt_isop */

#endif /* YIN_TT_H */
