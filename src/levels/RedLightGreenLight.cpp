#include "RedLightGreenLight.h"
#include <cstdio>
#include <cmath>

Texture2D RedLightGreenLight::LoadSprite(const char* path) {
    // PNGs already have transparency baked in by the build pipeline
    Texture2D tex = LoadTexture(path);
    if (tex.id > 0) {
        SetTextureFilter(tex, TEXTURE_FILTER_POINT);
    }
    return tex;
}

void RedLightGreenLight::Init() {
    W = GetScreenWidth();
    H = GetScreenHeight();

    // Load sprite assets
    texPlayer = LoadSprite("assets/textures/player_run.png");
    texDollFront = LoadSprite("assets/textures/doll_front.png");
    texDollBack = LoadSprite("assets/textures/doll_back.png");
    texTree = LoadSprite("assets/textures/dead_tree.png");
    texHouse = LoadSprite("assets/textures/house.png");

    // Dynamic ground baseline positioning (bottom-anchored)
    groundY = H * 0.82f;
    startX = W * 0.15f;
    finishX = W * 0.80f;

    playerX = startX;
    playerY = groundY;

    greenLight = true;
    failed = false;
    complete = false;
    caughtMoving = false;
    isMoving = false;

    timeLeft = 60.0f;
    lightTimer = 0.0f;
    legAnim = 0.0f;
    flashAlpha = 0.0f;

    RandomizeLightDuration();
}

void RedLightGreenLight::RandomizeLightDuration() {
    lightDuration = GetRandomValue(150, 500) / 100.0f;
    lightTimer = 0.0f;
}

void RedLightGreenLight::Update(float dt) {
    if (failed || complete) {
        if (failed && caughtMoving) {
            flashAlpha += dt * 3.0f;
        }
        return;
    }

    timeLeft -= dt;
    if (timeLeft <= 0.0f) {
        timeLeft = 0.0f;
        failed = true;
        return;
    }

    lightTimer += dt;
    if (lightTimer >= lightDuration) {
        lightTimer = 0.0f;
        greenLight = !greenLight;
        RandomizeLightDuration();
    }

    isMoving = false;

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        isMoving = true;
        
        // Caught moving during RED LIGHT
        if (!greenLight) {
            caughtMoving = true;
            failed = true;
            return;
        }

        playerX += playerSpeed * dt;
        legAnim += dt * 14.0f;

        if (playerX >= finishX) {
            playerX = finishX;
            complete = true;
        }
    }
}

