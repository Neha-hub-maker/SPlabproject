#pragma once

class ILevel {
public:
    virtual void Init() = 0;
    virtual void Update(float dt) = 0;
    virtual void Draw() = 0;
    virtual bool IsComplete() = 0;
    virtual bool IsFailed() = 0;
    virtual void Unload() = 0;
    virtual ~ILevel() = default;
};