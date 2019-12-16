using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MVServer
{
    class Program
    {

       

        static void Main(string[] args)
        {
            byte[] startmatch = {0x90, 0x03, 0x20, 0x40, 0xCB, 0x1A, 0x26, 0x00 };
            //C
            //byte[] cgdpass = Encoding.ASCII.GetBytes("!dptmzpdl@xmfkdlvhtm@goqm!");
            Console.WriteLine(BitConverter.ToString(startmatch) + "\n");
            if (Packets.Cryptography.EncryptPacket(startmatch))
            {
                //string result = Encoding.UTF8.GetString(startmatch);
                //Console.WriteLine(result + "\n");
                Console.WriteLine(BitConverter.ToString(startmatch) + "\n");
            }
         



            Console.WriteLine("Press any key to continue . . .");
            Console.ReadLine();
            //Console.WriteLine(MVServer.Packets.Cryptography.DecryptPacket(packet).ToString());
        }
    }
}
