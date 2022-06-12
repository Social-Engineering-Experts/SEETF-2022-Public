# Hall_of_Fame
**Author**: [@L0uisJ0shua](https://www.github.com/L0uisJ0shua)

**Category**: Pwn

[Solution](solve/exploit.py)

## Challenge Description
It’s about drive, it’s about power, we stay hungry, we devour Put in the work, put in the hours and take what’s ours. Time to get to the Hall of Fame and be among the GOATS.

## Difficulty
Medium - Hard

## Solution
The vulnerability found in this challenge is found in option 1, where we can assign size of the cart and the size of the cart, which is user-controlled, is used to determine the size of the malloc chunk allocated. The 2nd input taken in, controls the data placed into the malloc chunk allocated. Thus, we are able to utilise the HOF primitive to overwrite the data found in the heap chunk.

The exploit is Heap overflow utilisng House of Force Primitive to gain RCE
- HOF primitive (Overwrite size of wilderness)
- Locate &__malloc_hook in libc
- Malloc to overwrite function pointer in __malloc_hook to libc.system address
- Setup system command execution by allocating /bin/sh or search in libc
- Malloc one more time.
- Gain RCE and read flag.txt

Refer to `solution/exploit.py` for more.

## Deployment
```
docker-compose up -d
```
