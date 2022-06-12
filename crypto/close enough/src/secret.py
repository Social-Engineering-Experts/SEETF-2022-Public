from Crypto.Util.number import isPrime

flag = "SEE{i_love_really_secure_algorithms_b5c0b187fe309af0f4d35982fd961d7e}"


def getNextPrime(num):
    num += 1
    while not isPrime(num):
        num += 1
    return num
