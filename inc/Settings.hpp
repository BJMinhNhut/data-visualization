#ifndef SETTINGS_HPP
#define SETTINGS_HPP

struct Settings {
    enum Themes { Light, Dark };
    Themes theme;

    Settings();
    void print();
    bool operator==(const Settings& settings) const;
    bool operator!=(const Settings& settings) const;
};

const Settings getSettings();
void updateSettings(const Settings& settings);

#endif  // SETTINGS_HPP