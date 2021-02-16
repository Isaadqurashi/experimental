#!/usr/bin/python

# Perform a quick rewrite of
#
#    #define MONGO_LOGV2_DEFAULT_COMPONENT CCCC
#
# into
#
#    LOGV2_SET_COMPONENT_FOR_FILE(CCCC)
#
# further down the file after all includes.

import sys
import re


class FileState:
    def __init__(self, filename):
        self.filename = filename
        self.lines = []
        self.out_lines = []
        self.component = None
        self.first_namespace = None
        self.last_include = None

    def load(self):
        # print(f'filename="{filename}"', file=sys.stderr)
        self.lines = []
        with open(self.filename) as f:
            for i, line in enumerate(f):
                self.lines.append({'i':i, 's':line, 'f':self.filename})

    def analyze(self):
        """ Find last '#include' and first line of real code. """

        component_re = re.compile(r'^\s*#define\s+MONGO_LOGV2_DEFAULT_COMPONENT\s+(.*)\s*$')
        include_re = re.compile(r'^\s*#include.*\s*')
        namespace_re = re.compile(r'^\s*namespace.*\s*')
        if_re = re.compile(r'^\s*#\s*(if|ifdef|ifndef).*\s*')
        endif_re = re.compile(r'^\s*#\s*endif.*\s*')

        if_level = 0

        for i, line in enumerate(self.lines):
            s = line['s']

            this_level = if_level

            if if_re.fullmatch(s):
                if_level = if_level + 1
                this_level = if_level
            if endif_re.fullmatch(s):
                if_level = if_level - 1
            if this_level > 0:
                line['if_level'] = this_level

            m = component_re.fullmatch(s)
            if m:
                self.component = m[1]
                line['component'] = self.component

            if include_re.fullmatch(s):
                line['inc'] = True
                self.last_include = i

            if namespace_re.fullmatch(s):
                line['ns'] = True
                if not self.first_namespace:
                    self.first_namespace = i

        if self.last_include:
            self.lines[self.last_include]['last_include'] = True
            # Find an insert point.
            # This is the first line after the last include that is not in a #if block
            for line in self.lines[self.last_include+1:]:
                if 'if_level' not in line:
                    self.insert_point = line['i']
                    line['insert_point'] = True
                    break

        #if self.first_namespace:
        #    print(f'first_namespace: {self.first_namespace}')

    def edit(self):
        if self.component is None:
            self.out_lines = self.lines
            return
        self.out_lines = list()
        for line in self.lines:
            if 'component' in line:
                continue  # omit the old component definition
            if 'insert_point' in line:
                self.out_lines.append({'s':''})
                self.out_lines.append({'s':f'LOGV2_SET_COMPONENT_FOR_FILE({self.component})\n'})
                self.out_lines.append({'s':''})
            self.out_lines.append(line)

    def report(self):
        for i, line in enumerate(self.out_lines):
            if_level = f'[if_level={line["if_level"]}]' if 'if_level' in line else ""
            inc = '[inc]' if 'inc' in line else ''
            ns = '[ns]' if 'ns' in line else ''
            component = f'[component=\"{line["component"]}\"]' if 'component' in line else ''

            last_include = '[last_include]' if 'last_include' in line else ''
            insert_point = '[insert_point]' if 'insert_point' in line else ''


            print(f'{self.filename}:{i:05d}: {line["s"].rstrip():120s}:{component}{if_level}{inc}{ns}{last_include}{insert_point}')

class LineState:
    pass

def main(argv):
    if len(argv) < 2:
        print(f'Usage: {argv[0]} <filename...>')
        return 1
    for filename in sys.argv[1:]:
        fileState = FileState(filename)
        fileState.load()
        fileState.analyze()
        fileState.edit()
        fileState.report()
    return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv))
