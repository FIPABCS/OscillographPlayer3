using FFMediaToolkit;
using FFMediaToolkit.Decoding;
using FFMediaToolkit.Graphics;
using System;
using System.Collections.Generic;
using System.Text;
using OscillographPlayer.WaveGenerateLib;

namespace TestProject.WaveGenerateLibTest
{
    class WaveGenerateTest
    {
        internal static void EdgeDetectionAndPrintFrame()
        {
            FFmpegLoader.FFmpegPath = @"D:\05.Code\.Process\OscillographPlayer\ffmpeg\bin\";
            var mediaOption = new MediaOptions() { VideoPixelFormat = ImagePixelFormat.Gray8 };
            string videoPath = @"D:\01.Video\素材\【東方】Bad Apple!! ＰＶ【影絵】.mp4";

            Console.WriteLine(Directory.Exists(FFmpegLoader.FFmpegPath));
            Console.WriteLine(string.Join(Environment.NewLine, Directory.GetFiles(FFmpegLoader.FFmpegPath, "*.dll")));

            var testVideo = MediaFile.Open(videoPath, mediaOption);
            int second;
            string? buffer;
            while (true)
            {
                Console.WriteLine("Enter the second:");
                buffer = Console.ReadLine();
                if (string.IsNullOrWhiteSpace(buffer))
                {
                    break;
                }
                second = int.Parse(buffer);

                ImageData frame = testVideo.Video.GetFrame(TimeSpan.FromSeconds(second));
                int height = frame.ImageSize.Height;
                int width = frame.ImageSize.Width;

                //在此处获取帧像素数据
                byte[] pixDataBuf = frame.Data.ToArray();
                byte[] pixData = new byte[width * height];
                for(int i=0;i<width*height;i++)
                {
                    pixData[i] = pixDataBuf[i];
                }

                var edgeMap = EdgeDetactionServices.EdgeDetectionImage(pixData, width, height, 1.0f, 8, 128);

                for (int y = 0; y < edgeMap.GetLength(0); y++)
                {
                    for(int x=0;x<edgeMap.GetLength(1);x++)
                    {
                        Console.Write("{0}{1}", edgeMap[y, x] == 255 ? 'X' : '-', x == edgeMap.GetLength(1) - 1 ? '\n' : ' ');
                    }
                }
            }

            return;
        }
    }
}
