#include "raylib.h"
#include "raymath.h"

typedef struct EditorCamera2D {
    Camera2D camera;
    float zoomSpeed;
    float minZoom;
    float maxZoom;
} EditorCamera2D;

void UpdateEditorCamera(EditorCamera2D* camera)
{
    if (!camera) return;

    Vector2 dpi = GetWindowScaleDPI();

    camera->camera.offset = (Vector2){(float)GetRenderWidth() / 2.0F, (float)GetRenderHeight() / 2.0F};

    if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
    {
        // target -= rotate(mouse delta, -rotation) / zoom
        camera->camera.target = Vector2Subtract(camera->camera.target, Vector2Scale(Vector2Rotate(GetMouseDelta(), -camera->camera.rotation * DEG2RAD), 1.0F / camera->camera.zoom));
    }

    if (GetMouseWheelMove() != 0)
    {
        float nextZoom = exp2f(log2f(camera->camera.zoom) + camera->zoomSpeed * GetMouseWheelMove());
        // target += rotate(mouse position * dpi, -rotation) * (1 / zoom - 1 / next zoom)
        camera->camera.target = Vector2Add(camera->camera.target, Vector2Scale(Vector2Rotate(Vector2Subtract(Vector2Multiply(GetMousePosition(), dpi), camera->camera.offset), -camera->camera.rotation * DEG2RAD), 1.0F / camera->camera.zoom - 1.0F / nextZoom));
        camera->camera.zoom = nextZoom;
    }
}

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE);
    InitWindow(1200, 600, "Editor camera 2D");
    SetTargetFPS(144);

    EditorCamera2D camera = {0};
    camera.camera.zoom = 0.1F;
    camera.zoomSpeed = 0.1F;
    camera.minZoom = 0.015625F;
    camera.maxZoom = 64.0F;

    Texture texture[] = {
        LoadTexture("tile_00.png"),
        LoadTexture("tile_01.png"),
        LoadTexture("tile_02.png"),
        LoadTexture("tile_03.png")
    };

    int size = sizeof(texture) / sizeof(texture[0]);

    int textSize = (int)(20.0F / fminf(GetWindowScaleDPI().x, GetWindowScaleDPI().y));

    while (!WindowShouldClose())
    {
        UpdateEditorCamera(&camera);

        BeginDrawing();
        ClearBackground(WHITE);

        BeginMode2D(camera.camera);

        for (int i = 0; i < size; i++)
        {
            DrawTexture(texture[i], 8192 * i, 0, WHITE);
        }

        EndMode2D();

        DrawText("Panorama di Manerba del Garda con Pieve Rocca Montinelle Solarolo", 20, GetScreenHeight() - 60, textSize, BLACK);
        DrawText("Photo by Wolfgang Moroder, CC BY-SA 4.0", 20, GetScreenHeight() - 40, textSize, BLACK);

        EndDrawing();
    }

    for (int i = 0; i < size; i++)
    {
        UnloadTexture(texture[i]);
    }

    CloseWindow();
}
