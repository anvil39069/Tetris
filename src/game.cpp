#include "game.h"
#include <random>

Game::Game()
{
    grid = Grid();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    gameOver = false;
    shouldQuit = false;
    score = 0;
    //InitAudioDevice();
    //music = LoadMusicStream("Sounds/music.mp3");
    //PlayMusicStream(music);
    //rotateSound = LoadSound("Sounds/rotate.mp3");
    //clearSound = LoadSound("Sounds/clear.mp3");
    gameState = GameState::MENU;
    selectedMenuItem = 0;  


    //  (0 = Start Game, 1 = Quit)
    LoadHighScore();  //  to load saved high score
    gameState = GameState::MENU;
    selectedMenuItem = 0;
    selectedPauseItem = 0;
    settings = Settings();       
    selectedSettingItem = 0;     
    
    // Apply difficulty settings
    ApplyStartingGarbage();      
    
    // Apply audio settings
    /*if (settings.musicEnabled)  
    {
        InitAudioDevice();
        music = LoadMusicStream("Sounds/music.mp3");
        SetMusicVolume(music, settings.musicVolume);
        PlayMusicStream(music);
        rotateSound = LoadSound("Sounds/rotate.mp3");
        clearSound = LoadSound("Sounds/clear.mp3");
        SetSoundVolume(rotateSound, settings.soundVolume);
        SetSoundVolume(clearSound, settings.soundVolume);
    }*/
}

Game::~Game()
{
    /*UnloadSound(rotateSound);
    UnloadSound(clearSound);
    UnloadMusicStream(music);
    CloseAudioDevice();*/
}

Block Game::GetRandomBlock()
{
    if (blocks.empty())
    {
        blocks = GetAllBlocks();
    }
    int randomIndex = rand() % blocks.size();
    Block block = blocks[randomIndex];
    blocks.erase(blocks.begin() + randomIndex);
    return block;
}

std::vector<Block> Game::GetAllBlocks()
{
    return {IBlock(), JBlock(), LBlock(), OBlock(), SBlock(), TBlock(), ZBlock()};
}

void Game::Draw()
{
    grid.Draw();  
    currentBlock.Draw(11, 11);
    switch (gameState)
    {

        //show menu
    case GameState::MENU:
        DrawMenu();
        break;

        //show playing
    case GameState::PLAYING:
        grid.Draw();
        currentBlock.Draw(11, 11);

        // Draw next block - this should be inside the PLAYING case
            switch (nextBlock.id)
            {
            case 3:  // I-block
                nextBlock.Draw(255, 290);
                break;
            case 4:  // O-block
                nextBlock.Draw(255, 280);
                break;
            default:
                nextBlock.Draw(270, 270);
                break;
            }
            break;  // Only one break for the PLAYING case

        //show settings
     case GameState::SETTINGS:     
        DrawSettingsMenu();
        break;

    case GameState::PAUSED:
        DrawPauseMenu();
        break;
        
    case GameState::GAME_OVER:
        DrawGameOver();
        break;
    }
}

//the menu
void Game::DrawMenu()
{
    // Draw title
    DrawText("TETRIS", 150, 100, 60, WHITE);

    // Draw high score
    char highScoreText[50];
    sprintf(highScoreText, "High Score: %d", highScore);
    DrawText(highScoreText, 150, 150, 25, YELLOW);

    // Draw menu options
    Color startColor = (selectedMenuItem == 0) ? YELLOW : WHITE;
    Color settingsColor = (selectedMenuItem == 1) ? YELLOW : WHITE;
    Color quitColor = (selectedMenuItem == 2) ? YELLOW : WHITE;
    
    //Menu Options
    DrawText("START GAME", 160, 250, 30, startColor);
    DrawText("SETTINGS", 170, 300, 30, settingsColor);
    DrawText("QUIT", 200, 350, 30, quitColor);
    
    // Draw instructions
    DrawText("Use UP/DOWN arrows to select", 110, 400, 20, GRAY);
    DrawText("Press ENTER to confirm", 140, 430, 20, GRAY);
}

//for controls
void Game::HandleInput()

