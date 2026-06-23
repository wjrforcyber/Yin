Yin
===========

**Yin** — named after 寅虎 — is the bit-level Boolean **truth-table** library of the
Zodiac Program: synthesis and verification primitives that operate on packed
truth tables of up to six variables.

The library is pure C, built with CMake, and depends on the
`Zi <https://github.com/wjrforcyber/Zi>`__ submodule (the ``ziArray`` container).
Every ``truthTable*`` / ``cube*`` returned by this API is heap-allocated and must
be released with :c:func:`clearTT`; every ``ziArray`` follows
``initArray`` → ``clearEachEntryArray``.

.. toctree::
   :maxdepth: 2
   :caption: Contents

   quickstart
   api

Indices
-------

* :ref:`genindex`
* :ref:`search`
