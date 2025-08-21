#include <raylib.h>
#include "game.h"
#include "colors.h"
#include <iostream>

double lastUpdateTime = 0;

bool EventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

int main()
{
    InitWindow(500, 620, "raylib Tetris");
    SetTargetFPS(60);

    Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);

    Game game = Game();
    
    while (!WindowShouldClose())
    {
        game.HandleInput();
        
        // Only update game logic when playing
        if (game.gameState == GameState::PLAYING && EventTriggered(game.settings.GetGameSpeed()))
        {
            game.MoveBlockDown();
        }

        BeginDrawing();
        ClearBackground(darkBlue);
        
        // Only draw game UI when playing
        if (game.gameState == GameState::PLAYING)
        {
            DrawTextEx(font, "Score", {365, 15}, 38, 2, WHITE);
            DrawTextEx(font, "Next", {370, 175}, 38, 2, WHITE);

            if(!game.gameOver)
            {
            // Show pause instruction
            DrawTextEx(font,"Press P to Pause",{250,12},25, 20, GRAY);
            }

            char highScoreText[20];
            sprintf(highScoreText, "Best: %d", game.highScore);
            DrawText(highScoreText, 320, 120, 20, YELLOW);

            if (game.gameOver)
            {
                DrawTextEx(font, "GAME OVER", {320, 450}, 25, 2, WHITE);
                DrawTextEx(font, "Press any key for menu", {320, 480}, 12, 2, WHITE);
                
                if (game.score == game.highScore && game.score > 0)
                {
                    DrawText("NEW HIGH SCORE!",100, 400, 25, GOLD);
                }
            }
            
            DrawRectangleRounded({320, 55, 170, 60}, 0.3, 6, lightBlue);

            char scoreText[10];
            sprintf(scoreText, "%d", game.score);
            Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);

            DrawTextEx(font, scoreText, {320 + (170 - textSize.x) / 2, 65}, 38, 2, WHITE);
            DrawRectangleRounded({320, 215, 170, 180}, 0.3, 6, lightBlue);
        }

        game.Draw();
        //pause instructions
        game.Draw();
        if (game.gameState == GameState::PLAYING && !game.gameOver)
        {
            DrawText("Press P to Pause", 15, 15, 16, YELLOW);  // Top-left corner, yellow color
        }
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}