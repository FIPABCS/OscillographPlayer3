using System;
using System.Runtime.InteropServices;

namespace OscillographPlayer.WaveGenerateLib
{
    public partial class WaveGenerate
    {
        [StructLayout(LayoutKind.Sequential)]
        private struct NoiseReductionCore
        {
            private IntPtr weight;
            private int radius;
        };

        [LibraryImport(@"OscillographPlayer.WaveGenerateCore.dll", EntryPoint = "EdgeDetection")]
        private static partial void EdgeDetectionUnsafe(
            [In, MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.U1)]
            byte[] grayImage, 
            int width, int height,
            float skip, byte lowThreshold, byte highThreshold,
            [Out, MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.U1)]
            byte[] edgeMap
        );
    }
}
