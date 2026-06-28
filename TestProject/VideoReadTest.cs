using FFMediaToolkit;
using FFMediaToolkit.Graphics;
using FFMediaToolkit.Decoding;

namespace TestProject
{
    internal class VideoReadTest
    {
        static void Main()
        {
            FFmpegLoader.FFmpegPath = @"D:\05.Code\.Process\OscillographPlayer\ffmpeg\bin\";
            var mediaOption = new MediaOptions() { VideoPixelFormat = ImagePixelFormat.Gray8 };
            string videoPath = @"D:\01.Video\素材\【東方】Bad Apple!! ＰＶ【影絵】.mp4";

            Console.WriteLine(Directory.Exists(FFmpegLoader.FFmpegPath));
            Console.WriteLine(string.Join(Environment.NewLine, Directory.GetFiles(FFmpegLoader.FFmpegPath, "*.dll")));

            var testVideo = MediaFile.Open(videoPath, mediaOption);

            for (int second = 0; second!=-1;)
            {
                Console.WriteLine("Enter the second:");
                second = int.Parse(Console.ReadLine());

                ImageData frame = testVideo.Video.GetFrame(TimeSpan.FromSeconds(second));

                byte[] pixData = frame.Data.ToArray();

                int height = frame.ImageSize.Height;
                int width = frame.ImageSize.Width;

                bool[,] grayMap = new bool[height, width];

                for (int y = 0; y < height; y++)
                {
                    for(int x=0;x<width;x++)
                    {
                        byte buf = 0;
                        buf = pixData[y * width + x];

                        grayMap[y, x] = buf > 128;
                    }
                }

                for(int y=0;y<height;y++)
                {
                    for(int x=0;x<width;x++)
                    {
                        Console.Write("{0}{1}", grayMap[y,x]?'1':'0',x==width-1?'\n':' ');
                    }
                }
            }
        }
    }
}