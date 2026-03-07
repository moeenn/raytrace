#include "raylib.h"
#include <math.h>
#include <stddef.h>
#include <stdint.h>

#define WIN_TITLE "Game"
#define WIN_HEIGHT 600
#define WIN_WIDTH 800
#define FPS 60
#define RADIUS 10
#define NUM_LINES 100
#define LINE_MAX_LENGTH 1000

static const Color BG = { 0, 0, 0, 1 };
static const Color PARTICLE_COLOR = { 230, 41, 55, 255 };
static const Color LINE_COLOR = { 180, 32, 44, 255 };

typedef struct {
    Vector2 pos;
} Particle;

typedef struct {
    Vector2 start;
    Vector2 end;
} Line;

typedef struct {
    Vector2 pos;
    int size;
} Square;

void particle_init(Particle* self, int x, int y)
{
    self->pos = (Vector2) { .x = x, .y = y };
}

void particle_render(const Particle* p)
{
    DrawCircle(p->pos.x, p->pos.y, RADIUS, PARTICLE_COLOR);
}

Vector2 calculateLineXAndY(int hypotenuse, float angle_degrees)
{
    float angle_rad = angle_degrees * (3.142 / 180.0);
    float base = hypotenuse * cos(angle_rad);
    float height = hypotenuse * sin(angle_rad);
    return (Vector2) { .x = base, .y = height };
}

void lines_update(Line lines[], size_t numLines, Particle* p)
{
    size_t i;
    Vector2 end;
    Line* l;

    float deg = 90;
    int step = (360 / numLines) + 1; // 1 is line width.

    for (i = 0; i < numLines; i++) {
        l = &lines[i];
        l->start = p->pos;
        end = calculateLineXAndY(LINE_MAX_LENGTH, deg);
        l->end.x = l->start.x + end.x;
        l->end.y = l->start.y + end.y;
        deg += (float)(step % 360);
    }
}

void lines_render(Line lines[], size_t numLines)
{
    size_t i;
    Line* l;

    for (i = 0; i < numLines; i++) {
        l = &lines[i];
        DrawLine(l->start.x, l->start.y, l->end.x, l->end.y, LINE_COLOR);
    }
}

void square_render(Square* self)
{
    DrawRectangleLines(self->pos.x, self->pos.y, self->size, self->size, LINE_COLOR);
}

int main(void)
{
    InitWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE);
    SetTargetFPS(FPS);

    Particle p;
    particle_init(&p, 200, 200);

    Line lines[NUM_LINES];
    Square squares[2] = {
        { .pos = { .x = 400, .y = 300 }, .size = 50 },
        { .pos = { .x = 100, .y = 100 }, .size = 100 },
    };
    size_t i;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BG);
        {
            p.pos = GetMousePosition();
            lines_update(lines, NUM_LINES, &p);
            lines_render(lines, NUM_LINES);
            particle_render(&p);

            for (i = 0; i < 2; i++)
                square_render(&squares[i]);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
