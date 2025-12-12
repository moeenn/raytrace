#include "raylib.h"
#include <math.h>
#include <stddef.h>
#include <stdint.h>

const char *WIN_TITLE = "Game";
constexpr int WIN_HEIGHT = 600;
constexpr int WIN_WIDTH = 800;
constexpr int FPS = 60;
constexpr int DEFAULT_SPEED = 8;
constexpr int RADIUS = 20;
constexpr Color BG = {0, 0, 0, 1};
constexpr Color PARTICLE_COLOR = {230, 41, 55, 255};
constexpr Color LINE_COLOR = {180, 32, 44, 255};
constexpr size_t NUM_LINES = 100;
constexpr int LINE_MAX_LENGTH = 800;

typedef struct {
  Vector2 pos;
  int speed;
} Particle;

typedef struct {
  Vector2 start;
  Vector2 end;
} Line;

typedef struct {
  Vector2 pos;
  int size;
} Square;

typedef enum : int8_t {
  NOOP,
  NORTH,
  NORTH_EAST,
  EAST,
  SOUTH_EAST,
  SOUTH,
  SOUTH_WEST,
  WEST,
  NORTH_WEST,
} Direction;

void particle_init(Particle *self, int x, int y) {
  self->pos = (Vector2){.x = x, .y = y};
  self->speed = DEFAULT_SPEED;
}

void particle_render(const Particle *p) {
  DrawCircle(p->pos.x, p->pos.y, RADIUS, PARTICLE_COLOR);
}

void particle_move(Particle *self, Direction d) {
  float diagonal_dist = self->speed * 0.7;

  switch (d) {
  case NORTH:
    self->pos.y -= self->speed;
    break;

  case NORTH_EAST:
    self->pos.y -= diagonal_dist;
    self->pos.x += diagonal_dist;
    break;

  case EAST:
    self->pos.x += self->speed;
    break;

  case SOUTH_EAST:
    self->pos.y += diagonal_dist;
    self->pos.x += diagonal_dist;
    break;

  case SOUTH:
    self->pos.y += self->speed;
    break;

  case SOUTH_WEST:
    self->pos.y += diagonal_dist;
    self->pos.x -= diagonal_dist;
    break;

  case WEST:
    self->pos.x -= self->speed;
    break;

  case NORTH_WEST:
    self->pos.y -= diagonal_dist;
    self->pos.x -= diagonal_dist;
    break;

  case NOOP:
    break;
  }
}

Direction direction_getFromInput() {
  if (IsKeyDown(KEY_UP) && IsKeyDown(KEY_RIGHT)) {
    return NORTH_EAST;
  }

  if (IsKeyDown(KEY_DOWN) && IsKeyDown(KEY_RIGHT)) {
    return SOUTH_EAST;
  }

  if (IsKeyDown(KEY_DOWN) && IsKeyDown(KEY_LEFT)) {
    return SOUTH_WEST;
  }

  if (IsKeyDown(KEY_UP) && IsKeyDown(KEY_LEFT)) {
    return NORTH_WEST;
  }

  if (IsKeyDown(KEY_UP)) {
    return NORTH;
  }

  if (IsKeyDown(KEY_DOWN)) {
    return SOUTH;
  }

  if (IsKeyDown(KEY_RIGHT)) {
    return EAST;
  }

  if (IsKeyDown(KEY_LEFT)) {
    return WEST;
  }

  return NOOP;
}

Vector2 calculateLineXAndY(int hypotenuse, float angle_degrees) {
  float angle_rad = angle_degrees * (3.142 / 180.0);
  float base = hypotenuse * cos(angle_rad);
  float height = hypotenuse * sin(angle_rad);
  return (Vector2){.x = base, .y = height};
}

typedef Line Lines[NUM_LINES];

void lines_update(Line lines[], size_t numLines, Particle *p) {
  size_t i;
  Vector2 end;
  Line *l;

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

void lines_render(Line lines[], size_t numLines) {
  size_t i;
  Line *l;

  for (i = 0; i < numLines; i++) {
    l = &lines[i];
    DrawLine(l->start.x, l->start.y, l->end.x, l->end.y, LINE_COLOR);
  }
}

void square_render(Square *self) {
  DrawRectangle(self->pos.x, self->pos.y, self->size, self->size, LINE_COLOR);
}

int main(void) {
  InitWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE);
  SetTargetFPS(FPS);

  Particle p;
  particle_init(&p, 200, 200);

  Lines lines = {};
  Square squares[2] = {
      {.pos = {.x = 400, .y = 300}, .size = 50},
      {.pos = {.x = 100, .y = 100}, .size = 100},
  };
  size_t i;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BG);
    {
      Direction d = direction_getFromInput();
      particle_move(&p, d);

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
