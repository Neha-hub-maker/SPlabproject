#include "raylib.h"
#include "core/Game.h"
#include "core/FontManager.h"

int main() {
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(0, 0, "Squid Game");
    SetTargetFPS(60);

    // Load pixel font immediately after window init
    GetPixelFont();

    Game game;
    game.Init();

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        game.Update(dt);
        game.Draw();
    }

    game.Unload();
    UnloadFont(GetPixelFont());
    CloseWindow();
    return 0;
}