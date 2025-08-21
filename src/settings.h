#pragma once

enum class Difficulty
{
    EASY,
    MEDIUM,
    HARD,
    EXPERT
};

class Settings
{
public:
    Settings();
    void SaveSettings();
    void LoadSettings();
    
    // Difficulty settings
    Difficulty difficulty;
    double GetGameSpeed();
    int GetStartingGarbageRows();
    
    /*// Audio settings
    bool musicEnabled;
    bool soundEffectsEnabled;
    float musicVolume;      // 0.0 to 1.0
    float soundVolume;      // 0.0 to 1.0
*/
private:
    void SetDefaults();
};