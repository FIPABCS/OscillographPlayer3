using SkiaSharp;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace OscillographPlayer.Models
{
    public class AudioModel
    {
        private const string _chunkID = "RIFF";

        private readonly int _chunkSize;

        private const string _format = "WAVE";

        private const string _subchunk1ID = "fmt ";

        private const short _subchunk1Size = 16;

        private const short _audioFormat = 1;

        private const short _numChannels = 2;

        private readonly int _sampleRate;

        public int SampleRate
        {
            get => _sampleRate;
        }

        private readonly int _byteRate;

        public int ByteRate
        {
            get => _byteRate;
        }

        private const short _blockAlign = _numChannels * _bitsPerSample / 8;

        private const short _bitsPerSample = 16;

        private const string _subchunk2ID = "data";

        private readonly int _subchunk2Size;

        private readonly short[] _audioData;

        public byte[] AudioFileData
        {
            get
            {
                using var memory = new MemoryStream(_chunkSize + 8);
                using var writer = new BinaryWriter(memory,Encoding.ASCII,true);

                writer.Write(Encoding.ASCII.GetBytes(_chunkID));
                writer.Write(_chunkSize);
                writer.Write(Encoding.ASCII.GetBytes(_format));
                
                writer.Write(Encoding.ASCII.GetBytes(_subchunk1ID));
                writer.Write(_subchunk1Size);
                writer.Write(_audioFormat);
                writer.Write(_numChannels);
                writer.Write(_sampleRate);
                writer.Write(_byteRate);
                writer.Write(_blockAlign);
                writer.Write(_bitsPerSample);
                
                writer.Write(Encoding.ASCII.GetBytes(_subchunk2ID));
                writer.Write(_subchunk2Size);

                foreach (var sampleData in _audioData)
                {
                    writer.Write(sampleData); ;
                }

                return memory.ToArray();
            }
        }

        private readonly double _durationInMillisecond;

        public double DurationInMillisecond
        {
            get => _durationInMillisecond;
        }

        public AudioModel(short[] audioData, int sampleRate)
        {
            _audioData = audioData;

            int dataSize = _audioData.Length * sizeof(short);
            _subchunk2Size = dataSize;
            _chunkSize = dataSize + 36;
            _sampleRate = sampleRate;
            _byteRate = _sampleRate * _numChannels * _bitsPerSample / 8;

            _durationInMillisecond = (_subchunk2Size / _byteRate) * 1000;
        }
    }
}
