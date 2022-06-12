# pypy go brrrr
# pypy solve.py

import sys
sys.path.append("../distrib")

from collections import Counter
from rc4 import *

ct = open("../distrib/ct", "rb").read()
flag_len = len(ct)//0x100000
ct = [ct[i*flag_len:(i+1)*flag_len] for i in range(len(ct)//flag_len)]

samples = [rc4(gen_rand_key(), b"\0"*flag_len) for _ in range(0x1000000)]
pos_flag = b"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_"

flag = []
for idx in range(flag_len):
    
    c = Counter([i[idx] for i in samples])
    y = [c[i] if i in c else 0 for i in range(0x100)]
    k = y.index(max(y))

    c = Counter([i[idx]^k for i in ct])
    p = [(c[f], chr(f)) for f in pos_flag]
    p = [y for _,y in sorted(p, key=lambda x:-x[0])]

    # Print top 4 candidates
    print(" ".join(p[:4]), idx)
    flag.append(p[0])

# One or two chars might be wrong, but can be guessed
#
#            .---(should be 4)
#     _______v__________________________________
# SEE{Lo0K_rcf_w4s_Writt3n_wh3n_n0body_kn3w_sh1t}
print(f"SEE{{{''.join(flag)}}}")