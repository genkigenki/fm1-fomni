#!/bin/sh
# SPDX-License-Identifier: GPL-3.0-only
# Copyright (C) 2026 M. Geisler (genkigenki)
# FoMni-1 generated headers for the Cardputer build (as host/build_host.sh makes them), into build/gen.
# Python with Pillow + raqm (on Windows: in a Linux container, see CARDPUTER.md).
set -e
cd "$(dirname "$0")/../.."
PY="${PYTHON:-python3}"
mkdir -p build/gen
[ -f build/gen/felucca_font.h ] || "$PY" tools/gen_font.py build/gen/felucca_font.h >/dev/null
"$PY" tools/gen_words.py firmware/words.tsv web/words build/gen/ann_words.h >/dev/null
ls build/gen
