#pragma once
#ifndef OSUSYNC_ENGINE_H
#define OSUSYNC_ENGINE_H

#include "raylib.h"

class OsuSyncEngine {
public:
    OsuSyncEngine();
    ~OsuSyncEngine();

    void Run();

private:
    int screenWidth;
    int screenHeight;
    const char* windowTitle;
    bool running;

    void Init();
    void Update();
    void Draw();
    void Cleanup();
};
#endif