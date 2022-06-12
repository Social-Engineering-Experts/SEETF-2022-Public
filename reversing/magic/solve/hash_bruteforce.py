hash_to_find = 0xB0241528

def generate_hash(plaintext):
    i = 5381
    for byte in plaintext:
        j = ord(byte) + 32 * i
        i += j
    
    return i & 0xFFFFFFFF

for i in range(99999999):
    i = str(i)
    if generate_hash(i) == hash_to_find:
        print(f"Password: {i}")
	break
