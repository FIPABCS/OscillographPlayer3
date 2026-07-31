using System;
using System.Collections.Generic;
using System.Text;

namespace OscillographPlayer.Models
{
    public class SettingsModel
    {
        //低阈值
        private byte _lowThreshold;

        public byte LowThreshold
        {
            get => _lowThreshold;
            set
            {
                if (value < byte.MinValue)
                {
                    _lowThreshold = byte.MinValue;
                }
                else if (value > _highThreshold)
                {
                    _lowThreshold = _highThreshold;
                }
                else
                {
                    _lowThreshold = value;
                }
            }
        }

        //高阈值
        private byte _highThreshold;

        public byte HighThreshold
        {
            get => _highThreshold;
            set
            {
                if (value > byte.MaxValue)
                {
                    _highThreshold = byte.MaxValue;
                }
                else if (value < _lowThreshold)
                {
                    _highThreshold = _lowThreshold;
                }
                else
                {
                    _highThreshold = value;
                }
            }
        }

        //振幅最大化
        public bool AmplitudeMaximization { get; set; } = true;

        //水平翻转
        public bool HorizontalFlip { get; set; } = false;

        //竖直翻转
        public bool VerticalFlip { get; set; } = true;

        //视频帧采样步长
        public double SampleStepLength { get; set; } = 0;

        //音频采样率
        private int _sampleRate;

        public int SampleRate
        {
            get => _sampleRate;
            set
            {
                if (value < 0)
                {
                    _sampleRate = 1;
                }
                else
                {
                    _sampleRate = value;
                }
            }
        }

        //采样点排布方式
        public enum ArrangeMethodCollection
        {
            ArrangeByFrame,
            ArrangeByPoint
        }

        public ArrangeMethodCollection ArrangeMethod { get; set; } = ArrangeMethodCollection.ArrangeByFrame;

        public SettingsModel(byte highThreshold, byte lowThreshold, bool amplitudeMaximization, bool horizontalFlip, bool verticalFlip, int sampleRate, ArrangeMethodCollection arrangeMethod)
        {
            HighThreshold = highThreshold;
            LowThreshold = lowThreshold;
            AmplitudeMaximization = amplitudeMaximization;
            HorizontalFlip = horizontalFlip;
            VerticalFlip = verticalFlip;
            SampleRate = sampleRate;
            ArrangeMethod = arrangeMethod;
        }
    }
}