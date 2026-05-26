#include <raylib.h>

#include "background.h"

struct Background {
    Shader shader;
    int time_location;
    int resolution_location;
};

Background* LoadBackground(void) {
    Background* background = MemAlloc(sizeof(Background));

    background->shader = LoadShader(nullptr, "assets/shaders/background.fs");
    background->resolution_location = GetShaderLocation(background->shader, "resolution");
    background->time_location = GetShaderLocation(background->shader, "time");

    return background;
}

void UpdateBackground(const Background* background, float time, int width, int height) {
    float resolution[2] = {width, height};
    SetShaderValue(background->shader, background->resolution_location, resolution, SHADER_UNIFORM_VEC2);
    SetShaderValue(background->shader, background->time_location, &time, SHADER_UNIFORM_FLOAT);
}

void DrawBackground(const Background* background) {
    BeginShaderMode(background->shader);
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
    EndShaderMode();
}

void UnloadBackground(Background* background) {
    UnloadShader(background->shader);
    MemFree(background);
}
