#!/bin/bash

. .python3-venv/bin/activate
cheetah fill - <weekly.tpl.svg >weekly.svg

convert \
    -size 300x300 \
    -density 300x300 \
    -units PixelsPerInch \
    weekly.svg weekly.pdf

