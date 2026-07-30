using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Channels;
using OscillographPlayer.Models;

namespace TestProject.OscillographPlayerTest
{
    internal class AudioSaveTest
    {
        internal static void SaveSinAudio()
        {
            short[] data = new short[88200];

            for (int i = 0, j = 0; i < 44100; i++)
            {
                data[j] = (short)((MathF.Sin(i * (1.0f / 200) * MathF.PI)) * 23767);
                j++;
                data[j] = (short)((MathF.Cos(i * (1.0f / 200) * MathF.PI)) * 23767);
                j++;
            }

            AudioModel audio = new(data, 44100);

            byte[] fileData = audio.AudioFileData;

            using FileStream saveStream = new(@"D:\05.Code\.Process\OscillographPlayer\TestProject\wave.wav", FileMode.Create);

            saveStream.Write(fileData);
            saveStream.Flush();

            long fileSize = new FileInfo(@"D:\05.Code\.Process\OscillographPlayer\TestProject\wave.wav").Length;

            Console.WriteLine($"File Size:{fileSize}");
            Console.WriteLine($"Byte Array Length:{fileData.Length}");
            Console.WriteLine($"Expected Size:{audio.AudioFileData.Length}");
        }
    }
}