from random import randint

p = 2 ** 255 - 19 # everyone's favourite prime

def tr(inp, n, p):
    
    print("n:", n)
    print("length:", len(inp))
    print("moves:", inp)
    
    for c in inp:
        if c == '+': n = (n + 1) % p
        if c == '-': n = (n - 1) % p
        if c == '/': 
            if n == 0: return True
            n = pow(n, -1, p)
    return False

def get_small_repr(x:int):
    
    """Finds small a and b such that a/b = x"""
    
    mat = matrix(ZZ, 3, 4)
    mat[0,0] = x*p
    mat[1,0] = p*p
    mat[2,0] = -1*p
    for i in range(3):
        mat[i,1+i] = 1

    lll = mat.LLL()
    pos = []
    for i in range(3):
        v = lll[i]
        if v[0] == 0:
            # b, k, a
            b,a = v[1], v[3]
            if b < 0: a,b = -a,-b
            pos.append((a,b))
    #print(pos)
    if len(pos) == 0:
        return None
    sc = [sum(abs(i) for i in r) for r in pos]
    return pos[sc.index(min(sc))]

def f_to_cont(f, optimize=True, _thres=-1, _d=0):
    
    print("Depth: ", _d, end="\r")
    a,b = f
    ret = []
    while b != 0:
        
        pre = []
        if optimize:
            
            thres = 50
            if abs(a//b) > thres:
                
                x = int(a * pow(b, -1, p))
                pre = []
                s = False
                for j in range(abs(a//b)//2):
                    x += 1
                    x = int(pow(x, -1, p))
                    pre.append(-1)
                    xx = f_to_cont(get_small_repr(x), optimize=False, _thres=thres, _d=_d+1)
                    if max(xx) < thres:
                        s = True
                        break
                
                if not s:
                    pre = []
                    x = int((a*pow(b,-1,p))%p)
                    
            else:
                x = int((a*pow(b,-1,p))%p)
            
            _f = get_small_repr(x)
            if (
                _f and 
                abs(_f[0]//_f[1]) < abs(a//b)
            ):
                a,b = _f
        
        ret.extend(pre)
        ret.append(a//b)
        if _thres > 0 and ret[-1] > _thres:
            return ret
        a,b = b,a%b
    return ret

def cont_to_inp(cont):
    f = lambda s: ("-+")[s < 0] * abs(s)
    inp = "".join(f(r) + "/" for r in cont[:-1]) + f(cont[-1])
    inp = inp.replace("//", "")
    return inp + "/"

n = 314159265358979323846264338327950288419716939937510582097494459230781640628
ninv = int(pow(n, -1, p))
cont = f_to_cont((p+1, ninv+p))
print("Expansion:", cont)
inp = cont_to_inp(cont)
assert tr(inp, n, p)

import requests

requests.post("http://localhost:5555/api/submit", 
    json = {
        "player_name": "Jules!!",
        "moves": inp
    }
)