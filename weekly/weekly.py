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


    # Emit 4-page groups: {FR:0,BL:N-1,BR:N-2,FL:1}
    # {01,04,03,02}
    # {01,08,07,02},{03,06,05,04}
    # {01,12,11,02},{03,10,09,04},{05,08,07,06}
    def bookletPermutation(self, pages, fill, fc, bc):
        while (len(pages) + 2) % 4 != 0:
            pages.append(fill) # fill to get to mod 4 w/covers
        tmp = []
        tmp.append(fc)
        for pi, p in enumerate(pages):
            phase = pi % 4
            if phase == 0 or phase == 3:
                tmp.append(pages.pop(0)) # front
            elif phase == 1 or phase == 2:
                tmp.append(pages.pop()) # back
        tmp.append(bc)
        return tmp

    def collate(self, weeks):
        pfm = PyPDF2.PdfFileMerger()

        pageFiles = []
        for w in weeks:
            pageFiles.append(w['file'])
            pageFiles.append(self.dots)
        pageFiles = bookletPermutation(pageFiles, fill=self.dots, fc=self.dots, bc=self.dots)

        for pg, p in enumerate(pageFiles):
            pfm.merge(pg, p)

        pfm.write(f'{self.outDir}/year_{year:04d}.pdf')

if __name__ == "__main__":
    year = int(sys.argv[1])
    planner = WeeklyPlanner(f"year_{year:04d}", year)
    weeks = planner.generate()
    planner.collate(weeks)
