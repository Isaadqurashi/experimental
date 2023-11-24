#!/bin/bash
year=${1:-2022}
. .python3-venv/bin/activate
python weekly.py $year