{
    if (gameState == GameState::MENU)
    {
        HandleMenuInput();
        return;
    }
    
    if (gameState == GameState::SETTINGS)  // Add this block
    {
        HandleSettingsInput();
        return;
    }

     if (gameState == GameState::PAUSED)  // Add this block
    {
        HandlePauseInput();
        return;
    }

    // Handle game over state - return to menu
    int keyPressed = GetKeyPressed();
    if (gameOver && keyPressed != 0)
    {
        gameState = GameState::MENU;  // Go back to menu instead of resetting
        gameOver = false;
        return;
    }
    if (gameState == GameState::PLAYING){
    if (keyPressed == KEY_P || keyPressed == KEY_ESCAPE) 
    {
        gameState = GameState::PAUSED;
        return;
    }


    switch (keyPressed)
    {
    case KEY_LEFT:
        MoveBlockLeft();
        break;
    case KEY_RIGHT:
        MoveBlockRight();
        break;
    case KEY_DOWN:
        MoveBlockDown();
        UpdateScore(0, 1);
        break;
    case KEY_UP:
        RotateBlock();
        break;
    }
}
}

//MenuInputs
void Game::HandleMenuInput()
{
    int keyPressed = GetKeyPressed();
    
    switch (keyPressed)
    {
    case KEY_UP:
        selectedMenuItem--;
        if (selectedMenuItem < 0) selectedMenuItem = 2;
        break;
    case KEY_DOWN:
        selectedMenuItem++;
        if (selectedMenuItem > 2) selectedMenuItem = 0;
        break;
    case KEY_ENTER:
        if (selectedMenuItem == 0)
        {
            ApplyStartingGarbage();//apply difficulty when starting
            gameState = GameState::PLAYING;
            Reset(); // Start a fresh game
        }
        else if (selectedMenuItem == 1)
        {
            gameState = GameState::SETTINGS;
        }
        else if (selectedMenuItem == 2)  // Quit
        {
            // Check and update high score before quitting
            if (score > highScore)
            {
                highScore = score;
                SaveHighScore();
            }
            CloseWindow();
        }
        break;

        case KEY_ESCAPE:
        // ESC key highlights quit option
        selectedMenuItem = 2;  
        break;
        
            /*CloseWindow();// Quit game - you can handle this in main.cpp*/

}
}

void Game::DrawPauseMenu()
{
    // Draw the game in background (dimmed)
    grid.Draw();
    currentBlock.Draw(11, 11);
    DrawRectangle(0, 0, 500, 620, Fade(BLACK, 0.7f));
    
    // Draw pause menu
    DrawText("PAUSED", 180, 150, 50, WHITE);
    
    // Draw menu options
    Color continueColor = (selectedPauseItem == 0) ? YELLOW : WHITE;
    Color menuColor = (selectedPauseItem == 1) ? YELLOW : WHITE;
    Color quitColor = (selectedPauseItem == 2) ? YELLOW : WHITE;
    
    DrawText("CONTINUE", 170, 250, 30, continueColor);
    DrawText("RETURN TO MENU", 120, 300, 30, menuColor);
    DrawText("QUIT", 200, 350, 30, quitColor);
    
    // Draw instructions
    DrawText("Use UP/DOWN arrows to select", 110, 450, 20, GRAY);
    DrawText("Press ENTER to confirm", 140, 480, 20, GRAY);
    DrawText("Press P or ESC to resume", 130, 510, 20, GRAY);
}


void Game::HandlePauseInput()
{
    int keyPressed = GetKeyPressed();
    
    switch (keyPressed)
    {
    case KEY_UP:
        selectedPauseItem--;
        if (selectedPauseItem < 0) selectedPauseItem = 2;
        break;
    case KEY_DOWN:
        selectedPauseItem++;
        if (selectedPauseItem > 2) selectedPauseItem = 0;
        break;
    case KEY_ENTER:
        if (selectedPauseItem == 0)  // Continue
        {
            gameState = GameState::PLAYING;
        }
        else if (selectedPauseItem == 1)  // Return to Menu
        {
            // Check and update high score before returning to menu
             if (score > highScore)
            {
                highScore = score;
                SaveHighScore();
            }
            gameState = GameState::MENU;
            Reset();  // Reset the game
        }
        else if (selectedPauseItem == 2)  // Quit
        {
            // Check and update high score before quitting
            if (score > highScore)
            {
                highScore = score;
                SaveHighScore();
            }
            CloseWindow();
        }
        break;
    case KEY_P:
    case KEY_ESCAPE:
        gameState = GameState::PLAYING;  // Resume game
        break;
    }
}

