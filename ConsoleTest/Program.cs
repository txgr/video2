using System;
using System.Runtime.InteropServices;

namespace ConsoleTest
{
    class Program
    {
        [DllImport("Video.Core.dll", EntryPoint = "Test", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        public static extern int Test();

        [DllImport("Video.Core.dll", EntryPoint = "Start", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        public static extern int Start(string str, string str2);

        static void Main(string[] args)
        {
            string video_fname = "G:\\0.flv";
            string rtmp_url = "rtmp://159.138.5.74:2250/live/fghgfhgfhfgdgd";
              Start(video_fname, rtmp_url);
            Console.ReadKey();
            Console.WriteLine("Hello World!");
        }
    }
}
