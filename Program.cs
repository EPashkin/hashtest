using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.IO;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace LockTest
{
    class Program
    {
        static void Main(string[] args)
        {
            if (System.Diagnostics.Debugger.IsAttached)
            {
                Console.WriteLine("Debugger attached");
#if DEBUG
                Console.ReadLine();
#endif
            }

            string lockFile = "test.lock";
            LockInfo li1 = new LockInfo { threadnum = 1, maxcount = 10, lockFile = lockFile };
            LockInfo li2 = new LockInfo { threadnum = 2, maxcount = 10, lockFile = lockFile };

            Thread thread1 = new Thread(li1.LockTest);
            Thread thread2 = new Thread(li2.LockTest);
            Stopwatch sw = Stopwatch.StartNew();
            thread1.Start();
            thread2.Start();
            /*
            Thread thread1 = new Thread(LockTest);
            Thread thread2 = new Thread(LockTest);
            thread1.Start(li1);
            thread2.Start(li2);
            */
            PerformanceCounter cnt = new PerformanceCounter("Процессор", "% загруженности процессора", "_Total");
            while (thread1.IsAlive && thread2.IsAlive)
            {
                float i = cnt.NextValue();
                Console.WriteLine("Proc: {0}", i);
            }
            thread1.Join();
            thread2.Join();
            sw.Stop();

            li1.PrintEnd();
            li2.PrintEnd();
            Console.WriteLine("All done in {0}, press ENTER", sw.Elapsed);
            Console.ReadLine();
        }

        [Conditional("DEBUG")]
        public static void PrintStack()
        {
            StackTrace st = new StackTrace(1, true);
            StackFrame sf = st.GetFrame(0);
            //Console.WriteLine(sf.ToString());
            System.Reflection.MethodBase met = sf.GetMethod();
            string mes = String.Format("{1}:{0}", met.ToString(), met.DeclaringType.ToString());
            string loc = sf.GetFileName();
            if (!String.IsNullOrEmpty(loc))
                mes += String.Format(" in {0}:{1}:{2}", loc, sf.GetFileLineNumber(), sf.GetFileColumnNumber());
            Console.WriteLine(mes);
            //Console.ReadLine();
        }

        public static void LockTest(object state)
        {
            LockInfo li = (LockInfo)state;
            Random rnd = new Random();
            for (int i = 0; i < li.maxcount; i++)
            {
                bool b = li.TryLockFile();
                if (b)
                {
                    int delay = rnd.Next(100, 500);
                    Console.WriteLine("{0}: lock {1} for {2}", li.threadnum, i, delay);
                    Thread.Sleep(delay);
                    Console.WriteLine("{0}: unlock {1}", li.threadnum, i);
                    li.CloseLock();
                    Thread.Sleep(10);
                }
                else
                {
                    Console.WriteLine("{0}: nolock {1}", li.threadnum, i);
                    Thread.Sleep(10);
                    i--;
                }
            }
        }
    }
    class LockInfo
    {
        public int threadnum;
        public int maxcount;
        public string lockFile;
        private FileStream m_f = null;
        private int succlock = 0;
        private int waiting = 0;

        ~LockInfo()
        {
            CloseLock();
        }

        public void LockTest()
        {
            Random rnd = new Random();
            for (int i = 0; i < maxcount; i++)
            {
                bool b = TryLockFile();
                if (b)
                {
                    int delay = rnd.Next(100, 500);
                    Console.WriteLine("{0}: lock {1} for {2}", threadnum, i, delay);
                    Thread.Sleep(delay);
                    Console.WriteLine("{0}: unlock {1}", threadnum, i);
                    CloseLock();
                    Thread.Sleep(10);
                }
                else
                {
                    Console.WriteLine("{0}: nolock {1}", threadnum, i);
                    Thread.Sleep(10);
                    i--;
                }
            }
            Program.PrintStack();
        }

        public void PrintEnd()
        {
            Console.WriteLine("{0}: locked {1}, waiting {2}", threadnum, succlock, waiting);
        }

        public bool TryLockFile()
        {
            CloseLock();
            try
            {
                m_f = File.Open(lockFile, FileMode.OpenOrCreate, FileAccess.ReadWrite, FileShare.None);
                succlock++;
            }
            catch (System.IO.IOException ex)
            {
                if (!IsFileLocked(ex))
                    throw;
                waiting++;
                return false;
            }
            return true;
        }

        private static bool IsFileLocked(IOException exception)
        {
            int errorCode = Marshal.GetHRForException(exception) & ((1 << 16) - 1);
            return errorCode == 32 || errorCode == 33;
        }

        public void CloseLock()
        {
            lock (this)
            {
                if (m_f != null)
                {
                    m_f.Close();
                    m_f = null;
                }
            }
        }

    }
}
