# BestSoftware - Solution

**Author**: Gelos

**Category**: RE

The goal of this challenge is to perform reverse engineering on .NET, using any disassembler of their choice, figure out how the license key is generated in the program and also successfully generate the license key on their own.

The difficulty of this challenge could be slightly increased by using any .NET obfuscator/packer such as `ConfuserEx`.

## JetBrains dotPeek

Opening the .exe with dotPeek, we can find a function is called `Main`.

Inside the function, we can see that the flag is actually `SEE{"License Key"}`.

```csharp
string licenseKey = str2;
if (Program.CheckLicenseKey(name, email, licenseKey))
{
Console.WriteLine("> Activated BestSoftware license...");
Console.WriteLine("> The flag is SEE{" + str2 + "}...");
}
```

Next, we take a look at this function called `CheckLicenseKey`.

Inside the function, we can see that the license key is generated using `name` + `1_l0v3_CSh4rp` + `email` followed by a SHA256 hashing function.

```csharp
public static bool CheckLicenseKey(string name, string email, string licenseKey)
{
    string shA256 = Program.CalculateSHA256(name + "1_l0v3_CSh4rp" + email);
    return licenseKey.Equals(shA256);
}
```
## Solution

To solve this, we can simply either create a simple keygen, or use an online SHA256 generator.

Since the name and email has been provided in the challenge description, we can just plug the values right in.
```csharp
public static void Main(string[] args)
{
    Console.WriteLine(CalculateSHA256("seetf1_l0v3_CSh4rpseetf@seetf.sg"));
    Console.ReadKey();
}

public static string CalculateSHA256(string inputString)
{
    using (SHA256 sha256 = SHA256.Create())
    {
        byte[] sha256Bytes = sha256.ComputeHash(Encoding.UTF8.GetBytes(inputString));

        StringBuilder builder = new StringBuilder();
        for (int currentIndex = 0; currentIndex < sha256Bytes.Length; currentIndex++)
        {
            builder.Append(sha256Bytes[currentIndex].ToString("X2"));
        }

        return builder.ToString();
    }
}
```

We should get the output of `28F313A48C1282DF95E07BCEF466D19517587BCAB4F7A78532FA54AC6708444E`.

Simply run `BestSoftware.exe` and plug in the name as `seetf`, email as `seetf@seetf.sg` and the license key as `28F313A48C1282DF95E07BCEF466D19517587BCAB4F7A78532FA54AC6708444E`.

The program should give an output like `The flag is SEETF{28F313A48C1282DF95E07BCEF466D19517587BCAB4F7A78532FA54AC6708444E}...`.