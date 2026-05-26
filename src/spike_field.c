#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <string.h>

#include "spike_field.h"

struct SpikeField {
    Mesh mesh;
    Material material;
    Matrix* transforms;
    int instances;
    int time_location;
};

constexpr static int GRID_SIZE = 256; // 256x256 spikes
constexpr static int INSTANCE_COUNT = GRID_SIZE * GRID_SIZE;

constexpr static float GRID_WORLD_SIZE = 2.0f; // 2x2 spike field in world "units"
constexpr static float SPIKE_WIDTH = GRID_WORLD_SIZE / GRID_SIZE;

constexpr static int SPIKE_MESH_TRIANGLE_COUNT = 4;
constexpr static int SPIKE_MESH_VERTEX_COUNT = SPIKE_MESH_TRIANGLE_COUNT * 3;

static const float SPIKE_MESH_VERTICES[SPIKE_MESH_VERTEX_COUNT * 3] = {
    -0.5f, 0.0f, 0.0f,
    0.5f, 0.0f, 0.0f,
    -0.5f, 1.0f, 0.0f,

    -0.5f, 1.0f, 0.0f,
    0.5f, 0.0f, 0.0f,
    0.5f, 1.0f, 0.0f,

    0.0f, 0.0f, -0.5f,
    0.0f, 0.0f, 0.5f,
    0.0f, 1.0f, -0.5f,

    0.0f, 1.0f, -0.5f,
    0.0f, 0.0f, 0.5f,
    0.0f, 1.0f, 0.5f,
};

#define SCREENSHOT

static const Camera CAMERA = (Camera) {
    #ifdef SCREENSHOT
    .position = (Vector3) { .x = 1.5f, .y = 2.6f, .z = 1.6f },
    #else
    .position = (Vector3) { .x = 1.8f, .y = 2.2f, .z = 1.6f },
    #endif
    .target = (Vector3) { .x = 0.0f, .y = 0.0f, .z = 0.0f },
    .up = (Vector3) { .x = 0.0f, .y = 1.0f, .z = 0.0f },
    .fovy = 45.0f,
    .projection = CAMERA_PERSPECTIVE,
};

static Mesh BuildSpikeMesh(void) {
    Mesh mesh = {};

    mesh.vertexCount = SPIKE_MESH_VERTEX_COUNT;
    mesh.triangleCount = SPIKE_MESH_TRIANGLE_COUNT;
    mesh.vertices = MemAlloc(sizeof(SPIKE_MESH_VERTICES));

    memcpy(mesh.vertices, SPIKE_MESH_VERTICES, sizeof(SPIKE_MESH_VERTICES));
    UploadMesh(&mesh, false);

    return mesh;
}

static Material BuildSpikeMaterial(void) {
    Shader shader = LoadShader("assets/shaders/spike_field.vs", "assets/shaders/spike_field.fs");
    shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocationAttrib(shader, "instanceTransform");
    SetShaderValue(shader, GetShaderLocation(shader, "gridSize"), &GRID_SIZE, SHADER_UNIFORM_INT);

    Texture mask = LoadTexture("assets/mask.png");
    SetTextureFilter(mask, TEXTURE_FILTER_BILINEAR);

    Material material = LoadMaterialDefault();
    material.shader = shader;
    SetMaterialTexture(&material, MATERIAL_MAP_DIFFUSE, mask);

    return material;
}

static Matrix* BuildSpikeTransforms(void) {
    Matrix* transforms = MemAlloc(sizeof(Matrix) * INSTANCE_COUNT);

    for (size_t idx = 0; idx < INSTANCE_COUNT; idx++) {
        int row = idx / GRID_SIZE;
        int column = idx % GRID_SIZE;

        float x = -GRID_WORLD_SIZE / 2.0f + row * SPIKE_WIDTH + SPIKE_WIDTH / 2.0f;
        float z = -GRID_WORLD_SIZE / 2.0f + column * SPIKE_WIDTH + SPIKE_WIDTH / 2.0f;

        transforms[idx] = MatrixMultiply(
            MatrixScale(SPIKE_WIDTH, 1.0f, SPIKE_WIDTH),
            MatrixTranslate(x, 0.0f, z)
        );
    }

    return transforms;
}

SpikeField* LoadSpikeField(void) {
    SpikeField* field = MemAlloc(sizeof(SpikeField));

    field->mesh = BuildSpikeMesh();
    field->material = BuildSpikeMaterial();
    field->transforms = BuildSpikeTransforms();
    field->instances = INSTANCE_COUNT;
    field->time_location = GetShaderLocation(field->material.shader, "time");

    return field;
}

void UpdateSpikeField(const SpikeField* field, float time) {
    SetShaderValue(field->material.shader, field->time_location, &time, SHADER_UNIFORM_FLOAT);
}

void DrawSpikeField(const SpikeField* field) {
    rlDisableBackfaceCulling();

    BeginMode3D(CAMERA);
        DrawMeshInstanced(field->mesh, field->material, field->transforms, field->instances);
    EndMode3D();

    rlEnableBackfaceCulling();
}

void UnloadSpikeField(SpikeField* field) {
    UnloadMaterial(field->material);
    UnloadMesh(field->mesh);
    MemFree(field);
}
