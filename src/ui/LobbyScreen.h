#pragma once
#include "raylib.h"
#include "../core/FontManager.h"

class LobbyScreen {
public:
    bool playPressed = false;

    void Update() {
        int W = GetScreenWidth();
        int H = GetScreenHeight();

        // Check if ENTER or SPACE keys are pressed
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            playPressed = true;
        }

        // Mouse click on PLAY button
        // Anchored to bottom right: W - 250, H - 100, width 210, height 70
        Rectangle playBtn = { (float)W - 250, (float)H - 100, 210, 70 };
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
            CheckCollisionPointRec(GetMousePosition(), playBtn)) {
            playPressed = true;
        }
    }

    void Draw() {
        int W = GetScreenWidth();
        int H = GetScreenHeight();

        // 1. Dark purple/maroon background
        ClearBackground({ 30, 10, 25, 255 });

        // 2. BACKGROUND STAIRS (Pastel Pink and Mint-Green Isometric Steps)
        // We will draw several sets of steps to create the Squid Game Escher-like staircase vibe.
        // Staircase A (Pink, ascending to the right on the left side)
        int stepsA = 10;
        int stepW = 24;
        int stepH = 18;
        float startXA = W * 0.1f;
        float startYA = H * 0.7f;
        for (int i = 0; i < stepsA; i++) {
            float sx = startXA + i * stepW;
            float sy = startYA - i * stepH;
            // Step top (pink)
            DrawRectangle((int)sx, (int)sy, stepW, stepH / 2, { 220, 100, 160, 255 });
            // Step front (darker pink)
            DrawRectangle((int)sx, (int)(sy + stepH / 2), stepW, stepH / 2, { 180, 70, 130, 255 });
        }

        // Staircase B (Mint Green, descending/ascending on the right side)
        int stepsB = 10;
        float startXB = W * 0.9f - (stepsB * stepW);
        float startYB = H * 0.7f;
        for (int i = 0; i < stepsB; i++) {
            float sx = startXB + i * stepW;
            float sy = startYB - (stepsB - 1 - i) * stepH;
            // Step top (mint green)
            DrawRectangle((int)sx, (int)sy, stepW, stepH / 2, { 80, 200, 160, 255 });
            // Step front (darker mint green)
            DrawRectangle((int)sx, (int)(sy + stepH / 2), stepW, stepH / 2, { 50, 160, 130, 255 });
        }

        // Staircase C (Upper center pink staircase)
        int stepsC = 6;
        float startXC = W * 0.45f - (stepsC * stepW) / 2;
        float startYC = H * 0.35f;
        for (int i = 0; i < stepsC; i++) {
            float sx = startXC + i * stepW;
            float sy = startYC + i * stepH;
            // Step top (pink)
            DrawRectangle((int)sx, (int)sy, stepW, stepH / 2, { 220, 100, 160, 255 });
            // Step front (darker pink)
            DrawRectangle((int)sx, (int)(sy + stepH / 2), stepW, stepH / 2, { 180, 70, 130, 255 });
        }

        // 3. FLOATING NEON SYMBOLS SCATTERED AROUND CENTER
        // Glow effect by drawing slightly larger and semi-transparent
        Color cyanGlow = { 0, 255, 240, 60 };
        Color cyanSolid = { 0, 255, 240, 255 };
        Color pinkGlow = { 255, 0, 160, 60 };
        Color pinkSolid = { 255, 0, 160, 255 };

        // Triangle (Cyan) - left of center
        Vector2 triCenter = { W * 0.38f, H * 0.45f };
        for (int t = 2; t >= 0; t--) {
            Color col = (t > 0) ? cyanGlow : cyanSolid;
            float offset = (float)t * 2.0f;
            DrawTriangleLines(
                { triCenter.x, triCenter.y - 20 - offset },
                { triCenter.x - 18 - offset, triCenter.y + 12 + offset },
                { triCenter.x + 18 + offset, triCenter.y + 12 + offset },
                col
            );
        }

        // Circle (Pink) - right of center
        Vector2 circCenter = { W * 0.62f, H * 0.45f };
        for (int t = 2; t >= 0; t--) {
            Color col = (t > 0) ? pinkGlow : pinkSolid;
            DrawCircleLines((int)circCenter.x, (int)circCenter.y, 16 + t * 2, col);
        }

        // Square (Pink) - below left
        Vector2 sqCenter = { W * 0.42f, H * 0.62f };
        for (int t = 2; t >= 0; t--) {
            Color col = (t > 0) ? pinkGlow : pinkSolid;
            float size = 28.0f + t * 4.0f;
            DrawRectangleLines((int)(sqCenter.x - size/2), (int)(sqCenter.y - size/2), (int)size, (int)size, col);
        }

        // Triangle (Cyan) - below right
        Vector2 triCenter2 = { W * 0.58f, H * 0.62f };
        for (int t = 2; t >= 0; t--) {
            Color col = (t > 0) ? cyanGlow : cyanSolid;
            float offset = (float)t * 2.0f;
            DrawTriangleLines(
                { triCenter2.x, triCenter2.y - 18 - offset },
                { triCenter2.x - 16 - offset, triCenter2.y + 10 + offset },
                { triCenter2.x + 16 + offset, triCenter2.y + 10 + offset },
                col
            );
        }

        // 4. CENTER PLATFORM (Purple 2D Pedestal Block) & PLAYER CHARACTER
        int pedW = 120;
        int pedH = 50;
        int pedX = W / 2 - pedW / 2;
        int pedY = H / 2 + 50;

        // Draw flat 2D pedestal block
        DrawRectangle(pedX, pedY, pedW, pedH, { 120, 60, 180, 255 });
        DrawRectangleLines(pedX, pedY, pedW, pedH, { 180, 100, 240, 255 }); // light outline
        DrawRectangle(pedX + 5, pedY + 5, pedW - 10, 4, { 150, 80, 210, 255 }); // inner highlight stripe

        // Player standing on the pedestal
        int px = W / 2;
        int py = pedY; // feet baseline

        // Shoes
        DrawRectangle(px - 18, py - 4, 14, 4, { 240, 240, 240, 255 });
        DrawRectangle(px + 4, py - 4, 14, 4, { 240, 240, 240, 255 });
        
        // Pants/legs (Teal green)
        DrawRectangle(px - 16, py - 32, 11, 28, { 30, 130, 110, 255 });
        DrawRectangle(px + 5, py - 32, 11, 28, { 30, 130, 110, 255 });

        // Body jacket (Teal green)
        DrawRectangle(px - 22, py - 66, 44, 34, { 30, 130, 110, 255 });
        // White sleeve stripes
        DrawRectangle(px - 22, py - 60, 4, 25, WHITE);
        DrawRectangle(px + 18, py - 60, 4, 25, WHITE);

        // Chest player number "456"
        DrawRectangle(px - 14, py - 52, 28, 10, { 20, 90, 80, 255 }); // dark backing for readability
        DrawPixelText("456", px - 11, py - 51, 8, WHITE);

        // Head and Neck (Skin tone)
        DrawRectangle(px - 6, py - 73, 12, 7, { 245, 190, 150, 255 });
        DrawRectangle(px - 16, py - 95, 32, 22, { 245, 190, 150, 255 });

        // Hair (Black retro crop)
        DrawRectangle(px - 18, py - 102, 36, 9, { 25, 20, 20, 255 });
        DrawRectangle(px - 18, py - 93, 4, 10, { 25, 20, 20, 255 });
        DrawRectangle(px + 14, py - 93, 4, 10, { 25, 20, 20, 255 });

        // Eyes
        DrawRectangle(px - 9, py - 86, 3, 3, BLACK);
        DrawRectangle(px + 6, py - 86, 3, 3, BLACK);
        
        // Mouth
        DrawRectangle(px - 4, py - 79, 8, 2, { 200, 100, 100, 255 });

        // 5. GUARDS STANDING ON A PINK RAMP ON THE LEFT
        // Pink platform/ramp triangle on bottom-left
        DrawTriangle(
            { 0, (float)H * 0.72f },
            { 0, (float)H },
            { (float)W * 0.32f, (float)H },
            { 210, 90, 160, 255 }
        );
        // Outer accent line for the ramp edge
        DrawLineEx({ 0, (float)H * 0.72f }, { (float)W * 0.32f, (float)H }, 4, { 240, 120, 190, 255 });

        // Function/lambda logic helper to draw a Guard
        auto DrawGuard = [&](float gx, float gy) {
            // Legs (Pink)
            DrawRectangle((int)gx - 10, (int)gy - 24, 6, 24, { 220, 40, 105, 255 });
            DrawRectangle((int)gx + 4, (int)gy - 24, 6, 24, { 220, 40, 105, 255 });
            DrawRectangle((int)gx - 11, (int)gy - 3, 8, 3, { 20, 20, 20, 255 });
            DrawRectangle((int)gx + 3, (int)gy - 3, 8, 3, { 20, 20, 20, 255 });

            // Torso (Pink)
            DrawRectangle((int)gx - 14, (int)gy - 52, 28, 28, { 220, 40, 105, 255 });

            // Hood & Mask (Black)
            DrawRectangle((int)gx - 10, (int)gy - 72, 20, 20, { 25, 25, 25, 255 });
            // Draw a subtle pink hood outline
            DrawRectangleLines((int)gx - 11, (int)gy - 73, 22, 22, { 220, 40, 105, 255 });

            // Gun (Black submachine gun held diagonally)
            DrawLineEx({ gx - 16, gy - 36 }, { gx + 16, gy - 28 }, 4, { 30, 30, 30, 255 });
            DrawRectangle((int)gx - 6, (int)gy - 34, 4, 8, { 30, 30, 30, 255 });
        };

        // Guard 1 (Circle Mask) - stands on the pink ramp
        float gx1 = W * 0.08f;
        float gy1 = H * 0.72f + (0.08f / 0.32f) * (H * 0.28f) - 4; // adjusted to stand on ramp line
        DrawGuard(gx1, gy1);
        // Draw White Circle on Mask
        DrawCircleLines((int)gx1, (int)gy1 - 62, 5, WHITE);

        // Guard 2 (Triangle Mask) - stands on the pink ramp
        float gx2 = W * 0.18f;
        float gy2 = H * 0.72f + (0.18f / 0.32f) * (H * 0.28f) - 4; // adjusted to stand on ramp line
        DrawGuard(gx2, gy2);
        // Draw White Triangle on Mask
        DrawTriangleLines(
            { gx2, gy2 - 67 },
            { gx2 - 5, gy2 - 58 },
            { gx2 + 5, gy2 - 58 },
            WHITE
        );

        // 6. TOP BAR: LION BADGE & STATS
        // Lion Badge (Top-Left)
        float badgeX = 50;
        float badgeY = 40;
        // Outer Circle (Gold)
        DrawCircle((int)badgeX, (int)badgeY, 24, { 230, 185, 30, 255 });
        DrawCircleLines((int)badgeX, (int)badgeY, 24, WHITE);
        // Lion face pixel blocks
        DrawRectangle((int)badgeX - 12, (int)badgeY - 12, 24, 18, { 210, 110, 15, 255 }); // Mane
        DrawRectangle((int)badgeX - 8, (int)badgeY - 8, 16, 16, { 245, 195, 40, 255 });  // Face
        DrawRectangle((int)badgeX - 4, (int)badgeY + 2, 8, 6, { 230, 160, 30, 255 });   // Snout
        DrawRectangle((int)badgeX - 2, (int)badgeY, 4, 3, BLACK);                       // Nose/eyes bridge
        DrawRectangle((int)badgeX - 6, (int)badgeY - 4, 3, 3, BLACK);                    // Eye Left
        DrawRectangle((int)badgeX + 3, (int)badgeY - 4, 3, 3, BLACK);                    // Eye Right
        DrawRectangle((int)badgeX - 1, (int)badgeY + 4, 2, 2, BLACK);                    // Mouth

        // Stats next to Lion
        DrawPixelText("25", 86, 20, 20, { 240, 200, 30, 255 });
        DrawPixelText("WILLIAM", 86, 44, 16, WHITE);

        // 7. TOP-RIGHT BUTTONS ROW
        // Three retro square buttons
        int btnSize = 36;
        int btnY = 22;
        int startBtnX = W - 150;

        // Button 1: Document Icon
        int bx1 = startBtnX;
        DrawRectangle(bx1, btnY, btnSize, btnSize, { 100, 115, 135, 255 });
        DrawRectangleLines(bx1, btnY, btnSize, btnSize, { 45, 55, 65, 255 });
        // Sheet of paper lines
        DrawRectangle(bx1 + 8, btnY + 8, 20, 20, WHITE);
        DrawRectangle(bx1 + 12, btnY + 12, 12, 2, { 100, 115, 135, 255 });
        DrawRectangle(bx1 + 12, btnY + 17, 12, 2, { 100, 115, 135, 255 });
        DrawRectangle(bx1 + 12, btnY + 22, 8, 2, { 100, 115, 135, 255 });

        // Button 2: Hamburger Menu Icon
        int bx2 = startBtnX + 46;
        DrawRectangle(bx2, btnY, btnSize, btnSize, { 100, 115, 135, 255 });
        DrawRectangleLines(bx2, btnY, btnSize, btnSize, { 45, 55, 65, 255 });
        // Three bars
        DrawRectangle(bx2 + 8, btnY + 10, 20, 3, WHITE);
        DrawRectangle(bx2 + 8, btnY + 17, 20, 3, WHITE);
        DrawRectangle(bx2 + 8, btnY + 24, 20, 3, WHITE);

        // Button 3: Blue Smile Button with Lock
        int bx3 = startBtnX + 92;
        DrawRectangle(bx3, btnY, btnSize, btnSize, { 40, 145, 230, 255 });
        DrawRectangleLines(bx3, btnY, btnSize, btnSize, { 20, 80, 150, 255 });
        // Smile face
        DrawRectangle(bx3 + 10, btnY + 10, 3, 3, BLACK);
        DrawRectangle(bx3 + 23, btnY + 10, 3, 3, BLACK);
        DrawRectangle(bx3 + 10, btnY + 20, 3, 3, BLACK);
        DrawRectangle(bx3 + 23, btnY + 20, 3, 3, BLACK);
        DrawRectangle(bx3 + 13, btnY + 23, 10, 3, BLACK);
        // Tiny gold lock sign overlapping at bottom-right
        DrawRectangle(bx3 + 24, btnY + 24, 12, 10, { 220, 175, 20, 255 }); // Lock body
        DrawRectangleLines(bx3 + 26, btnY + 18, 8, 8, { 220, 175, 20, 255 }); // Shackle

        // Tiny white number "2" badge above button 3
        DrawRectangle(bx3 + 26, btnY - 8, 14, 14, RED);
        DrawPixelText("2", bx3 + 30, btnY - 5, 8, WHITE);

        // 8. BOTTOM-LEFT: STORE & CURRENCY INFO
        int storeX = 40;
        int storeY = H - 90;
        DrawPixelText("STORE", storeX, storeY, 20, WHITE);

        // Gift icon
        int giftX = storeX;
        int giftY = storeY + 26;
        DrawRectangle(giftX, giftY, 28, 28, { 140, 80, 185, 255 }); // purple box
        DrawRectangle(giftX - 2, giftY - 4, 32, 6, { 170, 100, 215, 255 }); // lid
        DrawRectangle(giftX + 12, giftY - 4, 4, 32, { 240, 200, 30, 255 }); // vertical ribbon
        DrawRectangle(giftX - 2, giftY + 10, 32, 4, { 240, 200, 30, 255 }); // horizontal ribbon
        // Bow loops
        DrawRectangle(giftX + 6, giftY - 8, 6, 4, { 240, 200, 30, 255 });
        DrawRectangle(giftX + 16, giftY - 8, 6, 4, { 240, 200, 30, 255 });

        // Money/Cash stack icon
        int cashX = storeX + 44;
        int cashY = giftY + 2;
        // Three layered cash bills
        for (int m = 2; m >= 0; m--) {
            int offset = m * 4;
            DrawRectangle(cashX + offset, cashY - offset, 36, 20, { 55, 140, 80, 255 });
            DrawRectangleLines(cashX + offset, cashY - offset, 36, 20, { 35, 95, 55, 255 });
            DrawRectangle(cashX + offset + 13, cashY - offset + 4, 10, 12, { 90, 200, 125, 255 }); // band
        }

        // Currency value
        DrawPixelText("5000", cashX + 50, cashY + 2, 20, WHITE);

        // 9. BOTTOM-RIGHT: PLAY BUTTON (Sharp edges, double pixel outline)
        int playX = W - 250;
        int playY = H - 100;
        int playW = 210;
        int playH = 70;

        bool playHover = CheckCollisionPointRec(GetMousePosition(), { (float)playX, (float)playY, (float)playW, (float)playH });

        // Double border / Retro style button
        // Outer dark frame
        DrawRectangle(playX, playY, playW, playH, { 45, 60, 75, 255 });
        // Inner highlighted body
        Color btnCol = playHover ? Color{ 110, 140, 160, 255 } : Color{ 75, 100, 115, 255 };
        DrawRectangle(playX + 6, playY + 6, playW - 12, playH - 12, btnCol);
        
        // Light highlighting highlight border (top/left)
        DrawRectangle(playX + 6, playY + 6, playW - 12, 3, { 140, 175, 200, 255 });
        DrawRectangle(playX + 6, playY + 6, 3, playH - 12, { 140, 175, 200, 255 });
        
        // Dark shadow border (bottom/right)
        DrawRectangle(playX + 6, playY + playH - 9, playW - 12, 3, { 30, 40, 50, 255 });
        DrawRectangle(playX + playW - 9, playY + 6, 3, playH - 12, { 30, 40, 50, 255 });

        // Draw "PLAY" text inside
        const char* playTxt = "PLAY";
        int playTextSize = 28;
        int pw = MeasurePixelText(playTxt, playTextSize);
        DrawPixelText(playTxt, playX + playW / 2 - pw / 2, playY + playH / 2 - playTextSize / 2, playTextSize, WHITE);

        // 10. HINT TEXT IN CENTER BOTTOM
        const char* hint = "PRESS ENTER OR CLICK PLAY";
        int hw = MeasurePixelText(hint, 10);
        DrawPixelText(hint, W / 2 - hw / 2, H - 40, 10, { 110, 100, 120, 255 });
    }
};