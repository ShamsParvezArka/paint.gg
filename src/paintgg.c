#include "paintgg.h"

Toolbar init_toolbar(void) {
    Toolbar toolbar = {
       .rec = (Rectangle) { 16, 16, TOOLBAR_WIDTH, TOOLBAR_HEIGHT },
       .color = BLACK,
       .count = 6,
       .buttons = malloc(sizeof(Button) * toolbar.count)
    };

    Button brush = {
        .tex = NULL,
        .rec = (Rectangle) { 25, 26, BUTTON_WIDTH, BUTTON_HEIGHT },
        .selected = false,
        .id = BRUSH
    };
    Button eraser = {
        .tex = NULL,
        .rec = (Rectangle) {
            26,
            brush.rec.y + BUTTON_HEIGHT + TOOL_BAR_OFFSET,
            BUTTON_WIDTH,
            BUTTON_HEIGHT
        },
        .selected = false,
        .id = ERASER
    };

    Button red = {
        .tex = NULL,
        .rec = (Rectangle) { 26, 162, BUTTON_WIDTH / 2, BUTTON_HEIGHT / 2 },
        .selected = false,
        .id = COLOR_RED
    };
    Button green = {
        .tex = NULL,
        .rec = (Rectangle) { red.rec.x + (BUTTON_WIDTH / 2), 162, BUTTON_WIDTH / 2, BUTTON_HEIGHT / 2 },
        .selected = false,
        .id = COLOR_GREEN
    };
    Button blue = {
        .tex = NULL,
        .rec = (Rectangle) { 26, 162 + BUTTON_HEIGHT / 2, BUTTON_WIDTH / 2, BUTTON_HEIGHT / 2 },
        .selected = false,
        .id = COLOR_BLUE
    };
    Button black = {
        .tex = NULL,
        .rec = (Rectangle) { blue.rec.x + (BUTTON_WIDTH / 2), 162 + BUTTON_HEIGHT / 2, BUTTON_WIDTH / 2, BUTTON_HEIGHT / 2 },
        .selected = false,
        .id = COLOR_BLACK
    };

    toolbar.buttons[BRUSH] = brush;
    toolbar.buttons[ERASER] = eraser;
    toolbar.buttons[COLOR_RED] = red;
    toolbar.buttons[COLOR_GREEN] = green;
    toolbar.buttons[COLOR_BLUE] = blue;
    toolbar.buttons[COLOR_BLACK] = black;

    return toolbar;
};

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

#define SWITCH_TOOL(tb, tbuffer, current_tool) do {   \
    if (current_tool != tbuffer[0]) {                 \
        swap(&tbuffer[0], &tbuffer[1]);               \
        tb->buttons[tbuffer[0]].selected = true;      \
        tb->buttons[tbuffer[1]].selected = false;     \
    }                                                 \
} while (0)                                           \

void update_toolbar(Toolbar *tb, int *tool_selection_buffer, Color *brush_color) {
    for (size_t i = 0; i < tb->count; i++) {
        Button *btn = &tb->buttons[i];

        if (CheckCollisionPointRec(GetMousePosition(), btn->rec)) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                switch (btn->id) {
                case BRUSH:
                    SWITCH_TOOL(tb, tool_selection_buffer, BRUSH);
                #if DEBUG 
                    printf("Paint tool selected -> %d\n", btn->selected);
                #endif
                    break;

                case ERASER:
                    SWITCH_TOOL(tb, tool_selection_buffer, ERASER);
                #if DEBUG 
                    printf("Eraser tool selected -> %d\n", btn->selected);
                #endif
                    break;

                case COLOR_RED:
                    *brush_color = RED;
                    break;

                case COLOR_GREEN:
                    *brush_color = GREEN;
                    break;

                case COLOR_BLUE:
                    *brush_color = BLUE;
                    break;

                case COLOR_BLACK:
                    *brush_color = BLACK;
                    break;

                default:
                    break;
                }
            }
        }
    }

    return;
}

