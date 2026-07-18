#include "Dalgona.h"

#include <algorithm>
#include <cmath>

namespace {
constexpr float Pi = 3.14159265358979323846f;
constexpr Color CookieGold{224, 168, 55, 255};
constexpr Color CookieEdge{112, 66, 25, 255};
constexpr Color Background{247, 226, 190, 255};
}

void Dalgona::Init() {
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    const float centerY = screenHeight * 0.52f;
    gameShape = {{screenWidth * 0.5f, centerY}, std::min(screenWidth, screenHeight) * 0.20f};
    selectCircle = {{screenWidth * 0.22f, centerY}, std::min(screenWidth, screenHeight) * 0.09f};
    selectSquare = {{screenWidth * 0.50f, centerY}, std::min(screenWidth, screenHeight) * 0.09f};
    selectTriangle = {{screenWidth * 0.78f, centerY}, std::min(screenWidth, screenHeight) * 0.10f};

    screen = Screen::SelectShape;
    chosenShape = Shape::Circle;
    result = Result::None;
    cutting = false;
    complete = false;
    failed = false;
    resultTimer = 0.0f;
    outlineCount = 0;
    visitedCount = 0;
    visited.fill(false);
}

float Dalgona::PointToSegmentDistance(Vector2 point, Vector2 start, Vector2 end) {
    Vector2 segment{end.x - start.x, end.y - start.y};
    Vector2 offset{point.x - start.x, point.y - start.y};
    const float lengthSquared = segment.x * segment.x + segment.y * segment.y;
    float t = lengthSquared > 0.0f
        ? (offset.x * segment.x + offset.y * segment.y) / lengthSquared
        : 0.0f;
    t = std::clamp(t, 0.0f, 1.0f);
    const Vector2 closest{start.x + segment.x * t, start.y + segment.y * t};
    return std::hypot(point.x - closest.x, point.y - closest.y);
}

void Dalgona::GetTriangleCorners(ShapeData data, Vector2& first, Vector2& second, Vector2& third) {
    first = {data.center.x, data.center.y - data.size};
    second = {data.center.x - data.size * 0.8660254f, data.center.y + data.size * 0.5f};
    third = {data.center.x + data.size * 0.8660254f, data.center.y + data.size * 0.5f};
}

float Dalgona::DistanceToShapeOutline(Shape shape, ShapeData data, Vector2 point) {
    if (shape == Shape::Circle) {
        return std::fabs(std::hypot(point.x - data.center.x, point.y - data.center.y) - data.size);
    }

    if (shape == Shape::Square) {
        const float half = data.size;
        const Vector2 corners[4] = {
            {data.center.x - half, data.center.y - half},
            {data.center.x + half, data.center.y - half},
            {data.center.x + half, data.center.y + half},
            {data.center.x - half, data.center.y + half}
        };
        float distance = PointToSegmentDistance(point, corners[0], corners[1]);
        for (int index = 1; index < 4; ++index) {
            distance = std::min(distance,
                PointToSegmentDistance(point, corners[index], corners[(index + 1) % 4]));
        }
        return distance;
    }

    Vector2 first, second, third;
    GetTriangleCorners(data, first, second, third);
    return std::min({
        PointToSegmentDistance(point, first, second),
        PointToSegmentDistance(point, second, third),
        PointToSegmentDistance(point, third, first)
    });
}

