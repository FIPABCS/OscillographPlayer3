using CommunityToolkit.Mvvm.ComponentModel;
using System;
using System.Collections.Generic;

namespace OscillographPlayer.ViewModels
{
    public partial class MainWindowViewModel : ViewModelBase
    {
        [ObservableProperty]
        private ViewModelBase? _currentPage;

        [ObservableProperty]
        private object? _navigateViewSeletedItem;
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
    }
}
