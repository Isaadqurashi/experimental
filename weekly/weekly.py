#!/usr/bin/env python3

import sys
import datetime
from reportlab.lib import colors
from reportlab.lib import pagesizes
from reportlab.pdfgen.canvas import Canvas

y = int(sys.argv[1])

canvas = Canvas(f"weekly_planner_{y}.pdf", pagesize=pagesizes.LETTER)

ppi = 72

d0 = datetime.date(y, 1, 1)

dow0 = d0.weekday()

face = "Helvetica"

for w in range(0, 52 + 1):
    x = ppi
    y = 10 * ppi
    canvas.setFont(face, 18)
    canvas.setFillColor(colors.blue)
    canvas.setStrokeColor(colors.blue)
    canvas.drawString(x, y, f'Week {w+1}')

    p = canvas.beginPath()
    p.moveTo(x, y - .1 * ppi)
    p.lineTo(7 * ppi, y - .1 * ppi)
    p.close()
    canvas.drawPath(p)

    for dow in range(0, 7):
        try:
            d = d0 + datetime.timedelta(days=(7*w+dow - dow0))
            dfmt = d.strftime("%a %d %b")
            y = (10 - (9.5/7.0) * (1+dow)) * ppi

            canvas.setFont(face, 12)
            canvas.setFillColor(colors.gray)
            canvas.drawString(x, y, '{}'.format(d.strftime('%a')))
            canvas.setFont("Courier", 9)
            canvas.drawString(x + .5 * ppi, y, '{}'.format(d.strftime('%d %b')))

            p = canvas.beginPath()
            p.moveTo(x, y - .1 * ppi)
            p.lineTo(7 * ppi, y - .1 * ppi)
            p.close()
            canvas.setStrokeColor(colors.gray)
            canvas.drawPath(p)

        except ValueError:
            pass
    canvas.showPage()

canvas.save()

