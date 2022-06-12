a = open("rsrc/ps30.asm").read().split("\n")
b = open("rsrc/ps30 copy.asm").read().split("\n")

d = [(i,x,y) for i,(x,y) in enumerate(zip(a,b)) if x!=y]

#import IPython; IPython.embed()

for i,x,y in d:
    print(f"Line {i}:")
    print(f"  Orig:", y)
    print(f"  Reco:", x)
    print()