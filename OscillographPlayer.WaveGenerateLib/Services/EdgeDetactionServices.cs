using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace OscillographPlayer.WaveGenerateLib
{
    public partial class EdgeDetactionServices
    {
        [return: MarshalAs(UnmanagedType.Bool)]
        [UnmanagedCallConv(CallConvs = new Type[] { typeof(CallConvStdcall) })]
        [LibraryImport(@"OscillographPlayer.WaveGenerateCore.dll", EntryPoint = "EdgeDetection")]
        private static partial bool EdgeDetectionUnsafe(
            [In]
            byte[] grayImage,
            ushort width,
            ushort height,
            float stepLength,
            byte lowThreshold,
            byte highThreshold,
            [Out]
            byte[] edgeMap
        );

        public static byte[,] EdgeDetectionImage(byte[,] grayImage, float stepLength, byte lowThreshold, byte highThreshold)
        {
            ushort width = (ushort)grayImage.GetLength(1),
                   height = (ushort)grayImage.GetLength(0),
                   widthNew = (ushort)(width / stepLength),
                   heightNew = (ushort)(height / stepLength);

            byte[] edgeImageFlat = new byte[widthNew * heightNew];

            if (!EdgeDetectionUnsafe(ArrayHelpers.FlatMap<byte>(ref grayImage), width, height, stepLength, lowThreshold, highThreshold, edgeImageFlat))
            {
                throw new Exception("EdgeDetectionImage running except.");
            }
            if (edgeImageFlat == null)
            {
                throw new Exception("Fail to get new map.");
            }

            return ArrayHelpers.BulidMap<byte>(ref edgeImageFlat, widthNew, heightNew);
        }

        public static byte[,] EdgeDetectionImage(byte[] grayImageFlat, ushort width, ushort height, float stepLength, byte lowThreshold, byte highThreshold)
        {
            ushort widthNew = (ushort)(width / stepLength),
                   heightNew = (ushort)(height / stepLength);

            byte[] edgeImageFlat = new byte[widthNew * heightNew];

            if (!EdgeDetectionUnsafe(grayImageFlat, width, height, stepLength, lowThreshold, highThreshold, edgeImageFlat))
            {
                throw new Exception("EdgeDetectionImage running except.");
            }

            return ArrayHelpers.BulidMap<byte>(ref edgeImageFlat, widthNew, heightNew);
        }

        public static byte[] EdgeDetectionImageFlat(byte[] grayImageFlat, ushort width, ushort height, float stepLength, byte lowThreshold, byte highThreshold)
        {
            ushort widthNew = (ushort)(width / stepLength),
                   heightNew = (ushort)(height / stepLength);

            byte[] edgeImageFlat = new byte[widthNew * heightNew];

            if (!EdgeDetectionUnsafe(grayImageFlat, width, height, stepLength, lowThreshold, highThreshold, edgeImageFlat))
            {
                throw new Exception("EdgeDetectionImageFlat running except.");
            }

            return edgeImageFlat;
        }
    }
}