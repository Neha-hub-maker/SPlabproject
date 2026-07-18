#include "JumpRope.h"
#include "raylib.h"

void JumpRope::Init() {
    currentRopeHeight = RopeHeight::LOW;
    playerAction = PlayerAction::NONE;

    ropeInterval = 1.2f;
    ropeIntervalMin = 0.6f;
    ropeTimer = 0.0f;
    beatWindow = 0.18f;
    beatJudged = false;

    jumpTimer = 0.0f;
    jumpDuration = 0.35f;
    duckTimer = 0.0f;
    duckDuration = 0.35f;

    successfulPasses = 0;
    targetPasses = 15;

    levelComplete = false;
    levelFailed = false;

    SpawnNextBeat();
}

void JumpRope::SpawnNextBeat() {
    currentRopeHeight = (GetRandomValue(0, 1) == 0) ? RopeHeight::LOW : RopeHeight::HIGH;
    ropeTimer = 0.0f;
    beatJudged = false;

    // Difficulty ramps up as the player racks up successful passes:
    // interval shrinks linearly from 1.2s down to a 0.6s floor.
    float progress = (float)successfulPasses / (float)targetPasses;
    ropeInterval = 1.2f - progress * (1.2f - ropeIntervalMin);
    if (ropeInterval < ropeIntervalMin) ropeInterval = ropeIntervalMin;
}

void JumpRope::UpdatePlayerState(float deltaTime) {
    if (jumpTimer > 0.0f) {
        jumpTimer -= deltaTime;
        if (jumpTimer <= 0.0f) {
            jumpTimer = 0.0f;
            playerAction = PlayerAction::NONE;
        }
    }
    if (duckTimer > 0.0f) {
        duckTimer -= deltaTime;
        if (duckTimer <= 0.0f) {
            duckTimer = 0.0f;
            playerAction = PlayerAction::NONE;
        }
    }

    if (IsKeyPressed(KEY_SPACE) && duckTimer <= 0.0f) {
        playerAction = PlayerAction::JUMPING;
        jumpTimer = jumpDuration;
        JudgeBeat();
    }
    if (IsKeyPressed(KEY_S) && jumpTimer <= 0.0f) {
        playerAction = PlayerAction::DUCKING;
        duckTimer = duckDuration;
        JudgeBeat();
    }
}

void JumpRope::JudgeBeat() {
    if (beatJudged) return;

    bool inWindow = (ropeTimer >= ropeInterval - beatWindow) &&
                     (ropeTimer <= ropeInterval + beatWindow);
    if (!inWindow) return; // pressed way too early/late; wait for the real window

    bool correct =
        (currentRopeHeight == RopeHeight::LOW  && playerAction == PlayerAction::JUMPING) ||
        (currentRopeHeight == RopeHeight::HIGH && playerAction == PlayerAction::DUCKING);

    beatJudged = true;

    if (correct) {
        successfulPasses++;
    } else {
        levelFailed = true;
    }
}

void JumpRope::Update(float deltaTime) {
    if (levelComplete || levelFailed) return;

    ropeTimer += deltaTime;
    UpdatePlayerState(deltaTime);

    // Window closed with no correct action registered -> instant fail.
    if (!beatJudged && ropeTimer > ropeInterval + beatWindow) {
        levelFailed = true;
        beatJudged = true;
    }

    if (!levelFailed && beatJudged && ropeTimer > ropeInterval + beatWindow) {
        SpawnNextBeat();
    }

    if (!levelFailed && successfulPasses >= targetPasses) {
        levelComplete = true;
    }
}

void JumpRope::Draw() {
    ClearBackground(RAYWHITE);

    DrawLine(0, 500, GetScreenWidth(), 500, DARKGRAY);

    int ropeY = (currentRopeHeight == RopeHeight::LOW) ? 480 : 380;
    DrawRectangle(0, ropeY, GetScreenWidth(), 8, MAROON);

    int playerY = 460;
    if (playerAction == PlayerAction::JUMPING) playerY -= 60;
    if (playerAction == PlayerAction::DUCKING) playerY += 20;
    DrawRectangle(GetScreenWidth() / 2 - 20, playerY, 40, 60, DARKBLUE);

    DrawText(TextFormat("Score: %d / %d", successfulPasses, targetPasses), 20, 20, 24, BLACK);

    if (levelFailed) {
        DrawText("MISSED!", GetScreenWidth() / 2 - 70, 250, 40, RED);
    }
}

bool JumpRope::IsComplete() const { return levelComplete; }
bool JumpRope::IsFailed() const { return levelFailed; }

void JumpRope::Unload() {
    // No textures/sounds loaded yet (placeholder rectangles for now) —
    // hook here once sprite assets are added.
}
