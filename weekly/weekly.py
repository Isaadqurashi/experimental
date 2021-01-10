#!/usr/bin/env python3

import os
import sys
import regex
import cairo
import cairosvg

from Cheetah.Template import Template

import generate_weeks


class WeeklyPlanner:
    def __init__(self, outDir, year):
        self.outDir = outDir
        self.year = year

    def svgPdf(self, s):
        dpi = 96
        return cairosvg.svg2pdf(
            bytestring=s, dpi=dpi, parent_width=dpi * 5.5, parent_height=dpi * 8.5
        )

    def makeDots(self):
        dotsSvg = open("dots.svg").read()
        dotsPdf = self.svgPdf(dotsSvg)
        open(f"{self.outDir}/dots.pdf", "wb").write(dotsPdf)

    def makeWeeks(self, weeks):
        tmpl = open("weekly.tpl.svg").read()
        for wi, w in enumerate(weeks):
            # strftime formats: {%B: MONTH, %d: 31, %A: DAYOFWEEK}
            ww = w[0]
            weekOf = f'Week of {ww["B"]} {ww["d"]}, {ww["Y"]}:'
            t = Template(tmpl, searchList={"wHead": ww, "weekOf": weekOf, "days": w})
            weekPdf = self.svgPdf(str(t))
            open(f"{self.outDir}/week_{wi:02d}.pdf", "wb").write(weekPdf)

    def generate(self):
        try:
            os.mkdir(self.outDir)
        except FileExistsError:
            pass
        self.makeDots()
        weeks = generate_weeks.generate(self.year)
        self.makeWeeks(weeks)

    def collate(self):
        pass


if __name__ == "__main__":
    year = int(sys.argv[1])
    WeeklyPlanner(f"year_{year:04d}", year).generate()
