#!/usr/bin/env python

import subprocess

INCLUDE_DIR = "../../include"
SOURCE_DIR = "../../src/stella"

subprocess.call(
    'cppcheck --output-file=./scripts/report.xml --enable=all --inconclusive --xml-version=2 \
    -I {} {}'.format(INCLUDE_DIR, SOURCE_DIR)
    , shell=True)

subprocess.call(
    './scripts/cppcheck-htmlreport.py --title=stella --file=./scripts/report.xml --report-dir=html'
    , shell=True)
