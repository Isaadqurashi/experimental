#!/bin/bash

. .python3-venv/bin/activate

cairosvg dots.svg -o dots.pdf

python weekly.py 2021
