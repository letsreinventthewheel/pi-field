#include <raylib.h>

#include "background.h"
#include "spike_field.h"

int main(void) {
    InitWindow(1280, 720, "Pi Field");
    SetTargetFPS(60);

    Background* background = LoadBackground();
    SpikeField* field = LoadSpikeField();

    bool paused = false;
    float time = 0.0f;
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) {
            paused = !paused;
        }

        if (!paused) {
            time += GetFrameTime();
        }

        UpdateBackground(background, time, GetRenderWidth(), GetRenderHeight());
        UpdateSpikeField(field, time);

        BeginDrawing();
            ClearBackground(WHITE);
            DrawBackground(background);
            DrawSpikeField(field);
        EndDrawing();
    }

    UnloadSpikeField(field);
    UnloadBackground(background);
    CloseWindow();

    return 0;
}
