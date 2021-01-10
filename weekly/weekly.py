#!/usr/bin/env python3

import os
import sys
import regex
import cairosvg

from Cheetah.Template import Template
from reportlab.lib import colors
from reportlab.lib import pagesizes

import generate_weeks

y = int(sys.argv[1])

template = None
with open('weekly.tpl.svg') as f:
    template = f.read()

weeks = generate_weeks.generate(y)

try:
    os.mkdir(f'year_{y:04d}')
except FileExistsError:
    pass

# strftime formats:
#   %B: MONTH
#   %d: 31
#   %A: DAYOFWEEK
for wi, w in enumerate(weeks):
    ww = w[0]
    weekOf = f'Week of {ww["B"]} {ww["d"]}, {ww["Y"]}:'
    t = Template(template, searchList={'wHead': ww, 'weekOf': weekOf, 'days': w})

    svgBase = f'year_{y:04d}/week_{wi:02d}'
    with open(f'{svgBase}.svg', 'w') as fout:
        print(t, file=fout)
    cairosvg.svg2pdf(url=f'{svgBase}.svg', write_to=f'{svgBase}.pdf')
