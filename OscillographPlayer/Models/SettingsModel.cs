using System;
using System.Collections.Generic;
using System.Text;

namespace OscillographPlayer.Models
{
    public class SettingsModel
    {

        public bool MaintainAspectRatio { get; set; } = true;

        public bool StretchToOriginalFrame { get; set; } = true;
        
        public bool HorizontalFlip {  get; set; } = false;

        public bool VerticalFlip { get; set; }=true;

        private double _widthSqueezeRate;
        public double WidthSqueezeRate
        {
            get => _widthSqueezeRate;
            set
            {
                _widthSqueezeRate= value;

                if (MaintainAspectRatio && _heightSqueezeRate != _widthSqueezeRate)
                {
                    _heightSqueezeRate = _widthSqueezeRate;
                }
            }
        }

        private double _heightSqueezeRate;
        public double HeightSqueezeRate
        {
            get => _heightSqueezeRate;
            set
            {
                if(MaintainAspectRatio)
                {
                    throw new InvalidOperationException("Setting HeightSqueezeRate directly is disabled when MaintainAspectRatio is true.");
                }

                _heightSqueezeRate = value;
            }
        }

        private int _sampleRate;
        public int SampleRate
        {
            get => _sampleRate;
            set => _sampleRate = value;
        }

        public enum MappingMethodCollection
        {
            HighFrequecy, 
            LowFrequecy
        }

        public MappingMethodCollection MappingMethod { get; set; } = MappingMethodCollection.HighFrequecy;

        public SettingsModel(double widthSqueezeRate,bool maintainAspectRatio,bool stretchToOriginalFrame,bool horizontalFlip,bool verticalFlip,int sampleRate,MappingMethodCollection mappingMethod)
        {
            WidthSqueezeRate = widthSqueezeRate;
            MaintainAspectRatio = maintainAspectRatio;
            StretchToOriginalFrame = stretchToOriginalFrame;
            HorizontalFlip = horizontalFlip;
            VerticalFlip = verticalFlip;
            SampleRate = sampleRate;
            MappingMethod = mappingMethod;
        }
    }
}
