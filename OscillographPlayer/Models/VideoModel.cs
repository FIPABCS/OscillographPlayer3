using FFMediaToolkit.Decoding;
using FFMediaToolkit.Graphics;
using System;
using System.Collections.Generic;
using System.Reflection.Metadata;
using System.Text;

namespace OscillographPlayer.Models
{
    public class VideoModel
    {
        private readonly MediaFile _video;
        public MediaFile Video
        {
            get => _video;
        }

        private readonly int _width;
        public int Width
        {
            get => _width;
        }

        private readonly int _height;
        public int Height
        {
            get => _height;
        }

        private readonly double _durationInMillisecond;
        public double DurationInMillisecond
        {
            get => _durationInMillisecond;
        }

        private readonly int _totalFrameNumber;
        public int TotalFrameNumber
        {
            get => _totalFrameNumber;
        }

        private readonly double _frameRate;
        public double FrameRate
        {
            get => _frameRate;
        }

        public VideoModel(MediaFile video)
        {
            _video = video;

            _width = _video.Video.Info.FrameSize.Width;
            _height = _video.Video.Info.FrameSize.Height;
            _durationInMillisecond = _video.Video.Info.Duration.TotalMilliseconds;
            _totalFrameNumber = _video.Video.Info.NumberOfFrames ?? -1;
            _frameRate = _video.Video.Info.AvgFrameRate;
        }
    }
}
