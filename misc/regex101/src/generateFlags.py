import random, string

flagStart = 'SEE{'
flagEnd = '}'

with open('flags.txt', 'w') as f:
    for flagCount in range(2999):
        if (flagCount == 369):
            firstPart = ''.join(random.choices(string.ascii_uppercase, k=5))
            secondPart = ''.join(random.choices(string.digits, k=5))
            thirdPart = ''.join(random.choices(string.ascii_uppercase, k=6))
            flagContent = firstPart + secondPart + thirdPart
        else:            
            flagContent = ''.join(random.choices(string.ascii_uppercase + string.digits, k=16))

        f.write(flagStart + flagContent + flagEnd)
        f.write('\n')
