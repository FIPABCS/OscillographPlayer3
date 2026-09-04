using OscillographPlayer.Helpers;
using System;
using System.Collections.Generic;
using System.Text;

namespace OscillographPlayer.ViewModels
{
    public partial class InfoPageViewModel:ViewModelBase
    {

        //View Text
        public static IObservable<string?> Title => LanguageHelper.Instance.InfoPage_Title;

        public static IObservable<string?> AppName => LanguageHelper.Instance.Global_AppName;
        public static IObservable<string?> LicenseInfo => LanguageHelper.Instance.Global_AppLicense;

        public static IObservable<string?> CurrentVersionCardTitle => LanguageHelper.Instance.InfoPage_CurrentVersionCardTitle;
        public static IObservable<string?> Version => LanguageHelper.Instance.Global_AppVersion;
        public static IObservable<string?> ViewRelease => LanguageHelper.Instance.InfoPage_ViewRelease;

        public static IObservable<string?> RepositoryCardTitle => LanguageHelper.Instance.InfoPage_RepositoryCardTitle;
        public static IObservable<string?> RepositoryCardDes => LanguageHelper.Instance.InfoPage_RepositoryCardDes;

        public static IObservable<string?> LicenseCardTitle => LanguageHelper.Instance.InfoPage_LicenseCardTitle;
        public static IObservable<string?> LicenseCardDes => LanguageHelper.Instance.InfoPage_LicenseCardDes;

        public static IObservable<string?> HelpAndFeedbackSubtitle => LanguageHelper.Instance.InfoPage_HelpAndFeedbackSubtitle;

        public static IObservable<string?> HelpCardTitle => LanguageHelper.Instance.InfoPage_HelpCardTitle;
        public static IObservable<string?> HelpCardDes => LanguageHelper.Instance.InfoPage_HelpCardDes;

        public static IObservable<string?> FeedbackCardTitle => LanguageHelper.Instance.InfoPage_FeedbackCardTitle;
        public static IObservable<string?> FeedBackCardDes => LanguageHelper.Instance.InfoPage_FeedbackCardDes;

        public static IObservable<string?> DependencyAndReferenceSubtitle => LanguageHelper.Instance.InfoPage_DependencyAndReferenceSubtitle;
    }
}
