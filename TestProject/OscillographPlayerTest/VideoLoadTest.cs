using FFMediaToolkit;
using FFMediaToolkit.Decoding;
using FFMediaToolkit.Graphics;
using System;
using System.Collections.Generic;
using System.Text;

namespace TestProject.OscillographPlayerTest
{
    internal class VideoLoadTest
    {
        internal static void LoadAndPrintVideoFrames()
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
                buffer=Console.ReadLine();
                if(buffer == null)
                {
                    break;
                }
                second = int.Parse(buffer);

                ImageData frame = testVideo.Video.GetFrame(TimeSpan.FromSeconds(second));

                //在此处获取帧像素数据
                byte[] pixData = frame.Data.ToArray();

                int height = frame.ImageSize.Height;
                int width = frame.ImageSize.Width;

                bool[,] grayMap = new bool[height, width];

                for (int y = 0; y < height; y++)
                {
                    for (int x = 0; x < width; x++)
                    {
                        byte buf;
                        buf = pixData[y * width + x];

                        grayMap[y, x] = buf > 128;
                    }
                }

                for (int y = 0; y < height; y++)
                {
                    for (int x = 0; x < width; x++)
                    {
                        Console.Write("{0}{1}", grayMap[y, x] ? '1' : '0', x == width - 1 ? '\n' : ' ');
                    }
                }
            }
        }
    }
}
