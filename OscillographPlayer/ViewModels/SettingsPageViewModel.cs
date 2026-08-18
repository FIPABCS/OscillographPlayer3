using CommunityToolkit.Mvvm.ComponentModel;
using OscillographPlayer.Helpers;
using System;
using System.Collections.Generic;
using System.Text;
using System.Globalization;
using Irihi.Lingua;
using Avalonia.Styling;
using System.Drawing;
using AvaloniaFluentUI.Styling;

namespace OscillographPlayer.ViewModels
{
    public partial class SettingsPageViewModel : ViewModelBase
    {
        //Language Option
        [ObservableProperty]
        public partial List<LinguaCulture> LanguageList { get; set; } =
        [
            new(){Culture=new CultureInfo("zh-CN"),DisplayName="中文 (简体, 中国大陆)"},
            new(){Culture=new CultureInfo("en-US"),DisplayName="English (United States)"}
        ];

        [ObservableProperty]
        public partial LinguaCulture SelectedLanguage { get; set; }

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
        public record class ThemeOption 
        {
            public required ThemeVariant Theme { get; init; }
            public required IObservable<string?> Description { get; init; }
        }

        [ObservableProperty]
        public partial List<ThemeOption> ThemeOptions { get; set; } =
        [
            new () {Theme=ThemeVariant.Default,Description=LanguageHelper.Instance.SettingsPage_ThemeOption_System},
            new () {Theme=ThemeVariant.Light,Description=LanguageHelper.Instance.SettingsPage_ThemeOption_Light},
            new () {Theme=ThemeVariant.Dark,Description=LanguageHelper.Instance.SettingsPage_ThemeOption_Dark},
        ];
        [ObservableProperty]
        public partial ThemeOption SelectedThemeOption { get; set; }

        partial void OnSelectedThemeOptionChanged(ThemeOption value)
        {
            AvaloniaFluentTheme.Instance.CurrentTheme = value.Theme;
        }

        //View Text
        public static IObservable<string?> PageTitle=>LanguageHelper.Instance.SettingsPage_Title;

        public static IObservable<string?> LanguageSubtitle=>LanguageHelper.Instance.SettingsPage_LanguageSubtitle;

        public static IObservable<string?> LanguageCardTitle=>LanguageHelper.Instance.SettingsPage_LanguageCardTitle;
        public static IObservable<string?> LanguageCardDes => LanguageHelper.Instance.SettingsPage_LanguageCardDes;
        
        public static IObservable<string?> AppearanceSubtitlt=>LanguageHelper.Instance.SettingsPage_AppearanceSubtitle;

        public static IObservable<string?> ThemeCardTitle=>LanguageHelper.Instance.SettingsPage_ThemeCardTitle;
        public static IObservable<string?> ThemeCardDes => LanguageHelper.Instance.SettingsPage_ThemeCardDes;

        public SettingsPageViewModel()
        {
            SelectedLanguage = LanguageList[0];
            SelectedThemeOption = ThemeOptions[0];
        }
    }


}
