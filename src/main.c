#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "raylib.h"
#include "paintgg.h"

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

    Toolbar toolbar = init_toolbar();
    int tool_selection_buffer[2] = { ERASER, BRUSH };

    Color brush_color = BLACK;
    float brush_radius = 6.0f;

    RenderTexture2D canvas = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    Rectangle canvas_area = { 116, 16, 1119, 621 };

    Sprite keys = init_sprite("assets/keys.png");

    while (!WindowShouldClose()) {
        update_toolbar(&toolbar, tool_selection_buffer, &brush_color);
        update_brush_size(&brush_radius);

        BeginTextureMode(canvas);
        {
            update_canvas(&toolbar, brush_radius, brush_color);
        }
        EndTextureMode();
        
        BeginDrawing();
        {
            ClearBackground(OFFWHITE);
            rander_toolbar(toolbar);
            rander_canvas(canvas_area, canvas);
           
            if (!CheckCollisionPointRec(GetMousePosition(), toolbar.rec) &&
                (toolbar.buttons[BRUSH].selected == true ||
                 toolbar.buttons[ERASER].selected == true)) {
                DrawCircleLinesV(GetMousePosition(), brush_radius, BLACK);
            }

            if (IsKeyPressed(KEY_D))
                keys.source.y += keys.tex.height / 2;

            rander_sprite(keys);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}