void update_canvas(Toolbar *tb, float brush_radius, Color brush_color) {
    if (!CheckCollisionPointRec(GetMousePosition(), tb->rec) &&
        tb->buttons[BRUSH].selected == true &&
        IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        DrawCircle(GetMouseX(), GetMouseY(), brush_radius, brush_color);
    }

    if (!CheckCollisionPointRec(GetMousePosition(), tb->rec) &&
        tb->buttons[ERASER].selected == true &&
        IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        DrawCircle(GetMouseX(), GetMouseY(), brush_radius, OFFWHITE);
    }
}

void update_sprite(Sprite *s) {
    return;
}

void rander_toolbar(Toolbar tb) {
    DrawRectangleRoundedLines(tb.rec, 0.1f, 0, 1, BLACK);
    //DrawRectangleLinesEx(tb.rec, 1.0f, tb.color);

    for (size_t i = 0; i < tb.count; i++) {
        Button btn = tb.buttons[i];

        switch (btn.id) {
        case COLOR_RED:
            DrawRectangleRec(btn.rec, RED);
            break;

        case COLOR_GREEN:
            DrawRectangleRec(btn.rec, GREEN);
            break;

        case COLOR_BLUE:
            DrawRectangleRec(btn.rec, BLUE);
            break;

        case COLOR_BLACK:
            DrawRectangleRec(btn.rec, BLACK);
            break;

        default:
            break;
        }

        DrawRectangleRoundedLines(tb.buttons[i].rec, 0.1f, 10, 1, BLACK);
    }

    return;
}

#if 1
void rander_canvas(Rectangle area, RenderTexture2D canvas) {
    //DrawRectangleLinesEx(area, 1.0f, BLACK);
    DrawTextureRec(canvas.texture,
                   (Rectangle) { 0, 0, canvas.texture.width, -canvas.texture.height },
                   (Vector2) { 0, 0 },
                   WHITE);

    return;
}
#endif

void update_brush_size(float *brush_radius) {
    if (IsKeyDown(KEY_D) && *brush_radius >= BRUSH_SIZE_MIN)
        *brush_radius -= BRUSH_SIZE_CHANGE_RATE;
    if (IsKeyDown(KEY_F) && *brush_radius <= BRUSH_SIZE_MAX)
        *brush_radius += BRUSH_SIZE_CHANGE_RATE;

    return;
}

void rander_sprite(Sprite s) {
    DrawTexturePro(s.tex, s.source, s.destination, s.origin, s.rotation, WHITE);

    return;
}

Sprite init_sprite(const char *path) {
    Sprite s = {
        .tex = LoadTexture(path),
        .dimension = { s.tex.width / 4, s.tex.height - s.tex.height / 2 },
        .source = { 2 * s.dimension.x, 0.0f, s.dimension.x , s.dimension.y },
        .destination = {
            30,
            WINDOW_HEIGHT - 50,
            s.dimension.x * 4.0f,
            s.dimension.y * 4.0f
        },
        .rotation = 0.0f,
        .origin = s.dimension
    };

    return s;
}

Dynamic_Array init_da() {
    Dynamic_Array da = {
        .points = malloc(sizeof(Dynamic_Array) * DA_DEFAULT_CAPACITY),
        .capacity = DA_DEFAULT_CAPACITY,
        .count = 0
    };
    
    memset(da.points, 0, da.capacity);
    
    return da;
}

void da_resolve_capacity(Dynamic_Array *da) {
    size_t new_capacity = (da->capacity * 1.25) + da->capacity;
    da->capacity = new_capacity;
    da->points = realloc(da->points, sizeof(Vector2) * new_capacity);

    return;
}

void da_insert(Dynamic_Array *da, Vector2 point) {
    if (da->count >= da->capacity)
        da_resolve_capacity(da);

    DA_AT(da, da->count) = point;
    da->count++;

    return;
}