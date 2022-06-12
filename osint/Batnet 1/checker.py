print("Flag checker for Batnet 1")

FLAG_PARTS = [
    'SEE{br0th3r',
    '_3y3_help_m3',
    '_f1nd_0ut',
    '_ab0ut_3very0ne}'
]

for part in range(len(FLAG_PARTS)):
    print(f"Part {part + 1} of {len(FLAG_PARTS)}:")
    attempt = input("> ")

    if attempt == FLAG_PARTS[part]:
        print("Correct!")
    
    else:
        print("Incorrect!")
