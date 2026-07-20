using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace OscillographPlayer.WaveGenerateLib
{
    public partial class EdgeDetaction
    {
        [return: MarshalAs(UnmanagedType.Bool)]
        [UnmanagedCallConv(CallConvs = new Type[] { typeof(CallConvStdcall) })]
        [LibraryImport(@"OscillographPlayer.WaveGenerateCore.dll", EntryPoint = "EdgeDetection")]
        private static partial bool EdgeDetectionUnsafe(
            [In, MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.U1)]
            byte[] grayImage, 
            int width,
            int height, 
            float stepLength,
            byte lowThreshold,
            byte highThreshold,
            [Out, MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.U1)]
            byte[] edgeMap
        );

        public static byte[,] EdgeDetectionImage(byte[,] grayImage,float stepLength,byte lowThreshold,byte highThreshold)
        {
            int width = grayImage.GetLength(1),
                height = grayImage.GetLength(0),
                widthNew = (int)(width / stepLength),
                heightNew = (int)(height / stepLength);

            byte[] edgeImageFlat = new byte[widthNew * heightNew];

            if(!EdgeDetectionUnsafe(ArrayHelpers.FlatMap<byte>(ref grayImage), width, height, stepLength, lowThreshold, highThreshold, edgeImageFlat))
            {
                throw new Exception("WaveGenerateCore running except.");
            }
            if (edgeImageFlat == null)
            {
                throw new Exception("Fail to get new map.");
            }

            return ArrayHelpers.BulidMap<byte>(ref edgeImageFlat, widthNew, heightNew);
        }

        public static byte[,] EdgeDetectionImage(byte[] grayImageFlat, int width,int height, float stepLength, byte lowThreshold, byte highThreshold)
        {
            int widthNew = (int)(width / stepLength),
                heightNew = (int)(height / stepLength);

            byte[] edgeImageFlat = new byte[widthNew * heightNew];

            if(!EdgeDetectionUnsafe(grayImageFlat, width, height, stepLength, lowThreshold, highThreshold, edgeImageFlat))
            {
                throw new Exception("WaveGenerateCore running except.");
            }

            return ArrayHelpers.BulidMap<byte>(ref edgeImageFlat, widthNew, heightNew);
        }
    }
}