void RedLightGreenLight::DrawBackground() {
    float skyHeight = H * 0.45f;
    float wheatHeight = H * 0.35f;
    float grassHeight = H * 0.05f;
    float horizonY = skyHeight + wheatHeight;

    // 1. Sky (light blue gradient-like color)
    DrawRectangle(0, 0, W, (int)skyHeight, { 135, 206, 235, 255 });
    // Decorative pixel clouds
    DrawRectangle((int)(W * 0.12f), (int)(skyHeight * 0.4f), 80, 20, WHITE);
    DrawRectangle((int)(W * 0.14f), (int)(skyHeight * 0.35f), 50, 30, WHITE);
    DrawRectangle((int)(W * 0.45f), (int)(skyHeight * 0.5f), 100, 22, WHITE);
    DrawRectangle((int)(W * 0.75f), (int)(skyHeight * 0.3f), 90, 20, WHITE);

    // 2. Wheat field band (dense yellow/gold wheat stalks)
    DrawRectangle(0, (int)skyHeight, W, (int)wheatHeight, { 215, 175, 45, 255 });
    // Draw textured wheat stalks
    for (int x = 0; x < W; x += 8) {
        int stalkH = 30 + (x % 5) * 5;
        DrawLine(x, (int)horizonY, x, (int)(horizonY - stalkH), { 190, 150, 35, 255 });
        DrawRectangle(x - 1, (int)(horizonY - stalkH), 3, 3, { 235, 200, 60, 255 });
    }

    // 3. Chain-link fence behind/near the doll on the grass line
    float fenceH = H * 0.08f;
    float fenceY = horizonY;
    for (int x = 0; x < W; x += 40) {
        // Vertical fence posts
        DrawRectangle(x, (int)(fenceY - fenceH), 3, (int)fenceH, { 100, 100, 100, 255 });
        // Criss-cross wires
        DrawLine(x, (int)fenceY, x + 40, (int)(fenceY - fenceH), { 85, 85, 85, 255 });
        DrawLine(x, (int)(fenceY - fenceH), x + 40, (int)fenceY, { 85, 85, 85, 255 });
    }

    // 4. Grass strip (green with blades texture)
    DrawRectangle(0, (int)horizonY, W, (int)grassHeight, { 45, 165, 45, 255 });
    for (int x = 0; x < W; x += 5) {
        int bladeH = 3 + (x % 4) * 2;
        DrawLine(x, (int)horizonY, x, (int)(horizonY + bladeH), { 30, 120, 30, 255 });
    }

    // 5. Dirt ground (brown)
    DrawRectangle(0, (int)(horizonY + grassHeight), W, H - (int)(horizonY + grassHeight), { 120, 60, 25, 255 });

    // 6. Two detailed houses on the left background (sitting on horizon line)
    if (texHouse.id > 0) {
        float aspect = (float)texHouse.width / (float)texHouse.height;
        
        // House 1 (leftmost)
        float h1Height = H * 0.22f;
        float h1Width = h1Height * aspect;
        DrawTexturePro(texHouse, { 0, 0, (float)texHouse.width, (float)texHouse.height },
                       { W * 0.06f, horizonY - h1Height + 5, h1Width, h1Height }, { 0, 0 }, 0.0f, WHITE);

        // House 2 (smaller, slightly right)
        float h2Height = H * 0.17f;
        float h2Width = h2Height * aspect;
        DrawTexturePro(texHouse, { 0, 0, (float)texHouse.width, (float)texHouse.height },
                       { W * 0.22f, horizonY - h2Height + 5, h2Width, h2Height }, { 0, 0 }, 0.0f, WHITE);
    }

    // 7. Two dark shadow silhouettes (using player sprite filled with semi-transparent dark shade)
    if (texPlayer.id > 0) {
        float aspect = (float)texPlayer.width / (float)texPlayer.height;
        float sh = H * 0.12f;
        float sw = sh * aspect;
        Color shadowColor = { 35, 25, 20, 150 };

        // Silhouette 1
        DrawTexturePro(texPlayer, { 0, 0, (float)texPlayer.width, (float)texPlayer.height },
                       { W * 0.03f, horizonY - sh + 5, sw, sh }, { 0, 0 }, 0.0f, shadowColor);
        // Silhouette 2
        DrawTexturePro(texPlayer, { 0, 0, (float)texPlayer.width, (float)texPlayer.height },
                       { W * 0.17f, horizonY - sh + 5, sw, sh }, { 0, 0 }, 0.0f, shadowColor);
    }

    // 8. Dead tree on the right side behind/near the doll (anchored to groundY walking line)
    if (texTree.id > 0) {
        float treeX = W * 0.90f;
        float aspect = (float)texTree.width / (float)texTree.height;
        float th = H * 0.48f;
        float tw = th * aspect;
        DrawTexturePro(texTree, { 0, 0, (float)texTree.width, (float)texTree.height },
                       { treeX - tw / 2.0f, groundY - th + 10, tw, th }, { 0, 0 }, 0.0f, WHITE);
    }

    // 9. START line and label
    DrawLineEx({ startX, skyHeight }, { startX, horizonY + grassHeight }, 3, { 80, 180, 240, 255 });
    int startTxtW = MeasurePixelText("START", 12);
    DrawPixelText("START", (int)(startX - startTxtW / 2), (int)(skyHeight - 20), 12, { 80, 180, 240, 255 });

    // 10. FINISH line and label
    DrawLineEx({ finishX, skyHeight }, { finishX, horizonY + grassHeight }, 3, { 245, 140, 30, 255 });
    int finishTxtW = MeasurePixelText("FINISH", 12);
    DrawPixelText("FINISH", (int)(finishX - finishTxtW / 2), (int)(skyHeight - 20), 12, { 245, 140, 30, 255 });
}

void RedLightGreenLight::DrawDoll() {
    Texture2D currentDollTex = greenLight ? texDollBack : texDollFront;
    if (currentDollTex.id > 0) {
        float aspect = (float)currentDollTex.width / (float)currentDollTex.height;
        float dh = H * 0.32f; // giant doll, visibly taller
        float dw = dh * aspect;
        float dx = finishX + dw * 0.5f; // positioned just past finish line
        
        // Feet anchored exactly to groundY walking line
        DrawTexturePro(currentDollTex, { 0, 0, (float)currentDollTex.width, (float)currentDollTex.height },
                       { dx, groundY, dw, dh }, { dw / 2.0f, dh }, 0.0f, WHITE);
    }
}

void RedLightGreenLight::DrawPlayer() {
    if (texPlayer.id > 0) {
        float px = playerX;
        float py = groundY;

        float aspect = (float)texPlayer.width / (float)texPlayer.height;
        float ph = H * 0.13f;
        float pw = ph * aspect;

        // Bobbing & tilting animation when walking
        float bob = 0.0f;
        float angle = 0.0f;
        if (isMoving && greenLight) {
            bob = std::abs(std::sin(legAnim)) * (H * 0.012f);
            angle = std::sin(legAnim) * 6.0f;
        }

        // Draw with rotation origin at bottom center (feet) so it tilts organically
        DrawTexturePro(texPlayer, { 0, 0, (float)texPlayer.width, (float)texPlayer.height },
                       { px, py - bob, pw, ph }, { pw / 2.0f, ph }, angle, WHITE);
    }
}

