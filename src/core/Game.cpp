#include "Game.h"
#include "raylib.h"
#include "../levels/RedLightGreenLight.h"

void Game::Init() {
    state = GameState::MENU;
    currentLevel = nullptr;
}

void Game::StartLevel(GameState levelState) {
    state = levelState;
    if (levelState == GameState::LEVEL_1)
        currentLevel = std::make_unique<RedLightGreenLight>();
    if (currentLevel) currentLevel->Init();
}

void Game::HandleLevelResult() {
    if (!currentLevel) return;
    if (currentLevel->IsFailed()) {
        currentLevel->Unload();
        currentLevel.reset();
        state = GameState::GAME_OVER;
        return;
    }
    if (currentLevel->IsComplete()) {
        currentLevel->Unload();
        currentLevel.reset();
        switch (state) {
            case GameState::LEVEL_1: StartLevel(GameState::LEVEL_2); break;
            case GameState::LEVEL_2: StartLevel(GameState::LEVEL_3); break;
            case GameState::LEVEL_3: state = GameState::WIN; break;
            default: break;
        }
    }
}

void Game::Update(float dt) {
    switch (state) {
        case GameState::MENU:
            if (IsKeyPressed(KEY_ENTER)) state = GameState::LOBBY;
            if (IsKeyPressed(KEY_ESCAPE)) CloseWindow();
            break;
        case GameState::LOBBY:
            lobby.playPressed = false;
            lobby.Update();
            if (lobby.playPressed) StartLevel(GameState::LEVEL_1);
            if (IsKeyPressed(KEY_ESCAPE)) state = GameState::MENU;
            break;
        case GameState::LEVEL_1:
        case GameState::LEVEL_2:
        case GameState::LEVEL_3:
            if (currentLevel) currentLevel->Update(dt);
            HandleLevelResult();
            break;
        case GameState::GAME_OVER:
        case GameState::WIN:
            if (IsKeyPressed(KEY_ENTER)) {
                state = GameState::MENU;
                currentLevel.reset();
            }
            break;
    }
}

void Game::Draw() {
    int W = GetScreenWidth();
    int H = GetScreenHeight();
    BeginDrawing();

    switch (state) {
        case GameState::MENU:
            menu.Draw();
            break;
        case GameState::LOBBY:
            lobby.Draw();
            break;
        case GameState::LEVEL_1:
        case GameState::LEVEL_2:
        case GameState::LEVEL_3:
            ClearBackground(BLACK);
            if (currentLevel) currentLevel->Draw();
            break;
        case GameState::GAME_OVER:
            ClearBackground(BLACK);
            DrawText("GAME OVER", W/2 - 180, H/2 - 40, 60, RED);
            DrawText("Press ENTER to return to Menu", W/2 - 200, H/2 + 40, 24, WHITE);
            break;
        case GameState::WIN:
            ClearBackground(BLACK);
            DrawText("YOU WIN!", W/2 - 140, H/2 - 40, 60, GREEN);
            DrawText("Press ENTER to return to Menu", W/2 - 200, H/2 + 40, 24, WHITE);
            break;
    }

    EndDrawing();
}

void Game::Unload() {
    if (currentLevel) currentLevel->Unload();
}