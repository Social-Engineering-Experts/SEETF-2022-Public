using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace KeyGenerator
{
    internal class Program
    {
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
    }
}
