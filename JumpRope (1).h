#pragma once
#include "../core/ILevel.h"

// Level 3: Jump Rope
// Rope height alternates LOW / HIGH on a shrinking timer. Player must
// JUMP (Space) when the rope is LOW and DUCK (S) when it is HIGH,
// timed to a short "beat window" around when the rope arrives.
// A single missed or mistimed beat ends the level immediately.
class JumpRope : public ILevel {
public:
    void Init() override;
    void Update(float deltaTime) override;
    void Draw() override;
    bool IsComplete() const override;
    bool IsFailed() const override;
    void Unload() override;

private:
    enum class RopeHeight { LOW, HIGH };
    enum class PlayerAction { NONE, JUMPING, DUCKING };

    RopeHeight currentRopeHeight;
    PlayerAction playerAction;

    // Rope timing
    float ropeTimer;        // time elapsed since the current beat started
    float ropeInterval;     // seconds until the rope "arrives" this beat
    float ropeIntervalMin;  // hardest difficulty floor
    float beatWindow;       // +/- seconds of timing tolerance
    bool  beatJudged;       // has this beat already been scored?

    // Player action durations (how long a jump/duck pose is held)
    float jumpTimer;
    float jumpDuration;
    float duckTimer;
    float duckDuration;

    int successfulPasses;
    int targetPasses;

    bool levelComplete;
    bool levelFailed;

    void SpawnNextBeat();
    void JudgeBeat();
    void UpdatePlayerState(float deltaTime);
};