int Dalgona::GenerateOutlinePoints() {
    int count = 0;
    if (chosenShape == Shape::Circle) {
        for (int index = 0; index < MaxOutlinePoints; ++index) {
            const float angle = 2.0f * Pi * index / MaxOutlinePoints;
            outlinePoints[count++] = {
                gameShape.center.x + gameShape.size * std::cos(angle),
                gameShape.center.y + gameShape.size * std::sin(angle)
            };
        }
        return count;
    }

    Vector2 corners[4]{};
    int sides = 0;
    if (chosenShape == Shape::Square) {
        const float half = gameShape.size;
        corners[0] = {gameShape.center.x - half, gameShape.center.y - half};
        corners[1] = {gameShape.center.x + half, gameShape.center.y - half};
        corners[2] = {gameShape.center.x + half, gameShape.center.y + half};
        corners[3] = {gameShape.center.x - half, gameShape.center.y + half};
        sides = 4;
    } else {
        GetTriangleCorners(gameShape, corners[0], corners[1], corners[2]);
        sides = 3;
    }

    const int pointsPerSide = MaxOutlinePoints / sides;
    for (int side = 0; side < sides; ++side) {
        const Vector2 start = corners[side];
        const Vector2 end = corners[(side + 1) % sides];
        for (int index = 0; index < pointsPerSide; ++index) {
            const float t = static_cast<float>(index) / pointsPerSide;
            outlinePoints[count++] = {
                start.x + (end.x - start.x) * t,
                start.y + (end.y - start.y) * t
            };
        }
    }
    return count;
}

void Dalgona::BeginCutting(Shape shape, float size) {
    chosenShape = shape;
    gameShape.size = size;
    outlineCount = GenerateOutlinePoints();
    visited.fill(false);
    visitedCount = 0;
    cutting = false;
    result = Result::None;
    screen = Screen::Cutting;
}

void Dalgona::FinishAttempt(Result attemptResult) {
    result = attemptResult;
    cutting = false;
    screen = Screen::Result;
    resultTimer = 0.0f;
}

void Dalgona::Update(float dt) {
    const Vector2 mouse = GetMousePosition();

    if (screen == Screen::SelectShape) {
        if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return;
        if (CheckCollisionPointCircle(mouse, selectCircle.center, selectCircle.size + 25.0f)) {
            BeginCutting(Shape::Circle, std::min(screenWidth, screenHeight) * 0.20f);
        } else if (CheckCollisionPointRec(mouse, {
            selectSquare.center.x - selectSquare.size - 25.0f,
            selectSquare.center.y - selectSquare.size - 25.0f,
            (selectSquare.size + 25.0f) * 2.0f,
            (selectSquare.size + 25.0f) * 2.0f})) {
            BeginCutting(Shape::Square, std::min(screenWidth, screenHeight) * 0.18f);
        } else if (CheckCollisionPointCircle(mouse, selectTriangle.center, selectTriangle.size + 25.0f)) {
            BeginCutting(Shape::Triangle, std::min(screenWidth, screenHeight) * 0.22f);
        }
        return;
    }

    if (screen == Screen::Cutting) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            cutting = true;
            lastMousePosition = mouse;
            visited.fill(false);
            visitedCount = 0;
        }

        if (cutting && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (DistanceToShapeOutline(chosenShape, gameShape, mouse) > tolerance) {
                FinishAttempt(Result::Broken);
                return;
            }
            for (int index = 0; index < outlineCount; ++index) {
                if (!visited[index] &&
                    PointToSegmentDistance(outlinePoints[index], lastMousePosition, mouse) <= visitRadius) {
                    visited[index] = true;
                    ++visitedCount;
                }
            }
            lastMousePosition = mouse;
        }

        if (cutting && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            const float progress = outlineCount > 0
                ? static_cast<float>(visitedCount) / outlineCount
                : 0.0f;
            FinishAttempt(progress >= completionThreshold ? Result::Win : Result::Incomplete);
        }
        return;
    }

    resultTimer += dt;
    if (resultTimer >= 1.25f) {
        complete = result == Result::Win;
        failed = result == Result::Broken || result == Result::Incomplete;
    }
}

void Dalgona::DrawShapeFilled(Shape shape, ShapeData data, Color color) const {
    if (shape == Shape::Circle) {
        DrawCircleV(data.center, data.size, color);
    } else if (shape == Shape::Square) {
        DrawRectangle(static_cast<int>(data.center.x - data.size),
                      static_cast<int>(data.center.y - data.size),
                      static_cast<int>(data.size * 2.0f), static_cast<int>(data.size * 2.0f), color);
    } else {
        Vector2 first, second, third;
        GetTriangleCorners(data, first, second, third);
        DrawTriangle(first, second, third, color);
    }
}

