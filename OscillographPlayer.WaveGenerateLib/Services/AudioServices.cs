using OscillographPlayer.WaveGenerateLib.Models;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Threading.Tasks;

namespace OscillographPlayer.Services
{
    public static class AudioServices
    {
        public static async Task SaveAudioFileAsync(AudioModel audioFileData,string fileName,string filePath)
        {
            if(string.IsNullOrWhiteSpace(fileName))
            {
                throw new ArgumentNullException(nameof(fileName));
            }
            if (string.IsNullOrWhiteSpace(filePath))
            {
                throw new ArgumentNullException(nameof(filePath));
            }

            if(!Path.Exists(filePath))
            {
                Directory.CreateDirectory(filePath);
            }

            await File.WriteAllBytesAsync(Path.Combine(filePath, fileName), audioFileData.AudioFileData);
        }
    }
}
