#pragma once
#include "grid.h"
#include "blocks.cpp"
#include "settings.h"

//to create gamestate
enum class GameState
{
    MENU,
    PLAYING,
    GAME_OVER,
    PAUSED,
    SETTINGS
};

class Game
{
public:
    Game();
    ~Game();
    void Draw();
    void HandleInput();
    void MoveBlockDown();
    bool gameOver;
    int score;
    //Music music;
    GameState gameState;
    int selectedMenuItem;
    int selectedPauseItem;//for pause
    int highScore;
    Settings settings;  //for settings         
    int selectedSettingItem;
    bool shouldQuit;

private:
    void MoveBlockLeft();
    void MoveBlockRight();
    Block GetRandomBlock();
    std::vector<Block> GetAllBlocks();
    bool IsBlockOutside();
    void RotateBlock();
    void LockBlock();
    bool BlockFits();
    void Reset();
    void UpdateScore(int linesCleared, int moveDownPoints);
    Grid grid;
    std::vector<Block> blocks;
    Block currentBlock;
    Block nextBlock;
    //Sound rotateSound;
    //Sound clearSound;
    void DrawMenu();
    void HandleMenuInput();
    void DrawGameOver();
    void DrawPauseMenu();      
    void HandlePauseInput();
    void SaveHighScore();    
    void LoadHighScore();
    void DrawSettingsMenu();     
    void HandleSettingsInput();
    void ApplyStartingGarbage(); 
};

