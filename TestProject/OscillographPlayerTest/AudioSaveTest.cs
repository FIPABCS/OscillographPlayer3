using System;
using System.Collections.Generic;
using System.Text;
using OscillographPlayer.Models;

namespace TestProject.OscillographPlayerTest
{
    internal class AudioSaveTest
    {
        internal static void SaveSinAudio()
        {
            short[] data = new short[44100];

            for (int i = 0; i < 44100; i++)
            {
                data[i] = (short)((MathF.Sin(i * (1 / 200) * MathF.PI)) * 128);
            }

            AudioModel audio = new(data, 44100);

            using FileStream saveStream = new(@"D:\05.Code\.Process\OscillographPlayer\TestProject\wave.wav", FileMode.Create);

            saveStream.Write(audio.AudioFileData);
        }
    }
}