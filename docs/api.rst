API reference
=============

The interface is organised into modules. Each links to the Doxygen-generated
reference for its types and functions.

Core representation
-------------------

Construction, display, and lifetime management of truth tables and cubes.

.. doxygengroup:: tt_core
   :project: yin

Boolean operations
------------------

Cofactoring, logical connectives (``not`` / ``and`` / ``or`` / ``xor``), and the
``zero`` / ``one`` constants. Every function returns a freshly allocated table
except :c:func:`notInplace`.

.. doxygengroup:: tt_logic
   :project: yin

Inspection
----------

Structural queries: constant-ness, variable dependence, weight, Hamming
distance, unateness, symmetry, and threshold-function tests.

.. doxygengroup:: tt_query
   :project: yin

Irredundant sum-of-products
---------------------------

ISOP cover generation (Minato-Morreale) and the ``cube`` literal helpers.

.. doxygengroup:: tt_isop
   :project: yin

Expression parser
-----------------

Parsing of Boolean expressions into an AST and exact symbolic simulation.

.. doxygengroup:: tt_parser
   :project: yin

Simulation
----------

Evenly-distributed simulation-vector generation.

.. doxygengroup:: tt_sim
   :project: yin