void Game::HandleSettingsInput()
{
    int keyPressed = GetKeyPressed();
    
    switch (keyPressed)
    {
    case KEY_UP:
        selectedSettingItem--;
        if (selectedSettingItem < 0) selectedSettingItem = 0; 
        break;
    case KEY_DOWN:
        selectedSettingItem++;
        if (selectedSettingItem >0) selectedSettingItem = 0;
        break;
    case KEY_LEFT:
    case KEY_RIGHT:
        // Handle setting changes
        if (selectedSettingItem == 0)  // Difficulty
        {
            int diff = (int)settings.difficulty;
            if (keyPressed == KEY_RIGHT)
                diff = (diff + 1) % 4;
            else
                diff = (diff - 1 + 4) % 4;
            settings.difficulty = (Difficulty)diff;
        }
        /*else if (selectedSettingItem == 1)  // Music
        {
            settings.musicEnabled = !settings.musicEnabled;
        }
        else if (selectedSettingItem == 2)  // Sound Effects
        {
            settings.soundEffectsEnabled = !settings.soundEffectsEnabled;
        }
        else if (selectedSettingItem == 3)  // Music Volume
        {
            if (keyPressed == KEY_RIGHT)
                settings.musicVolume = (settings.musicVolume + 0.1f > 1.0f) ? 1.0f : settings.musicVolume + 0.1f;
            else
                settings.musicVolume = (settings.musicVolume - 0.1f < 0.0f) ? 0.0f : settings.musicVolume - 0.1f;
        }
        else if (selectedSettingItem == 4)  // Sound Volume
        {
            if (keyPressed == KEY_RIGHT)
                settings.soundVolume = (settings.soundVolume + 0.1f > 1.0f) ? 1.0f : settings.soundVolume + 0.1f;
            else
                settings.soundVolume = (settings.soundVolume - 0.1f < 0.0f) ? 0.0f : settings.soundVolume - 0.1f;
        }*/
        break;
        case KEY_ENTER:
        case KEY_ESCAPE:
        gameState = GameState::MENU;
        break;
    }
}

void Game::DrawSettingsMenu()
{
    DrawText("DIFFICULTY", 80, 50, 50, WHITE);
    
    // Difficulty setting
    const char* diffNames[] = {"EASY", "MEDIUM", "HARD", "EXPERT"};
    Color diffColor = (selectedSettingItem == 0) ? YELLOW : WHITE;
    char diffText[100];
    sprintf(diffText, "Difficulty: %s", diffNames[(int)settings.difficulty]);
    DrawText(diffText, 50, 120, 25, diffColor);
    
    /* Music setting
    Color musicColor = (selectedSettingItem == 1) ? YELLOW : WHITE;
    char musicText[50];
    sprintf(musicText, "Music: %s", settings.musicEnabled ? "ON" : "OFF");
    DrawText(musicText, 50, 160, 25, musicColor);
    
    // Sound Effects setting
    Color soundColor = (selectedSettingItem == 2) ? YELLOW : WHITE;
    char soundText[50];
    sprintf(soundText, "Sound Effects: %s", settings.soundEffectsEnabled ? "ON" : "OFF");
    DrawText(soundText, 50, 200, 25, soundColor);
    
    // Music Volume
    Color musicVolColor = (selectedSettingItem == 3) ? YELLOW : WHITE;
    char musicVolText[50];
    sprintf(musicVolText, "Music Volume: %.0f%%", settings.musicVolume * 100);
    DrawText(musicVolText, 50, 240, 25, musicVolColor);
    
    // Sound Volume
    Color soundVolColor = (selectedSettingItem == 4) ? YELLOW : WHITE;
    char soundVolText[50];
    sprintf(soundVolText, "Sound Volume: %.0f%%", settings.soundVolume * 100);
    DrawText(soundVolText, 50, 280, 25, soundVolColor);
    */
    
    // Instructions
    DrawText("Use LEFT/RIGHT to change values", 50, 350, 20, GRAY);
    DrawText("Press ENTER to return", 50, 410, 20, GRAY);

}

