#!/usr/bin/python3

import datetime
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
                    "dow": _days[d.weekday()],
                    "year": d.year,
                    "month": _months[d.month - 1],
                    "day": d.day,
                    "doy": doy,
                }
            )
            d = d + datetime.timedelta(days=1)
            doy = doy + 1
        weeks.append(week)
    return weeks

def main():
    year = int(sys.argv[1])
    weeks = generate(year)
    for w in weeks:
        for d in w:
            _carp(f'['\
f'{d["month"]:9}|'\
f'{d["day"]:2}|'\
f'{d["dow"]:9}'\
']')
        _carp('')

if __name__ == "__main__":
    # execute only if run as a script
    main()
