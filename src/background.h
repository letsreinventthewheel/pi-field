#pragma once

typedef struct Background Background;

Background* LoadBackground(void);
void UpdateBackground(const Background* background, float time, int width, int height);
void DrawBackground(const Background* background);
void UnloadBackground(Background* background);
