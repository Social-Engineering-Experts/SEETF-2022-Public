import string
from Crypto.Util.number import long_to_bytes, bytes_to_long

FLAG = "SEE{1nf0rm3d_1nv3st1ng_8883695db92227ff6670b2e4ac3986d5}"

with open("pigeon.txt", "r") as f:
    data = f.read()

encoded = ""
flag = bin(bytes_to_long(FLAG.encode()))[2:]

j = 0
for i in range(len(data)):
    if data[i] in string.ascii_letters:
        if flag[j % len(flag)] == "1":
            encoded += data[i].upper()
        elif flag[j % len(flag)] == "0":
            encoded += data[i].lower()
        j += 1
    else:
        encoded += data[i]

print(encoded)

res = ""
for char in encoded:
    if char in string.ascii_uppercase:
        res += "1"
    elif char in string.ascii_lowercase:
        res += "0"

res = int(res, 2)
print(res)
print(long_to_bytes(res))