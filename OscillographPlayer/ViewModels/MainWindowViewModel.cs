using CommunityToolkit.Mvvm.ComponentModel;
using OscillographPlayer.Helpers;
using System;
using System.Collections.Generic;
using System.Security.Cryptography.X509Certificates;

namespace OscillographPlayer.ViewModels
{
    public partial class MainWindowViewModel : ViewModelBase
    {
        [ObservableProperty]
        public partial ViewModelBase? CurrentPage { get; set; }

        [ObservableProperty]
        public partial object? NavigateViewSeletedItem { get; set; }

        partial void OnNavigateViewSeletedItemChanged(object? value)
        {
            if(value is AvaloniaFluentUI.Controls.NavigationViewItem item)
            {
                ToggleToPage(item.Tag + "");
            }

            return;
        }

        private void ToggleToPage(string tag)
        {
            if(!pages.TryGetValue(tag, out ViewModelBase? pageViewModel))
            {
                throw (new Exception("No ViewModel Founded"));
            }

            CurrentPage = pageViewModel;

            return;
        }

        private Dictionary<string, ViewModelBase> pages;

        public MainWindowViewModel()
        {
            pages = new()
            {
                {"VideoPage",new VideoGeneratePageViewModel() },
                {"InfoPage",new InfoPageViewModel() },
                {"SettingsPage",new SettingsPageViewModel() },
            };
        }

        //View Texts

        public static IObservable<string?> AppTitle => LanguageHelper.Instance.Global_AppName;

        public static IObservable<string?> VideoPageName => LanguageHelper.Instance.VideoPage_Title;
        public static IObservable<string?> InfoPageName => LanguageHelper.Instance.InfoPage_Title;
        public static IObservable<string?> SettingsPageName => LanguageHelper.Instance.SettingsPage_Title;

    }
}
