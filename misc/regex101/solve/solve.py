import re

matches = []

with open('flags.txt') as f:
    for line in f:
        matches += re.findall('SEE{[A-Z]{5}\d{5}[A-Z]{6}}', line)

for match in matches:
    print(match)
