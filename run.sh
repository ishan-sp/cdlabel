#!/usr/bin/env bash
set -euo pipefail

SOURCE=${1:-tests/test_multiply.c}
BASE=$(basename "$SOURCE" .c)
OUT="artifacts/${BASE}_san"

if [ ! -f "$SOURCE" ]; then
  echo "Source file not found: $SOURCE"
  exit 1
fi

./bin/numsan "$SOURCE" -o "$OUT"
"./$OUT"
