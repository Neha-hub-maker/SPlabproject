#pragma once

#include "../core/ILevel.h"
#include "../core/FontManager.h"
#include "raylib.h"

#include <array>

class Dalgona : public ILevel {
public:
    void Init() override;
    void Update(float dt) override;
    void Draw() override;
    bool IsComplete() override;
    bool IsFailed() override;
    void Unload() override;

private:
    enum class Screen { SelectShape, Cutting, Result };
    enum class Shape { Circle, Square, Triangle };
    enum class Result { None, Win, Broken, Incomplete };

    struct ShapeData {
        Vector2 center{};
        float size = 0.0f;
    };

    static constexpr int MaxOutlinePoints = 120;

    Screen screen = Screen::SelectShape;
    Shape chosenShape = Shape::Circle;
    Result result = Result::None;

    int screenWidth = 0;
    int screenHeight = 0;
    bool cutting = false;
    bool complete = false;
    bool failed = false;
    float resultTimer = 0.0f;
    Vector2 lastMousePosition{};

    const float tolerance = 15.0f;
    const float visitRadius = 22.0f;
    const float completionThreshold = 1.0f;

    ShapeData gameShape{};
    ShapeData selectCircle{};
    ShapeData selectSquare{};
    ShapeData selectTriangle{};

    std::array<Vector2, MaxOutlinePoints> outlinePoints{};
    std::array<bool, MaxOutlinePoints> visited{};
    int outlineCount = 0;
    int visitedCount = 0;

    static float PointToSegmentDistance(Vector2 point, Vector2 start, Vector2 end);
    static void GetTriangleCorners(ShapeData data, Vector2& first, Vector2& second, Vector2& third);
    static float DistanceToShapeOutline(Shape shape, ShapeData data, Vector2 point);

    int GenerateOutlinePoints();
    void BeginCutting(Shape shape, float size);
    void FinishAttempt(Result attemptResult);
    void DrawShapeFilled(Shape shape, ShapeData data, Color color) const;
    void DrawShapeOutline(Shape shape, ShapeData data, Color color) const;
    void DrawSelection() const;
    void DrawCutting() const;
    void DrawResult() const;
};
