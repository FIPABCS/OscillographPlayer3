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

        public int Width
        {
            get => _video.Video.Info.FrameSize.Width;
        }

        public int Height
        {
            get => _video.Video.Info.FrameSize.Height;
        }

        public double DurationInMillisecond
        {
            get => _video.Video.Info.Duration.TotalMilliseconds;
        }

        public int TotalFrameNumber
        {
            get => _video.Video.Info.NumberOfFrames ?? -1;
        }

        public double FrameRate
        {
            get => _video.Video.Info.AvgFrameRate;
        }

        public VideoModel(MediaFile video) => _video = video;
    }
}
