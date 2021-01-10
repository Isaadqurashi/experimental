#!/usr/bin/python3

import datetime
import json
import sys

_months = [
    "JANUARY",
    "FEBRUARY",
    "MARCH",
    "APRIL",
    "MAY",
    "JUNE",
    "JULY",
    "AUGUST",
    "SEPTEMBER",
    "OCTOBER",
    "NOVEMBER",
    "DECEMBER",
]

_days = ["MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY", "SUNDAY"]

def _carp(s):
    print(s, file=sys.stderr)


def generate(year):
    d0 = datetime.date(year, 1, 1)

    # back up until we reach a Monday
    while d0.weekday() != 0:
        d0 = d0 - datetime.timedelta(days=1)
        
    #_carp(f'dow:{d0.weekday()}, ')
    #d0 = d0 - datetime.timedelta(days=(6 - (d0.weekday())))

    # Start on Monday even if it's in the previous year.
    d = d0
    doy = 1

    weeks = []

    while d.year < year + 1:
        week = []
        for _ in _days:
            week.append(
                {
                    "Y": d.year,
                    "B": _months[d.month - 1],
                    "d": d.day,
                    "A": _days[d.weekday()],
                    "j": doy,
                }
            )
            d = d + datetime.timedelta(days=1)
            doy = doy + 1
        weeks.append(week)
    return weeks

if __name__ == "__main__":
    year = datetime.datetime.today().year
    if len(sys.argv) > 1:
        year = int(sys.argv[1])
    print(f'# Calendar for {year}:')
    weeks = generate(year)
    print(f'{json.dumps(weeks, indent=2)}')
