using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace OscillographPlayer.WaveGenerateLib
{
    public partial class WaveGenerate
    {
        [return: MarshalAs(UnmanagedType.Bool)]
        [UnmanagedCallConv(CallConvs = new Type[] { typeof(CallConvStdcall) })]
        [LibraryImport(@"OscillographPlayer.WaveGenerateCore.dll", EntryPoint = "EdgeDetection")]
        private static partial bool EdgeDetectionUnsafe(
            [In, MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.U1)]
            byte[] grayImage, 
            int width, int height, float stepLength, byte lowThreshold, byte highThreshold,
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

            if(!EdgeDetectionUnsafe(FlatMap<byte>(ref grayImage), width, height, stepLength, lowThreshold, highThreshold, edgeImageFlat))
            {
                throw new Exception("WaveGenerateCore running except.");
            }
            if (edgeImageFlat == null)
            {
                throw new Exception("Fail to get new map.");
            }

            return BulidMap<byte>(ref edgeImageFlat, widthNew, heightNew);
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

            return BulidMap<byte>(ref edgeImageFlat, widthNew, heightNew);
        }

        private static T[] FlatMap<T>(ref T[,] map)
        {
            int width = map.GetLength(1),
                height = map.GetLength(0);

            T[] flatMap = new T[width * height];

            for(int y=0,i=0;y<height;y++)
            {
                for(int x=0;x<width;x++,i++)
                {
                    flatMap[i] = map[y, x];
                }
            }

            return flatMap;
        }

        private static T[,] BulidMap<T>(ref T[] flatMap,int  width,int height)
        {
            if(flatMap.Length!=width*height)
            {
                throw new Exception("Can not build map int this size.");
            }

            T[,] map = new T[height, width];

            for (int y = 0, i = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++, i++)
                {
                    map[y, x] = flatMap[i];
                }
            }

            return map;
        }
    }
}
