from libnum import n2s, s2n
from pwn import *
from math import gcd
import gmpy2

def generateRelatedMessages():
    start = s2n(b"SEE{xxx") # add some x to give more padding for the cubic root
    curr_bitlength = start.bit_length()

    # add 7 to give some padding, this number can be tweaked
    start <<= (1600+7 - curr_bitlength)
    small_msg = int(gmpy2.root(start,3))+1
    large_msg = small_msg**3


    assert large_msg.bit_length() > 1600
    assert n2s(large_msg).startswith(b'SEE{')
    assert 500 < small_msg.bit_length() <=600

    return large_msg, small_msg

m1,m2 = generateRelatedMessages()

def collectMultiplesOfModulus():
    # p = remote('localhost', 1337)
    p = remote('fun.chall.seetf.sg', 30004)
    # nc fun.chall.seetf.sg 30004
    p.recvuntil(b'(as integer) :')
    p.sendline(str(m1).encode('utf-8'))
    p.recvuntil(b'(as integer) :')
    p.sendline(str(m2).encode('utf-8'))
    p.recvuntil(b'Ciphers: \n')

    line = p.recvline()
    c1,c2 = eval(line)

    num1 = c2**3 - c1
    p.close()
    return num1

#collect 4 multiples of modulus to be safe, if you're lucky 2 is sufficient
modulus_multiples = [collectMultiplesOfModulus() for _ in range(4)]

modulus = gcd(*modulus_multiples)
print("Modulus found:", modulus)

#Square root the modulus multiple times until flag is valid
gmpy2.get_context().precision=3000 #this line is necessary to make sure no bytes are lost
while True:
    flag = int(gmpy2.root(modulus,2))
    try:
        print("Flag:", n2s(flag).decode('utf-8'))
        break
    except:
        modulus = flag
