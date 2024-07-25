#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"

#ifndef DEBUG
    #define DEBUG 1
#endif

#define WINDOW_WIDTH 1260
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE "paint.gg"

#define TOOLBAR_WIDTH 72
#define TOOLBAR_HEIGHT 216
#define TOOL_BAR_OFFSET 8

#define BUTTON_WIDTH 54
#define BUTTON_HEIGHT 54

#define BRUSH_SIZE_MIN 4.0f
#define BRUSH_SIZE_MAX 16.0f

#define DA_DEFAULT_CAPACITY 1024
#define DA_AT(da, i) da->points[i]
#define DA_VIEW_X(da, i) da.points[i].x
#define DA_VIEW_Y(da, i) da.points[i].y

#define OFFWHITE CLITERAL(Color){ 253, 246, 227, 255 }

typedef enum {
    BRUSH,
    ERASER,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_BLACK
} Button_ID;

typedef struct {
    Rectangle rec;
    Texture2D tex;
    bool selected;
    Button_ID id;
} Button;

typedef struct {
    Rectangle rec;
    Color color;
    size_t count;
    Button *buttons;
} Toolbar;

typedef struct {
    Vector2 *points;
    size_t count;
    size_t capacity;
} Dynamic_Array;


Toolbar init_toolbar(void);
void swap(int *a, int *b);
void update_toolbar(Toolbar *tb, int *tool_selection_buffer, Color *brush_color);
void update_canvas(Toolbar *tb, float brush_radius, Color brush_color);
void rander_toolbar(Toolbar tb);
void rander_canvas(Rectangle area, RenderTexture2D canvas);
void update_brush_size(float *brush_radius);

Dynamic_Array da_init();
void da_resolve_capacity(Dynamic_Array *da);
void da_insert(Dynamic_Array *da, Vector2 point);

