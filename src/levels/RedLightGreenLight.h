#pragma once
#include "../core/ILevel.h"
#include "../core/FontManager.h"
#include "raylib.h"

class RedLightGreenLight : public ILevel {
public:
    void Init() override;
    void Update(float dt) override;
    void Draw() override;
    bool IsComplete() override;
    bool IsFailed() override;
    void Unload() override;

private:
    // Player settings
    float playerX = 0;
    float playerY = 0; // feet baseline
    float playerSpeed = 220.0f;
    bool isMoving = false;
    float legAnim = 0; // leg animation / bobbing timer

    // Game state
    bool greenLight = true;
    bool failed = false;
    bool complete = false;
    bool caughtMoving = false; // flag when caught red-handed

    // Doll light timers
    float lightTimer = 0;
    float lightDuration = 0;

    // Game timer
    float timeLeft = 60.0f;

    // Screen dimensions and baseline
    int W = 0;
    int H = 0;
    float groundY = 0; // shared baseline Y for player, doll, tree, etc.

    // Positions of lines
    float startX = 0;
    float finishX = 0;

    // Flash alpha when caught
    float flashAlpha = 0;

    // Sprite Textures
    Texture2D texPlayer;
    Texture2D texDollFront;
    Texture2D texDollBack;
    Texture2D texTree;
    Texture2D texHouse;

    // Helper to load and key out magenta background
    Texture2D LoadSprite(const char* path);

    void DrawBackground();
    void DrawPlayer();
    void DrawDoll();
    void DrawHUD();
    void DrawStatusBar();
    void RandomizeLightDuration();
};