using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace BestSoftware
{
    internal class Program
    {
        private const string SECRET_KEY = "1_l0v3_CSh4rp";

        public static void Main(string[] args)
        {
            Console.WriteLine("===== BestSoftware =====");
            Console.WriteLine("> Checking BestSoftware license...");
            Console.WriteLine("> BestSoftware is unlicensed...");
            
            //LICENSE
            Console.WriteLine("> Please enter your name...");
            Console.Write("> ");
            string name = Console.ReadLine();

            Console.WriteLine("> Please enter your email...");
            Console.Write("> ");
            string email = Console.ReadLine();

            Console.WriteLine("> Please enter your license key...");
            Console.Write("> ");
            string licenseKey = Console.ReadLine();

            Console.WriteLine("> Activating BestSoftware license...");

            if (CheckLicenseKey(name, email, licenseKey))
            {
                Console.WriteLine("> Activated BestSoftware license...");

                Console.WriteLine("> The flag is SEE{" + licenseKey + "}...");
            }
            else
                Console.WriteLine("> Activation failed, invalid license key...");

            Console.WriteLine("> Press any key to exit...");
            Console.ReadKey();
        }

        public static bool CheckLicenseKey(string name, string email, string licenseKey)
        {
            string licenseToCompare = name + SECRET_KEY + email;
            string licenseToCompare_Hashed = CalculateSHA256(licenseToCompare);

            if (licenseKey.Equals(licenseToCompare_Hashed))
            {
                return true;
            }

            return false;
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
