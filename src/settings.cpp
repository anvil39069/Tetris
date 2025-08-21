#include "settings.h"
#include <cstdio>

Settings::Settings()
{
    LoadSettings();
}

void Settings::SetDefaults()
{
    difficulty = Difficulty::MEDIUM;
    /*musicEnabled = true;
    soundEffectsEnabled = true;
    musicVolume = 0.7f;
    soundVolume = 0.8f;*/
}

void Settings::SaveSettings()
{
    FILE* file = fopen("settings.txt", "w");
    if (file != NULL)
    {
        fprintf(file, "%d\n", (int)difficulty);
        /*fprintf(file, "%d\n", musicEnabled ? 1 : 0);
        fprintf(file, "%d\n", soundEffectsEnabled ? 1 : 0);
        fprintf(file, "%.2f\n", musicVolume);
        fprintf(file, "%.2f\n", soundVolume);
        fclose(file);*/
    }
}

void Settings::LoadSettings()
{
    FILE* file = fopen("settings.txt", "r");
    if (file != NULL)
    {

        int diff, music, sound;
        fscanf(file, "%d", &diff);
        difficulty = (Difficulty)diff;
        /*fscanf(file, "%d", &music);
        fscanf(file, "%d", &sound);
        fscanf(file, "%f", &musicVolume);
        fscanf(file, "%f", &soundVolume);
        
        difficulty = (Difficulty)diff;
        musicEnabled = (music == 1);
        soundEffectsEnabled = (sound == 1)*/
        
        fclose(file);
    }
    else
    {
        SetDefaults();
    }
}

double Settings::GetGameSpeed()
{
    switch (difficulty)
    {
    case Difficulty::EASY:
        return 0.8;   // Very slow
    case Difficulty::MEDIUM:
        return 0.5;   // Normal speed
    case Difficulty::HARD:
        return 0.2;   // Fast
    case Difficulty::EXPERT:
        return 0.1;   // Very fast
    default:
        return 0.5;
    }
}

int Settings::GetStartingGarbageRows()
{
    switch (difficulty)
    {
    case Difficulty::EASY:
        return 0;     // No garbage
    case Difficulty::MEDIUM:
        return 0;     // No garbage
    case Difficulty::HARD:
        return 3;     // 3 rows of garbage
    case Difficulty::EXPERT:
        return 5;     // 5 rows of garbage
    default:
        return 0;
    }
}