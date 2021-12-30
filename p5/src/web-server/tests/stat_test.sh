#!/bin/bash

tmpfile="tmp.txt"
rm -f "$tmpfile"

python3 "tests/stat_test.py" $@ > "$tmpfile"

if [[ "$TEST_DEBUG_MODE" == "1" ]]; then
    cat "$tmpfile"
fi

cat "$tmpfile" |
# Crop the last 2 lines because output may be interrupted
head -n -2 | awk 'BEGIN {FS=" :? ?"} \
  NF == 4 {totals[$1] = $2; statics[$1] = $3; dynamics[$1] = $4} \
  END { \
    for (x in totals) { total += totals[x]; } \
    for (x in statics) { static += statics[x]; } \
    for (x in dynamics) { dynamic += dynamics[x]; } \
    printf("Total requests: %d; Total static: %d; Total dynamic: %d\n", \
      total, static, dynamic) \
  }'

rm -f "$tmpfile"
