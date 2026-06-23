Quick start
===========

Build the library first (requires a recursive clone for the ``Zi`` submodule):

.. code-block:: sh

   git clone --recursive <yin-url>
   cmake -B build && cmake --build build
   ctest --test-dir build        # or: (cd build && ctest)

Construct a truth table, classify it, and parse an expression — all three
examples below are taken from the test suite (``test/``). See the
:doc:`API reference <api>` for the full signatures and ownership rules.

Read a truth table from an MSB-first string
-------------------------------------------

``readTT`` consumes a string of ``0`` / ``1`` / ``-`` (don't-care) whose length is
a power of two. The first character is the **most significant** bit, so
``"0110"`` is 2-input XOR:

.. code-block:: c

   #include "tt.h"

   /* "0110" is XOR; the function is binate in both variables. */
   truthTable* ttXor = readTT("0110", /*verbose=*/1);
   /* ...use ttXor... */
   clearTT(ttXor);

Classify unateness
------------------

The inspection group includes the unateness test. Note that ``isUnate`` calls
``initArray`` on ``res`` itself — do **not** pre-initialize it, and release it
with ``clearEachEntryArray`` afterwards:

.. code-block:: c

   #include "unate.h"

   ziArray record;                 /* uninitialized */
   truthTable* ttXor = readTT("0110", 0);
   enum FUNC_TYPE type = isUnate(ttXor, &record);   /* == BINATE */
   clearEachEntryArray(&record);
   clearTT(ttXor);

Parse and simulate a Boolean expression
---------------------------------------

The expression parser parses an ASCII expression (``~`` / ``*`` / ``+``,
identifiers ``a``–``z``, precedence ``~`` > ``*`` > ``+``) and simulates it into
a truth table:

.. code-block:: c

   #include "parser.h"

   exprNode* e = parseExpr("~(~a * (~b + c))");
   truthTable* sim = simulateExpr(e);   /* == readTT("10101110", 0) */
   clearTT(sim);
   clearExpr(e);
