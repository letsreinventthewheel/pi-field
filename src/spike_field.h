#pragma once

typedef struct SpikeField SpikeField;

SpikeField* LoadSpikeField(void);
void UpdateSpikeField(const SpikeField* field, float time);
void DrawSpikeField(const SpikeField* field);
void UnloadSpikeField(SpikeField* field);
