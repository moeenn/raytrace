#include "raylib.h"
#include "raymath.h"
#include <cstdio>

constexpr const char *WIN_TITLE = "Game";
constexpr int WIN_HEIGHT = 800;
constexpr int WIN_WIDTH = 1000;
constexpr int FPS = 60;
constexpr Color BG = {6, 0, 50, 255};
constexpr Color FG = {205, 0, 13, 255};
constexpr Color PARTICLE_COLOR = {230, 41, 55, 255};
constexpr Color LINE_COLOR = {180, 32, 44, 255};
constexpr int BOX_SIZE = 50;
constexpr int RADIUS = 10;
constexpr int NUM_LINES = 130;
constexpr int LINE_MAX_LENGTH = 1300;
constexpr int NUM_RECTANGLES = 10;

struct Line
{
    Vector2 start;
    Vector2 end;
};

bool getLineRectangleCollisionPoint(Line &line, const Rectangle &rect, Vector2 *collisionPoint)
{
    Vector2 rectEdges[4][2] = {
        {{rect.x, rect.y}, {rect.x + rect.width, rect.y}},                             // Top edge
        {{rect.x + rect.width, rect.y}, {rect.x + rect.width, rect.y + rect.height}},  // Right edge
        {{rect.x, rect.y + rect.height}, {rect.x + rect.width, rect.y + rect.height}}, // Bottom edge
        {{rect.x, rect.y}, {rect.x, rect.y + rect.height}}                             // Left edge
    };

    Vector2 closestPoint;
    float closestDist = 1e9f; // Start with a very large distance
    bool collisionFound = false;

    for (int i = 0; i < 4; i++)
    {
        Vector2 point;
        if (CheckCollisionLines(line.start, line.end, rectEdges[i][0], rectEdges[i][1], &point))
        {
            float dist = Vector2Distance(line.start, point);
            if (dist < closestDist)
            {
                closestDist = dist;
                closestPoint = point;
                collisionFound = true;
            }
        }
    }

    if (collisionFound && collisionPoint != nullptr)
    {
        *collisionPoint = closestPoint;
    }

    return collisionFound;
}

void initRectangles(Rectangle rects[], size_t numRects)
{
    size_t i;
    for (i = 0; i < numRects; i++)
    {
        rects[i] = {
            static_cast<float>(GetRandomValue(BOX_SIZE, WIN_WIDTH - BOX_SIZE)),
            static_cast<float>(GetRandomValue(BOX_SIZE, WIN_HEIGHT - BOX_SIZE)),
            BOX_SIZE,
            BOX_SIZE,
        };
    }
}

Vector2 calculateLineXAndY(int hypotenuse, float angleDegrees)
{
    float angle_rad = angleDegrees * (3.142 / 180.0);
    float base = hypotenuse * cos(angle_rad);
    float height = hypotenuse * sin(angle_rad);
    Vector2 result = {.x = base, .y = height};
    return result;
}

void linesUpdate(Line lines[], size_t numLines, Vector2 &lightSource)
{
    size_t i;
    Vector2 end;
    Line *l;

    float deg = 90;
    int step = (360 / numLines) + 1; // 1 is line width.

    for (i = 0; i < numLines; i++)
    {
        l = &lines[i];
        l->start = lightSource;
        end = calculateLineXAndY(LINE_MAX_LENGTH, deg);
        l->end.x = l->start.x + end.x;
        l->end.y = l->start.y + end.y;
        deg += (float)(step % 360);
    }
}

void linesRender(Line lines[], size_t numLines, const Rectangle rects[], size_t numRects)
{
    size_t i, r;
    Line l;
    Rectangle rect;
    Vector2 c;

    for (i = 0; i < numLines; i++)
    {
        l = lines[i];
        for (r = 0; r < numRects; r++)
        {
            rect = rects[r];
            if (getLineRectangleCollisionPoint(l, rect, &c))
            {
                l.end = c;
            }
        }

        DrawLine(l.start.x, l.start.y, l.end.x, l.end.y, LINE_COLOR);
    }
}

void lineRender(Line &line, const Rectangle rects[], size_t numRectangles)
{
    size_t i;
    for (i = 0; i < numRectangles; i++)
    {
        Vector2 c;
        if (getLineRectangleCollisionPoint(line, rects[i], &c))
        {
            line.end = c;
        }

        DrawLine(line.start.x, line.start.y, line.end.x, line.end.y, LINE_COLOR);
    }
}

int main()
{
    InitWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE);
    SetTargetFPS(FPS);

    Line lines[NUM_LINES];
    Vector2 s = GetMousePosition();

    Rectangle rects[NUM_RECTANGLES];
    initRectangles(rects, NUM_RECTANGLES);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BG);
        {
            for (size_t r = 0; r < NUM_RECTANGLES; r++)
                DrawRectangleRec(rects[r], FG);

            s = GetMousePosition();
            DrawCircle(s.x, s.y, RADIUS, PARTICLE_COLOR);
            linesUpdate(lines, NUM_LINES, s);
            linesRender(lines, NUM_LINES, rects, NUM_RECTANGLES);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
