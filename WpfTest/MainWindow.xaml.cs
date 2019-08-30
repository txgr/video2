using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace WpfTest
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        [DllImport("Video.Core.dll", EntryPoint = "Start", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        public static extern int Start(String str, String str2);

       
        [DllImport("Video.Core.dll", EntryPoint = "GetCurrentPosition", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        public static extern int GetCurrentPosition();

        [DllImport("Video.Core.dll", EntryPoint = "GetPositionCount", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        public static extern int GetPositionCount();

        [DllImport("Video.Core.dll", EntryPoint = "SetPosition", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        public static extern int SetPosition(double position);

        private Thread _webSocketThread;
        private void StartWebSocket()
        {
            _webSocketThread = new Thread(ConnectWebSocket);
            _webSocketThread.Start();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            Slider1.AddHandler(Slider.MouseLeftButtonUpEvent, new MouseButtonEventHandler(blurSlider_MouseLeftButtonUp), true);
        }

        private void blurSlider_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            try
            {
                double ds = double.Parse(Slider1.Value.ToString());
                int df = SetPosition(ds);
            }
            catch (Exception)
            {

            }
           
          //  System.Windows.MessageBox.Show(df.ToString());
        }

        private void ConnectWebSocket()
        {
            String video_fname = "G:\\0.flv";
            String rtmp_url = "rtmp://159.138.5.74:2250/live/fghgfhgfhfgdgd";
            Start(video_fname, rtmp_url);
        }

        public MainWindow()
        {
           

             InitializeComponent();
            
            StartWebSocket();
          //  StartWebSocket();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            int dsf = GetPositionCount();
            
            int df = SetPosition(200);
            int sdf = GetCurrentPosition();
            int d3sf = 0;
        }
    }
}
