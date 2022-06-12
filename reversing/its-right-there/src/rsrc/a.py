from PIL import Image
import numpy as np

img = Image.open("font-big.png")
img = np.array(img)[:,:,3]
sz = img.shape[0]

f = lambda c: img[
    sz//16*(c//16) : sz//16*(1+c//16),
    sz//16*(c%16) : sz//16*(1+c%16)
]

emp = np.zeros((sz//16*8,sz//16*8), dtype=np.uint8)
chars = [*b"RtEFATqpkVjyMBKnwh25LrYxlbg0vU3}i_fIue8doXC{HWNS9P4Gz6Js7DQcam1O"]
assert len(chars) == 64

nchars = []
for i,c in enumerate(chars):
    if c == ord('_'):
        nchars.append("underscore")
    elif c == ord("{"):
        nchars.append("curlleft")
    elif c == ord("}"):
        nchars.append("curlright")
    else:
        nchars.append(chr(c))
    cc = f(c)
    emp[
        i//8*sz//16:(1+i//8)*sz//16,
        i%8*sz//16:(1+i%8)*sz//16
    ] = cc

Image.fromarray(emp).save("font-min0.jpg")


defines = "\n".join(f"#define _l{c} {i}./16." for i,c in enumerate(nchars))
#open("defines","w").write(str(defines))