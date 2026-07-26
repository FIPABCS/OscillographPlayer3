using FFMediaToolkit;
using FFMediaToolkit.Graphics;
using FFMediaToolkit.Decoding;
using TestProject.WaveGenerateLibTest;
using OscillographPlayer.WaveGenerateLib;

namespace TestProject
{
    internal class Program
    {
        static void Main()
        {
            //VideoLoadTest.LoadAndPrintVideoFrames();
            //WaveGenerateTest.EdgeDetectionAndPrintFrame();

            WaveGenerateTest.WaveGenerateAndPrintPoints(2.0f, 32, 128, WaveGenerateServices.ArrangeMethod.ByFrame,30.0,28800, false, false, false);

            return;
        }
    }
}