void RedLightGreenLight::DrawHUD() {
    DrawRectangle(0, 0, W, 80, { 15, 15, 15, 220 });

    char timerStr[32];
    snprintf(timerStr, sizeof(timerStr), "Time: %d", (int)timeLeft);
    DrawPixelText(timerStr, 20, 14, 18, WHITE);
    DrawPixelText("Level 1 - Red Light, Green Light", 20, 36, 10, { 200, 200, 200, 255 });
    DrawPixelText("A/D or Arrows to move. Freeze on RED. Esc to quit.", 20, 52, 8, { 145, 145, 145, 255 });

    // Center RED LIGHT / GREEN LIGHT Banner
    float bannerW = 280.0f;
    float bannerH = 52.0f;
    float bannerX = W / 2.0f - bannerW / 2.0f;
    float bannerY = 14.0f;

    DrawRectangleLines((int)bannerX, (int)bannerY, (int)bannerW, (int)bannerH, { 80, 80, 80, 255 });
    
    if (greenLight) {
        DrawRectangle((int)bannerX + 4, (int)bannerY + 4, (int)bannerW - 8, (int)bannerH - 8, { 20, 85, 25, 255 });
        DrawRectangleLines((int)bannerX + 2, (int)bannerY + 2, (int)bannerW - 4, (int)bannerH - 4, { 50, 240, 60, 255 });
        const char* greenText = "GREEN LIGHT";
        int gw = MeasurePixelText(greenText, 18);
        DrawPixelText(greenText, (int)(W / 2 - gw / 2), (int)(bannerY + 17), 18, { 60, 255, 70, 255 });
    } else {
        DrawRectangle((int)bannerX + 4, (int)bannerY + 4, (int)bannerW - 8, (int)bannerH - 8, { 90, 20, 20, 255 });
        DrawRectangleLines((int)bannerX + 2, (int)bannerY + 2, (int)bannerW - 4, (int)bannerH - 4, { 245, 50, 50, 255 });
        const char* redText = "RED LIGHT";
        int rw = MeasurePixelText(redText, 18);
        DrawPixelText(redText, (int)(W / 2 - rw / 2), (int)(bannerY + 17), 18, { 255, 60, 60, 255 });
    }

    // Top-right progress bar
    float progress = (playerX - startX) / (finishX - startX);
    if (progress < 0.0f) progress = 0.0f;
    if (progress > 1.0f) progress = 1.0f;

    DrawRectangle(W - 220, 30, 200, 20, { 55, 55, 55, 255 });
    DrawRectangleLines(W - 220, 30, 200, 20, WHITE);
    DrawRectangle(W - 216, 34, (int)(192.0f * progress), 12, { 80, 210, 100, 255 });
}

void RedLightGreenLight::DrawStatusBar() {
    DrawRectangle(0, H - 40, W, 40, BLACK);

    if (greenLight) {
        DrawCircle(24, H - 20, 6, { 50, 240, 50, 255 });
        DrawPixelText("STATUS: MOVING (GREEN LIGHT)", 42, H - 27, 12, { 50, 240, 50, 255 });
    } else {
        DrawCircle(24, H - 20, 6, { 240, 50, 50, 255 });
        DrawPixelText("STATUS: FROZEN (RED LIGHT)", 42, H - 27, 12, { 240, 50, 50, 255 });
    }

    const char* escHint = "ESC TO RETURN TO MENU";
    int ehW = MeasurePixelText(escHint, 10);
    DrawPixelText(escHint, W - ehW - 20, H - 26, 10, { 140, 140, 140, 255 });
}

void RedLightGreenLight::Draw() {
    DrawBackground();
    DrawPlayer();
    DrawDoll();
    DrawHUD();
    DrawStatusBar();

    // Flash red screen when caught moving on Red Light
    if (caughtMoving) {
        if ((int)(flashAlpha * 12.0f) % 2 == 0) {
            DrawRectangle(0, 0, W, H, { 240, 40, 40, 95 });
        }
    }
}

bool RedLightGreenLight::IsComplete() {
    return complete;
}

bool RedLightGreenLight::IsFailed() {
    return failed;
}

void RedLightGreenLight::Unload() {
    if (texPlayer.id > 0) UnloadTexture(texPlayer);
    if (texDollFront.id > 0) UnloadTexture(texDollFront);
    if (texDollBack.id > 0) UnloadTexture(texDollBack);
    if (texTree.id > 0) UnloadTexture(texTree);
    if (texHouse.id > 0) UnloadTexture(texHouse);
}