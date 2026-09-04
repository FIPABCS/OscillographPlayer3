using OscillographPlayer.Helpers;
using System;
using System.Collections.Generic;
using System.Text;

namespace OscillographPlayer.ViewModels
{
    public partial class VideoGeneratePageViewModel:ViewModelBase
    {

        //View Text

        public static IObservable<string?> Title => LanguageHelper.Instance.VideoPage_Title;

        public static IObservable<string?> SourceFileTitle => LanguageHelper.Instance.VideoPage_SourceFileCardTitle;
        public static IObservable<string?> SourceFileDes => LanguageHelper.Instance.VideoPage_SourceFileCardDes;
        public static IObservable<string?> SourceFileOpenFile => LanguageHelper.Instance.VideoPage_SourceFileCard_OpenFile;

        public static IObservable<string?> OutputFileTitle => LanguageHelper.Instance.VideoPage_OutputFileCardTitle;
        public static IObservable<string?> OutputFileDes => LanguageHelper.Instance.VideoPage_OutputFileCardDes;
        public static IObservable<string?> OutputFileOpenFloder => LanguageHelper.Instance.VideoPage_OutputFileCard_OpenFloder;

        public static IObservable<string?> VideoProgessSubtitle => LanguageHelper.Instance.VideoPage_VideoProcessingProfileSubitle;

        public static IObservable<string?> AverangeSampleStepLengthTitle => LanguageHelper.Instance.VideoPage_AverangeSampleStepLengthCardTitle;
        public static IObservable<string?> AverangeSampleStepLengthDes => LanguageHelper.Instance.VideoPage_AverangeSampleStepLengthCardDes;

        public static IObservable<string?> EdgeJudgementTitle => LanguageHelper.Instance.VideoPage_EdgeJudgementCardTitle;
        public static IObservable<string?> EdgeJudgementDes => LanguageHelper.Instance.VideoPage_EdgeJudgementCardDes;

        public static IObservable<string?> HighThresholdTitle => LanguageHelper.Instance.VideoPage_HighThresholdCardTitle;
        public static IObservable<string?> HighThresholdDes => LanguageHelper.Instance.VideoPage_HighThresholdCardDes;

        public static IObservable<string?> LowThresholdTitle => LanguageHelper.Instance.VideoPage_LowThresholdCardTitle;
        public static IObservable<string?> LowThresholdDes => LanguageHelper.Instance.VideoPage_LowThresholdCardDes;

        public static IObservable<string?> ScreenFlipTitle => LanguageHelper.Instance.VideoPage_ScreenFlipCardTitle;
        public static IObservable<string?> ScreenFlipDes => LanguageHelper.Instance.VideoPage_ScreenFlipCardDes;

        public static IObservable<string?> VerticalFlipTitle => LanguageHelper.Instance.VideoPage_VerticalFlipCardTitle;
        public static IObservable<string?> VerticalFlipDes => LanguageHelper.Instance.VideoPage_VerticalFlipCardDes;

        public static IObservable<string?> HorizontalFlipTitle => LanguageHelper.Instance.VideoPage_HorizontalFlipCardTitle;
        public static IObservable<string?> HorizontalFlipDes => LanguageHelper.Instance.VideoPage_HorizontalFlipCardDes;

        public static IObservable<string?> WaveGenerateSubtitle => LanguageHelper.Instance.VideoPage_WaveGenerateProfileSubtitle;

        public static IObservable<string?> SampleRateTitle => LanguageHelper.Instance.VideoPage_SampleRateCardTitle;
        public static IObservable<string?> SampleRateDes =>  LanguageHelper.Instance.VideoPage_SampleRateCardDes;

        public static IObservable<string?> SampleRepeatMethodTitle => LanguageHelper.Instance.VideoPage_SampleRepeatMethodCardTitle;
        public static IObservable<string?> SampleRepeatMethodDes => LanguageHelper.Instance.VideoPage_SampleRepeatMethodCardDes;

        public static IObservable<string?> AmplitudeMaximizeTitle =>  LanguageHelper.Instance.VideoPage_AmplitudeMaximizeCardTitle;
        public static IObservable<string?> AmplitudeMaximizeDes => LanguageHelper.Instance.VideoPage_AmplitudeMaximizeCardDes;

        public static IObservable<string?> ProfileAndGenerateSubtitle => LanguageHelper.Instance.VideoPage_ProfileAndGenerateSubtitle;

        public static IObservable<string?> ProfileManagementTitle => LanguageHelper.Instance.VideoPage_ProfileManageCardTitle;
        public static IObservable<string?> ProfileManagementDes => LanguageHelper.Instance.VideoPage_ProfileManageCardDes;

        public static IObservable<string?> ImportAndExportProfileTitle => LanguageHelper.Instance.VideoPage_ImportAndExportProfileCardTitle;
        public static IObservable<string?> ImportAndExportProfileDes => LanguageHelper.Instance.VideoPage_ImportAndExportProfileCardDes;
        public static IObservable<string?> ImportProfile => LanguageHelper.Instance.VideoPage_ImportProfile;
        public static IObservable<string?> ExportProfile =>  LanguageHelper.Instance.VideoPage_ExportProfile;

        public static IObservable<string?> DefauleProfileTitle => LanguageHelper.Instance.VideoPage_DefaultProfileCardTitle;
        public static IObservable<string?> DefauleProfileDes => LanguageHelper.Instance.VideoPage_DefaultProfileCardDes;
        public static IObservable<string?> DefaultProfileStr => LanguageHelper.Instance.VideoPage_DefaultProfile;

        public static IObservable<string?> StartGenerateTitle => LanguageHelper.Instance.VideoPage_StartGenerateCardTitle;
        public static IObservable<string?> StartGenerateDes => LanguageHelper.Instance.VideoPage_StartGenerateCardDes;

        public static IObservable<string?> StartGenerate => LanguageHelper.Instance.VideoPage_StartGenerate;
    }
}
