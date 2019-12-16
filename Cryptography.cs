using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace MVServer.Packets
{
    internal class Cryptography
    {
        [DllImport("MVS.Crypt.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool DecryptPacket4(IntPtr src, IntPtr dst, int size);

        [DllImport("MVS.Crypt.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool DecryptPacket8(IntPtr src, IntPtr dst, int size);

        [DllImport("MVS.Crypt.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool EncryptPacket4(IntPtr src, IntPtr dst, int size);

        [DllImport("MVS.Crypt.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool EncryptPacket8(IntPtr src, IntPtr dst, int size);

        [DllImport("MVS.Crypt.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool EncryptPacket16(IntPtr src, IntPtr dst, int size);

        public static bool DecryptPacket(byte[] src)
        {
            var size = src.Length;

            if (size < 4)
                return false;

            var bufPtr = Marshal.AllocHGlobal(size);
            Marshal.Copy(src, 0, bufPtr, size);

            if (!DecryptPacket4(bufPtr, bufPtr, 4) ||
                !DecryptPacket8(bufPtr + 4, bufPtr + 4, size - 4))
            {
                Marshal.FreeHGlobal(bufPtr);
                return false;
            }

            Marshal.Copy(bufPtr, src, 0, size);
            Marshal.FreeHGlobal(bufPtr);

            return true;
        }
        static public string ToReadableByteArray(byte[] bytes)
        {
            return string.Join(" ", bytes);
        }
        public static bool EncryptPacket(byte[] src)
        {
            var result = false;

            var size = src.Length;

            if (size < 4)
                return false;

            var bufPtr = Marshal.AllocHGlobal(size);
            Marshal.Copy(src, 0, bufPtr, size);

            if (size >= 8 && size < 16)
                result = EncryptPacket8(bufPtr + 4, bufPtr + 4, size - 4);

            else if (size >= 16)
                result = EncryptPacket16(bufPtr + 4, bufPtr + 4, size - 4);

            if (!result || !EncryptPacket4(bufPtr, bufPtr, 4))
            {
                Marshal.FreeHGlobal(bufPtr);
                return false;
            }

            Marshal.Copy(bufPtr, src, 0, size);
            Marshal.FreeHGlobal(bufPtr);

            return true;
        }
    }
}

