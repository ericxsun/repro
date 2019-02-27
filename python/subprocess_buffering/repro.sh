#!/bin/bash
set -eu -o pipefail

run() { (set -x; "$@"; ); echo; }

echo -e "-- python 2 --\n"
run python2 --version
# Works fine.
run python2 ./top.py -- bash ./sub.sh
# Fails.
! run python2 ./top.py -- python2 sub.py
# Works fine. (Needs `stdbuf`).
run python2 ./top.py -- stdbuf --output=0 python2 sub.py

echo -e "\n\n-- python 3 --\n"
run python3 --version
# Works fine.
run python3 ./top.py -- bash ./sub.sh
# Fails.
! run python3 ./top.py -- python3 sub.py
# Fails.
! run python3 ./top.py -- stdbuf --output=0 python3 sub.py
# Works.
run env PYTHONUNBUFFERED=1 python3 ./top.py -- python3 sub.py
# Works???
run python3 ./top.py -- env PYTHONUNBUFFERED=1 python3 sub.py

echo "Done"
