using FFMediaToolkit;
using FFMediaToolkit.Graphics;
using FFMediaToolkit.Decoding;
using TestProject.WaveGenerateLibTest;
using OscillographPlayer.WaveGenerateLib;
using TestProject.OscillographPlayerTest;

namespace TestProject
{
    internal class Program
    {
        private static void Main()
        {
            //VideoLoadTest.LoadAndPrintVideoFrames();
            //WaveGenerateTest.EdgeDetectionAndPrintFrame();

            //WaveGenerateTest.WaveGenerateAndPrintPoints(2.0f, 32, 128, WaveGenerateServices.ArrangeMethod.ByPoint, 30.0, 28800, false, false, false);

            OscillographPlayerTest.AudioSaveTest.SaveSinAudio();

            return;
        }
    }
}