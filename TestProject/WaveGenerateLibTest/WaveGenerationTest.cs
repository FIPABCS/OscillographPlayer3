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
                if (buffer == null)
                {
                    break;
                }
                second = int.Parse(buffer);

                ImageData frame = testVideo.Video.GetFrame(TimeSpan.FromSeconds(second));
                int height = frame.ImageSize.Height;
                int width = frame.ImageSize.Width;

                //在此处获取帧像素数据
                byte[] pixData = frame.Data.ToArray();

                var edgeMap = WaveGenerate.EdgeDetectionImage(pixData, width, height, 1, 32, 128);

                for (int y = 0; y < edgeMap.GetLength(0); y++)
                {
                    for(int x=0;x<edgeMap.GetLength(1);x++)
                    {
                        Console.Write("{0}{1}", edgeMap[y, x] ? '1' : '0', x == width - 1 ? '\n' : ' ');
                    }
                }
            }
        }
    }
}
