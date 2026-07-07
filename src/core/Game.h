#pragma once
#include "GameState.h"
#include "ILevel.h"
#include "../ui/Menu.h"
#include "../ui/LobbyScreen.h"
#include <memory>

class Game {
public:
    void Init();
    void Update(float dt);
    void Draw();
    void Unload();

private:
    GameState state = GameState::MENU;
    std::unique_ptr<ILevel> currentLevel;
    Menu menu;
    LobbyScreen lobby;

    void StartLevel(GameState levelState);
    void HandleLevelResult();
};