void Dalgona::DrawShapeOutline(Shape shape, ShapeData data, Color color) const {
    if (shape == Shape::Circle) {
        DrawCircleLines(static_cast<int>(data.center.x), static_cast<int>(data.center.y), data.size, color);
    } else if (shape == Shape::Square) {
        DrawRectangleLines(static_cast<int>(data.center.x - data.size),
                           static_cast<int>(data.center.y - data.size),
                           static_cast<int>(data.size * 2.0f), static_cast<int>(data.size * 2.0f), color);
    } else {
        Vector2 first, second, third;
        GetTriangleCorners(data, first, second, third);
        DrawTriangleLines(first, second, third, color);
    }
}

void Dalgona::DrawSelection() const {
    const char* title = "CHOOSE YOUR DALGONA SHAPE";
    DrawPixelText(title, screenWidth / 2 - MeasurePixelText(title, 26) / 2, 55, 26, CookieEdge);

    DrawShapeFilled(Shape::Circle, selectCircle, CookieGold);
    DrawShapeOutline(Shape::Circle, {selectCircle.center, selectCircle.size * 0.52f}, CookieEdge);
    DrawShapeFilled(Shape::Square, selectSquare, CookieGold);
    DrawShapeOutline(Shape::Square, {selectSquare.center, selectSquare.size * 0.52f}, CookieEdge);
    DrawShapeFilled(Shape::Triangle, selectTriangle, CookieGold);
    DrawShapeOutline(Shape::Triangle, {selectTriangle.center, selectTriangle.size * 0.52f}, CookieEdge);

    const char* labels[3] = {"CIRCLE", "SQUARE", "TRIANGLE"};
    const ShapeData shapes[3] = {selectCircle, selectSquare, selectTriangle};
    for (int index = 0; index < 3; ++index) {
        DrawPixelText(labels[index], static_cast<int>(shapes[index].center.x) - MeasurePixelText(labels[index], 15) / 2,
                      static_cast<int>(shapes[index].center.y + shapes[index].size + 35.0f), 15, CookieEdge);
    }
}

void Dalgona::DrawCutting() const {
    const char* instruction = "HOLD LEFT MOUSE AND TRACE THE WHOLE OUTLINE";
    DrawPixelText(instruction, screenWidth / 2 - MeasurePixelText(instruction, 16) / 2, 35, 16, CookieEdge);
    DrawShapeFilled(chosenShape, gameShape, CookieGold);
    DrawShapeOutline(chosenShape, gameShape, CookieEdge);
    for (int index = 0; index < outlineCount; ++index) {
        if (visited[index]) DrawCircleV(outlinePoints[index], 3.0f, LIME);
    }
    const int percentage = outlineCount > 0 ? 100 * visitedCount / outlineCount : 0;
    DrawPixelText(TextFormat("PROGRESS: %d%%", percentage), 30, screenHeight - 55, 16, DARKGREEN);
}

void Dalgona::DrawResult() const {
    DrawShapeFilled(chosenShape, gameShape, result == Result::Win ? CookieGold : Fade(CookieGold, 0.45f));
    DrawShapeOutline(chosenShape, gameShape, CookieEdge);
    const char* message = result == Result::Win ? "PERFECT CUT!" :
                          result == Result::Broken ? "THE COOKIE BROKE!" : "THE CUT IS INCOMPLETE!";
    const Color color = result == Result::Win ? DARKGREEN : RED;
    DrawPixelText(message, screenWidth / 2 - MeasurePixelText(message, 24) / 2, 45, 24, color);
}

void Dalgona::Draw() {
    ClearBackground(Background);
    if (screen == Screen::SelectShape) DrawSelection();
    else if (screen == Screen::Cutting) DrawCutting();
    else DrawResult();
}

bool Dalgona::IsComplete() { return complete; }
bool Dalgona::IsFailed() { return failed; }
void Dalgona::Unload() {}
