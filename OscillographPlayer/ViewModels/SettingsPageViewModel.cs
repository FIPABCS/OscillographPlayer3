using CommunityToolkit.Mvvm.ComponentModel;
using OscillographPlayer.Helpers;
using System;
using System.Collections.Generic;
using System.Text;
using System.Globalization;
using Irihi.Lingua;

namespace OscillographPlayer.ViewModels
{
    public partial class SettingsPageViewModel : ViewModelBase
    {
        //Language Option
        [ObservableProperty]
        private List<LinguaCulture> _languageList = new List<LinguaCulture>()
        {
            new(){Culture=new CultureInfo("zh-CN"),DisplayName="中文 (简体, 中国大陆)"},
            new(){Culture=new CultureInfo("en-US"),DisplayName="English (United States)"}
        };

        [ObservableProperty]
        private LinguaCulture _selectedLanguage;

        partial void OnSelectedLanguageChanged(LinguaCulture value)
        {
            try
            { 
                LanguageHelper.Instance.UpdateCulture(value.Culture); 
            }
            catch
            {
                Console.WriteLine($"Fali to update culture with \"{value.Culture.Name}\"");
                LanguageHelper.Instance.UpdateCulture(CultureInfo.InvariantCulture);
            }
        }

        //Theme Option


        //View Text
        public IObservable<string?> PageTitle=>LanguageHelper.Instance.SettingsPage_Title;
        
        public IObservable<string?> LanguageSubtitle=>LanguageHelper.Instance.SettingsPage_LanguageSubtitle;
        
        public IObservable<string?> LanguageCardTitle=>LanguageHelper.Instance.SettingsPage_LanguageCardTitle;
        public IObservable<string?> LanguageCardDes => LanguageHelper.Instance.SettingsPage_LanguageCardDes;
        
        public IObservable<string?> AppearanceSubtitlt=>LanguageHelper.Instance.SettingsPage_AppearanceSubtitle;
        
        public IObservable<string?> ThemeCardTitle=>LanguageHelper.Instance.SettingsPage_ThemeCardTitle;
        public IObservable<string?> ThemeCardDes => LanguageHelper.Instance.SettingsPage_ThemeCardDes;

        public SettingsPageViewModel()
        {
            SelectedLanguage = LanguageList[0];
        }
    }


}
