import sys
sys.path.append("../distrib")

from ecc import *
from out import *

ax,ay = alice_pub
blake_pub = Point(curve, *blake_pub)

_.<x> = PolynomialRing(ZZ)
R.<isqrta> = GF(p).extension(x*x + a)
alice_priv = discrete_log(ax + isqrta*ay, gx + isqrta*gy)
shared =  blake_pub * int(alice_priv)

print("Flag:", decrypt(shared, ct).decode())