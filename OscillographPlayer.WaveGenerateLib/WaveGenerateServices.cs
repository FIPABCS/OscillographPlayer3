using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;

namespace OscillographPlayer.WaveGenerateLib
{
    public partial class WaveGenerateServices
    {
        public enum ArrangeMethod { ByPoint,ByFrame};

        [return: MarshalAs(UnmanagedType.Bool)]
        [UnmanagedCallConv(CallConvs = new Type[] { typeof(CallConvStdcall) })]
        [LibraryImport(@"OscillographPlayer.WaveGenerateCore.dll", EntryPoint = "WaveGenerate")]
        private static partial bool WaveGenerateUnsafe(
            [In, MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.U1)]
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
            [MarshalAs(UnmanagedType.LPArray,ArraySubType = UnmanagedType.I2)]
            ref short[] waveArray,
            int length
            );

        private static short[] WaveGenerate(byte[] edgeImageFlat, ushort width,ushort height,ArrangeMethod arrangeMethod, bool horizontalFlip,bool verticalFlip)
        {
            const byte highGray = 255;
            int sampleInFrame = 0;
            foreach(var gray in edgeImageFlat)
            {
                if (gray == highGray)
                {
                    sampleInFrame++;
                }
            }

            short[] waveArray = new short[sampleInFrame * 2];

            if(!WaveGenerateUnsafe(edgeImageFlat, width, height, arrangeMethod, sampleInFrame, horizontalFlip, verticalFlip, waveArray))
            {
                throw new Exception("WaveGenerate running except.");
            }

            return waveArray;
        }

        private static void AmplitudeMaximization(short[] waveArray)
        {
            AmplitudeMaximizationUnsafe(ref waveArray, waveArray.Length);

            return;
        }
    }
}
