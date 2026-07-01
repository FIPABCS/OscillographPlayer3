using FFMediaToolkit.Decoding;
using FFMediaToolkit.Graphics;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace OscillographPlayer.Services
{
    public static class VideoServices
    {
        public static MediaFile OpenAndDecodeVideo(ref string path)
        {
            MediaFile video;
            
            try
            {
                MediaOptions options = new() { VideoPixelFormat = ImagePixelFormat.Gray8 };

                video = MediaFile.Open(path, options);
            }
            catch(Exception ex)
            {
                throw new Exception($"Fail Open Video File: {ex.Message}");
            }

            return video;
        }

        public static bool VideoStreamExist(ref MediaFile video)
        {
            if(video!= null&&video.Video.Info.NumberOfFrames!=0)
            {
                return true;
            }

            return false;
        }

    }
}
