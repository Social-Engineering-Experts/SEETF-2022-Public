# Regex101 - Solution

**Author**: Gelos

**Category**: Misc

The goal of this challenge is to test the basics of Regular Expression.

The difficulty of this challenge could be slightly increased by forming a more complicated flag, thereby requiring more complicated regular expressions.

## Solution

Since we know that the correct is formed using 5 uppercase letters, 5 digits and 5 uppercase letters, we are able to easily form a RegEx to use.

```regex
SEE{[A-Z]{5}\d{5}[A-Z]{6}}
```

```python
import re

matches = []

with open('flags.txt') as f:
    for line in f:
        matches += re.findall('SEE{[A-Z]{5}\d{5}[A-Z]{6}}', line)

for match in matches:
    print(match)
```

We should get the output of `SEE{RGSXG13841KLWIUO}`.
