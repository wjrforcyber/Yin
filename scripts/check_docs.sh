#!/usr/bin/env bash
# Verify documentation coverage of the public headers.
#
# With EXTRACT_ALL=YES Doxygen emits every symbol whether or not it carries a
# doc comment, so undocumented-symbol warnings do not fire. This lint enforces
# the project's actual bar: every non-static function / struct / enum /
# typedef declaration at file scope in tt/*.h must be preceded by a /** block.
#
# Struct/union/enum MEMBERS are exempt (they use inline `///<` docs) and so are
# `static` helpers/tables. Exits non-zero if any public declaration is undocumented.
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

python3 - <<'PY'
import re, sys
from pathlib import Path

issues = []
FUNC_LEAD = re.compile(r'^(truthTable\*|cube\*|enum\s+\w+|int|void|unsigned|char|size_t)\b')
AGG_OPEN = re.compile(r'^(struct|enum|union)\s+\w+')

for path in sorted(Path("tt").glob("*.h")):
    lines = path.read_text().splitlines()
    depth = 0  # brace depth; >0 means we are inside an aggregate body (members)
    for i, raw in enumerate(lines):
        s = raw.strip()
        # track aggregate body depth from the previous line's braces
        depth += raw.count("{") - raw.count("}")
        if depth < 0:
            depth = 0
        if not s or s.startswith(("#", "//", "/*", "*")):
            continue
        if s.startswith(("static", "typedef")):
            continue
        if depth > 0:
            # we are inside a struct/enum body: members use inline ///< docs
            continue
        is_func = ("(" in s) and (FUNC_LEAD.match(s) is not None)
        is_agg = AGG_OPEN.match(s) is not None and "{" in raw
        if not (is_func or is_agg):
            continue
        if is_func:
            # confirm the declaration terminates with ');' (possibly multi-line)
            j, buf = i, s
            while j < len(lines) and ");" not in buf:
                j += 1
                buf += " " + lines[j].strip()
            if ");" not in buf:
                continue
        # Scan back over blanks and comment lines until we either find a /**
        # opener (documented) or hit a real code line (undocumented).
        found = False
        for k in range(i - 1, -1, -1):
            l = lines[k].strip()
            if not l:
                continue
            if l.startswith("/**"):
                found = True
                break
            if l.startswith(("//", "/*", "*")):
                continue
            break  # a code line breaks the doc attachment
        if not found:
            issues.append(f"{path}:{i+1}: undocumented declaration: {s[:60]}")

if issues:
    print("Missing documentation blocks:", file=sys.stderr)
    for it in issues:
        print("  " + it, file=sys.stderr)
    sys.exit(1)
print("OK: all public declarations in tt/*.h are documented.")
PY
