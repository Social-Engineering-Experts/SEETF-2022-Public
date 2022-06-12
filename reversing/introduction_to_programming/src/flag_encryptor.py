FLAG = r"SEE{dll_d0uch3b4663ry_c02a72e47f3d024c824c95da6ba896eb}"
ENC_FLAG = [1139, 358, 44, 18, 1092, 5, 1100, 54, 1092, 275, 28, 10, 1096, 90, 1090, 279, 95, 95, 1043, 27, 1113, 380, 10, 89, 1042, 322, 94, 1042, 326, 93, 1047, 15, 1043, 327, 89, 1042, 279, 10, 1048, 273, 93, 10, 1049, 92, 1092, 322, 95, 11, 1089, 283, 80, 1046, 326, 11, 1117]

def isPrime(n):
    if n==2 or n==3: return True
    if n%2==0 or n<2: return False
    for i in range(3, int(n**0.5)+1, 2):   # only odd numbers
        if n%i==0:
            return False    

    return True


def doXOR(char, index):
    KEY = (0x69, 0x420, 0x123)
    return char ^ KEY[index % 3]


def encrypt(to_enc):
    since_last_prime = 0
    out = []
    for idx, char in enumerate(to_enc):
        if isPrime(idx):
            since_last_prime = 0
        else:
            since_last_prime += 1

        out.append(doXOR(char, since_last_prime))

    return out


def simpler_encrypt(to_enc):
    out = []
    for idx, char in enumerate(to_enc):
        out.append(doXOR(char, idx))

    return out

print(encrypt(FLAG))
print(''.join([chr(c) for c in encrypt(ENC_FLAG)]))

# prompt_one = "DS420: Doubly-Linked Lists\nWe've made a few changes to the code in the previous assignment.\nTry to get the password! The encryption algorithm hasn't changed much.\n- Prof Zhang\n"
# prompt_question = "What's the flag? >> "
# prompt_two = "Wrong! See me after class if you can't figure it out.\n"
# prompt_three = "Correct! Submit your flag, champ."
# char_arr_one = [ord(i) for i in prompt_question]
# print(simpler_encrypt(char_arr_one))