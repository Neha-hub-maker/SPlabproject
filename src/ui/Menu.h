#pragma once
#include "raylib.h"
#include "../core/FontManager.h"

class Menu {
public:
    void Draw() {
        int W = GetScreenWidth();
        int H = GetScreenHeight();

        // Dark pink/maroon background
        ClearBackground({ 20, 10, 10, 255 });

        // Top banner strip
        DrawRectangle(0, 0, W, 80, { 139, 0, 0, 255 });

        // Squid Game symbols in banner - Circle, Triangle, Square
        DrawCircleLines(W/2 - 120, 40, 22, WHITE);
        // Triangle
        DrawTriangleLines(
            {(float)W/2, 18},
            {(float)W/2 - 25, 62},
            {(float)W/2 + 25, 62},
            WHITE
        );
        // Square
        DrawRectangleLines(W/2 + 95, 18, 44, 44, WHITE);

        // Main title
        const char* title = "SQUID GAME";
        int titleSize = 90;
        int titleWidth = MeasurePixelText(title, titleSize);
        DrawPixelText(title, W/2 - titleWidth/2, H/2 - 180, titleSize, { 220, 20, 20, 255 });

        // Subtitle
        const char* sub = "456 Players. One Winner.";
        int subWidth = MeasurePixelText(sub, 24);
        DrawPixelText(sub, W/2 - subWidth/2, H/2 - 70, 24, { 200, 200, 200, 255 });

        // Divider line
        DrawRectangle(W/2 - 200, H/2 - 30, 400, 2, { 139, 0, 0, 255 });

        // PLAY button
        bool playHover = CheckCollisionPointRec(
            GetMousePosition(),
            { (float)W/2 - 160, (float)H/2, 320, 65 }
        );
        DrawRectangleRounded({ (float)W/2 - 160, (float)H/2, 320, 65 },
            0.3f, 8, playHover ? Color{180,0,0,255} : Color{139,0,0,255});
        const char* playText = "PRESS ENTER TO PLAY";
        int playWidth = MeasurePixelText(playText, 22);
        DrawPixelText(playText, W/2 - playWidth/2, H/2 + 20, 22, WHITE);

        // EXIT button
        bool exitHover = CheckCollisionPointRec(
            GetMousePosition(),
            { (float)W/2 - 160, (float)H/2 + 90, 320, 65 }
        );
        DrawRectangleRounded({ (float)W/2 - 160, (float)H/2 + 90, 320, 65 },
            0.3f, 8, exitHover ? Color{60,60,60,255} : Color{30,30,30,255});
        const char* exitText = "ESC TO EXIT";
        int exitWidth = MeasurePixelText(exitText, 22);
        DrawPixelText(exitText, W/2 - exitWidth/2, H/2 + 110, 22, { 180, 180, 180, 255 });

        // Bottom player count
        const char* count = "PLAYER: 456";
        int countWidth = MeasurePixelText(count, 20);
        DrawPixelText(count, W/2 - countWidth/2, H - 60, 20, { 150, 150, 150, 255 });

        // Bottom strip
        DrawRectangle(0, H - 25, W, 25, { 139, 0, 0, 255 });
    }
};