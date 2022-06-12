# magic - Solution

**Author**: Anonymous

**Category**: RE

The program is filled with a ton of anti-debug mechanisms. Viewing the binary in IDA will cause the auto-analysis to process the `main` function prematurely, causing the decompilation to be cut off. Forcing a function after the supposed end of function reveals the rest of the code in the decompiler.

Analyzing the function would reveal that the program opens a registry in `CurrentVersion\Run` and checks for the existence of a `SEEtf` key. The program also checks if the value of the key is set to `hunter_123456_qwerty`. The check is done by encrypting the key's value with RC4 and comparing the ciphertext found within the binary. Passing this check will reveal an admin panel, which prompts for an 8 digit passcode. The input is then passed into a hashing algorithm (djb2), and compared with `0xb0241528`. The bruteforce script for the passcode can be found [here](hash_bruteforce.py).

After supplying the correct passcode, the flag will be printed out to stdout.

`SEE{s3cur1ty_by_0bscur1ty_1899df}`
