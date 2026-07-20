using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;

namespace OscillographPlayer.WaveGenerateLib
{
    public partial class WaveGenerate
    {
        private enum ArrangeMethod { ByPoint,ByFrame};

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
            [In,MarshalAs(UnmanagedType.LPArray,ArraySubType = UnmanagedType.I2)]
            short[] waveArray,
            int length
            );
    }
}
