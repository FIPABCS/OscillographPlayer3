using SkiaSharp;
using System;
using System.Collections.Generic;
using System.Text;

namespace OscillographPlayer.Models
{
    public class AudioModel
    {
        private const string chunkID = "RIFF";

        private readonly int chunkSize;

        private const string format = "WAVE";

        private const string subchunk1ID = "fmt ";

        private const short subchunk1Size = 16;

        private const short audioFormat = 1;

        private const short numChannels = 2;

        private readonly int _sampleRate;

        private readonly int _byteRate;

        private const short _blockAlign = numChannels * bitsPerSample / 8;

        private const short bitsPerSample = 16;

        private const string subchunk2ID = "data";

        private readonly int subchunk2Size;

        private readonly short[] _audioData;

        public AudioModel(short[] audioData,int sampleRate)
        {
            _audioData = audioData;

            int dataSize = _audioData.Length * sizeof(short);
            subchunk2Size = dataSize;
            chunkSize = dataSize + 36;

            _sampleRate = sampleRate;
            
            _byteRate = _sampleRate * numChannels * bitsPerSample / 8;
        }
    }
}
