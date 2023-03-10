using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.Drawing;
using System.IO;

namespace VisualME7Logger
{
    static class Program
    {
        public static string ME7LoggerDirectory;
        public static bool DebugOutput;
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            ME7LoggerDirectory = System.IO.Path.GetDirectoryName(Application.ExecutablePath);
            if (args.Length > 0)
            {
                ME7LoggerDirectory = args[0];
            }

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new SettingsForm());
        }
    }
}
