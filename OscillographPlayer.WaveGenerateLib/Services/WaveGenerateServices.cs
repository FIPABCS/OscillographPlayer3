using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace OscillographPlayer.WaveGenerateLib
{
    public partial class WaveGenerateServices
    {
        public enum ArrangeMethod { ByPoint, ByFrame };

        [return: MarshalAs(UnmanagedType.Bool)]
        [UnmanagedCallConv(CallConvs = new Type[] { typeof(CallConvStdcall) })]
        [LibraryImport(@"OscillographPlayer.WaveGenerateCore.dll", EntryPoint = "WaveGenerate")]
        private static partial bool WaveGenerateUnsafe(
            [In]
            byte[] edgeArray,
            ushort width,
            ushort height,
            ArrangeMethod arrangeMethod,
            int sampleInFrame,
            [MarshalAs(UnmanagedType.U1)]
            bool horizontalFlip,
            [MarshalAs(UnmanagedType.U1)]
            bool verticalFlip,
            [Out, MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.I2)]
            short[] waveArray
            );

        [UnmanagedCallConv(CallConvs = new Type[] { typeof(CallConvStdcall) })]
        [LibraryImport(@"OscillographPlayer.WaveGenerateCore.dll", EntryPoint = "AmplitudeMaximization")]
        private static partial void AmplitudeMaximizationUnsafe(
            [In,Out]
            short[] waveArray,
            int length
            );

        public static short[] WaveGenerate(byte[] edgeImageFlat, ushort width, ushort height, ArrangeMethod arrangeMethod, double frameRate, int sampleRate, bool horizontalFlip, bool verticalFlip)
        {
            int sampleInFrame = (int)(sampleRate / frameRate);

            short[] waveArray = new short[sampleInFrame * 2];

            if (!WaveGenerateUnsafe(edgeImageFlat, width, height, arrangeMethod, sampleInFrame, horizontalFlip, verticalFlip, waveArray))
            {
                throw new Exception("WaveGenerate running except.");
            }

            return waveArray;
        }

        public static void AmplitudeMaximization(ref short[] waveArray)
        {
            AmplitudeMaximizationUnsafe(waveArray, waveArray.Length);

            return;
        }
    }
}