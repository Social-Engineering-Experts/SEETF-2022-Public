import sys
import cramjam
import pyshark
from binascii import unhexlify, hexlify

MOD = 256


def bit_not(n, numbits=8):
    return (1 << numbits) - 1 - n


def KSA(key):
    key_length = len(key)
    S = list(range(MOD))
    j = 0
    for i in range(MOD):
        j = (j + S[i] + key[i % key_length]) % MOD
        S[i], S[j] = S[j], S[i]

    return S


def PRGA(S):
    i = 0
    j = 0
    while True:
        i = (i + 1) % MOD
        j = (j + S[i]) % MOD

        S[i], S[j] = S[j], S[i]
        K = S[(S[i] + S[j]) % MOD]
        K = K ^ j  # MODIFICATION: extra XOR
        yield K


def get_keystream(key, drop):
    """
    Takes the encryption key to get the keystream using PRGA
    return object is a generator
    """
    S = KSA(key)
    i = 0
    j = 0

    # MODIFICATION: drop n
    for _ in range(drop):
        i = (i + 1) % MOD
        j = (j + S[i]) % MOD
        S[i], S[j] = S[j], S[i]

    return PRGA(S)


def encrypt_logic(key, input, shift, drop):
    """
    :key -> encryption key used for encrypting, as hex string
    :input -> array of unicode values/ byte string to encrpyt/decrypt
    """
    key = [ord(c) for c in key]

    keystream = get_keystream(key, drop)

    result = ""
    for c in input:
        # MODIFICATION: shift, bitwise negation
        if shift >= 0:
            val = "%02X" % (bit_not((c + shift) & 0xFF) ^ next(keystream) & 0xFF)
        elif shift:
            val = "%02X" % (((bit_not(c) ^ next(keystream)) + shift) & 0xFF)
        result += val
    return result


def decrypt(key, ciphertext, shift, drop):
    """
    :key -> encryption key used for encrypting, as hex string
    :ciphertext -> hex encoded ciphered text using RC4
    """
    ciphertext = unhexlify(ciphertext)
    result = encrypt_logic(key, ciphertext, shift, drop)
    if not len(result) % 2:
        return unhexlify(result)
    else:
        return unhexlify("0" + result)


def main():
    key = "FtkvWPDfIdKGWvP5788D4kNeO6FMCXsO"

    if len(sys.argv) != 2:
        print("Specify pcap filename as argument")
        exit(1)

    filename = sys.argv[1]
    needs_decompressing = False

    with pyshark.FileCapture(filename) as cap:
        for packet in cap:
            try:
                data = packet.data.data
            except AttributeError:
                # TCP packets like SYN, SYN ACK, ACK won't carry any data
                pass
            else:
                # if server to client
                if packet[packet.transport_layer].srcport == "1337":
                    decrypted = decrypt(key, data, -3, 3840)
                # if client to server
                else:
                    decrypted = decrypt(key, data, -2, 3328)
                    needs_decompressing = True

                if decrypted:
                    if needs_decompressing:
                        try:
                            decompressed = cramjam.snappy.decompress_raw(decrypted)
                        except Exception as e:
                            print("decrypted:", hexlify(decrypted))
                            print(e)
                        else:
                            print("decompressed: ", bytes(decompressed))
                    else:
                        print("decrypted:", decrypted)
                print("_____________________________")
                needs_decompressing = False

    print("Done")


if __name__ == "__main__":
    main()
