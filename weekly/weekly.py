#!/usr/bin/env python3

import os
import sys
import regex
import cairo
import cairosvg
import PyPDF2

from Cheetah.Template import Template

import generate_weeks


class WeeklyPlanner:
    def __init__(self, outDir, year):
        self.outDir = outDir
        self.year = year
        self.dpi = 96
        self.w = 5.5
        self.h = 8.5

    def svgPdf(self, s):
        return cairosvg.svg2pdf(
            bytestring=s,
            dpi=self.dpi,
            parent_width=self.dpi * self.w,
            parent_height=self.dpi * self.h,
        )

    def makeDots(self):
        dotsSvg = open("dots.svg").read()
        dotsPdf = self.svgPdf(dotsSvg)
        self.dots = f'{self.outDir}/dots.pdf'
        open(self.dots, "wb").write(dotsPdf)

    def makeWeeks(self, weeks):
        tmpl = open("weekly.tpl.svg").read()
        for wi, w in enumerate(weeks):
            # strftime formats: {%B: MONTH, %d: 31, %A: DAYOFWEEK}
            ww = w['days'][0]
            weekOf = f'Week of {ww["B"]} {ww["d"]}, {ww["Y"]}:'
            t = Template(tmpl, searchList={"wHead": ww, "weekOf": weekOf, "days": w['days']})
            weekPdf = self.svgPdf(str(t))
            w['pdf'] = weekPdf
            w['file'] = f"{self.outDir}/week_{wi:02d}.pdf"
            open(w['file'], "wb").write(weekPdf)
        return weeks

    def generate(self):
        try:
            os.mkdir(self.outDir)
        except FileExistsError:
            pass
        self.makeDots()
        weeks = [{'days':w} for w in generate_weeks.generate(self.year)]
        return self.makeWeeks(weeks)

    def collate(self, weeks):
        pfm = PyPDF2.PdfFileMerger()

        pageFiles = []
        pageFiles.append(self.dots) # front cover
        for w in weeks:
            pageFiles.append(w['file'])
            pageFiles.append(self.dots)
        pageFiles.append(self.dots) # back cover

        # arrange as booklet
        if False:
            ei = len(pageFiles)
            bi = 0
            booklet = []
            while bi != ei:
                booklet.append(pageFiles[ei - 1])
                ei -= 1
                booklet.append(pageFiles[bi])
                bi += 1
        else:
            booklet = pageFiles

        pg = 0
        for p in booklet:
            pfm.merge(pg, p)
            pg = pg + 1

        pfm.write(f'{self.outDir}/year_{year:04d}.pdf')

if __name__ == "__main__":
    year = int(sys.argv[1])
    planner = WeeklyPlanner(f"year_{year:04d}", year)
    weeks = planner.generate()
    planner.collate(weeks)
