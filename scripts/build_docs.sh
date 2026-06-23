#!/usr/bin/env bash
# Build the Yin documentation site.
#
# Pipeline:  Doxygen (Doxyfile -> docs/_doxygen/xml)  ->  Sphinx (docs/ -> docs/_build/html)
#
# Usage:
#   scripts/build_docs.sh            # build once
#   scripts/build_docs.sh --serve    # build, then serve docs/_build/html on :8000
#   scripts/build_docs.sh --watch    # rebuild on every header change (needs `entr`)
#
# Requires a venv with docs/requirements.txt installed. VENV (default ./.venv)
# may be overridden. Doxygen and Graphviz `dot` must be on PATH.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

VENV="${VENV:-$ROOT/.venv}"
DOXYGEN="${DOXYGEN:-doxygen}"

# Prefer a project venv; fall back to sphinx-build on PATH (e.g. CI installs
# the toolchain into the runner's system Python via setup-python).
if [ -x "$VENV/bin/sphinx-build" ]; then
    SPHINX_BUILD="$VENV/bin/sphinx-build"
elif command -v sphinx-build >/dev/null 2>&1; then
    SPHINX_BUILD="$(command -v sphinx-build)"
else
    echo "error: sphinx-build not found (neither $VENV/bin/sphinx-build nor on PATH)." >&2
    echo "       Create the venv:  python3 -m venv .venv && .venv/bin/pip install -r docs/requirements.txt" >&2
    exit 1
fi

build() {
    echo "==> Doxygen"
    mkdir -p docs/_doxygen
    "$DOXYGEN" Doxyfile
    echo "==> Sphinx"
    "$SPHINX_BUILD" -b html docs docs/_build/html
    echo "==> Done: file://$ROOT/docs/_build/html/index.html"
}

case "${1:-}" in
    --serve)
        build
        echo "==> Serving on http://localhost:8000 (Ctrl-C to stop)"
        (cd docs/_build/html && exec python3 -m http.server 8000)
        ;;
    --watch)
        if ! command -v entr >/dev/null 2>&1; then
            echo "error: --watch requires 'entr' (install entr or use --serve)." >&2
            exit 1
        fi
        build
        echo "==> Watching for changes..."
        ls tt/*.h tt/*.c Doxyfile docs/*.rst docs/conf.py | entr -c "$0"
        ;;
    *)
        build
        ;;
esac