void Game::ApplyStartingGarbage()
{
    int garbageRows = settings.GetStartingGarbageRows();
    
    // Clear the grid first
    grid.Initialize();
    
    // Add garbage rows at the bottom
    for (int row = 20 - garbageRows; row < 20; row++)
    {
        for (int col = 0; col < 10; col++)
        {
            // Create random garbage pattern with some gaps
            if (rand() % 4 != 0)  // 75% chance of filled cell
            {
                grid.grid[row][col] = 1 + (rand() % 7);  // Random colored block
            }
        }
    }
}

void Game::DrawGameOver()
{
    // Draw the game in background (dimmed)
    grid.Draw();
    DrawRectangle(0, 0, 500, 620, Fade(BLACK, 0.7f));
    
    // Draw game over text
    DrawText("GAME OVER", 120, 200, 40, RED);
    
    char scoreText[50];
    sprintf(scoreText, "Final Score: %d", score);
    DrawText(scoreText, 140, 260, 25, WHITE);

     char highScoreText[50];
    sprintf(highScoreText, "High Score: %d", highScore);
    DrawText(highScoreText, 150, 290, 25, YELLOW);
    
    if (score == highScore && score > 0)
    {
        DrawText("NEW HIGH SCORE!", 130, 320, 25, GOLD);
    }
    
    DrawText("Press any key to return to menu", 80, 350, 20, GRAY);
}

void Game::SaveHighScore()
{
    FILE* file = fopen("highscore.txt", "w");
    if (file != NULL)
    {
        fprintf(file, "%d", highScore);
        fclose(file);
    }
}

void Game::LoadHighScore()
{
    FILE* file = fopen("highscore.txt", "r");
    if (file != NULL)
    {
        fscanf(file, "%d", &highScore);
        fclose(file);
    }
    else
    {
        highScore = 0;  // Default if no file exists
    }
}

//gaame movements

void Game::MoveBlockLeft()
{
    if (!gameOver)
    {
        currentBlock.Move(0, -1);
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(0, 1);
        }
    }
}

void Game::MoveBlockRight()
{
    if (!gameOver)
    {
        currentBlock.Move(0, 1);
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(0, -1);
        }
    }
}

void Game::MoveBlockDown()
{
    if (!gameOver)
    {
        currentBlock.Move(1, 0);
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(-1, 0);
            LockBlock();
        }
    }
}

bool Game::IsBlockOutside()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        if (grid.IsCellOutside(item.row, item.column))
        {
            return true;
        }
    }
    return false;
}

void Game::RotateBlock()
{
    if (!gameOver)
    {
        currentBlock.Rotate();
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.UndoRotation();
        }
        /*else
        {
            PlaySound(rotateSound);
        }*/
    }
}

void Game::LockBlock()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        grid.grid[item.row][item.column] = currentBlock.id;
    }
    currentBlock = nextBlock;
    if (BlockFits() == false)
    {
        gameOver = true;
        // Check for new high score
        if (score > highScore)
        {
            highScore = score;
            SaveHighScore();  // Save immediately when new high score is set
        }
    }
    nextBlock = GetRandomBlock();
    int rowsCleared = grid.ClearFullRows();
    if (rowsCleared > 0)
    {
        //PlaySound(clearSound);
        UpdateScore(rowsCleared, 0);
    }
}

bool Game::BlockFits()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        if (grid.IsCellEmpty(item.row, item.column) == false)
        {
            return false;
        }
    }
    return true;
}

void Game::Reset()
{
    grid.Initialize();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    score = 0;
}

void Game::UpdateScore(int linesCleared, int moveDownPoints)
{
    switch (linesCleared)
    {
    case 1:
        score += 100;
        break;
    case 2:
        score += 300;
        break;
    case 3:
        score += 500;
        break;
    default:
        break;
    }

    score += moveDownPoints;
}
