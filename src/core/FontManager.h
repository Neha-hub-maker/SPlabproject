#pragma once
#include "raylib.h"

// Global pixel font - loaded once, used everywhere
inline Font& GetPixelFont() {
    static Font font = {0};
    static bool loaded = false;
    if (!loaded) {
        const char* path = "assets/fonts/PressStart2P-Regular.ttf";
        if (!FileExists(path)) {
            path = "../assets/fonts/PressStart2P-Regular.ttf";
        }
        font = LoadFont(path);
        SetTextureFilter(font.texture, TEXTURE_FILTER_POINT);
        loaded = true;
    }
    return font;
}

// Drop-in replacement for DrawText using pixel font
inline void DrawPixelText(const char* text, int x, int y, int size, Color color) {
    DrawTextEx(GetPixelFont(), text, {(float)x, (float)y},
               (float)size, 2, color);
}

// Measure text width with pixel font
inline int MeasurePixelText(const char* text, int size) {
    return (int)MeasureTextEx(GetPixelFont(), text, (float)size, 2).x;
}