#include <raylib.h>
#include <stdlib.h>
#include <stdbool.h>

#define SCREEN_SIZE 800
#define GRID_SIZE 20
#define GRID_SQUARE_SIZE 40

#define MOVE_INTERVAL 4

#define TRANSPARENT (Color){0, 0, 0, 0}

#define DIRECTION_UP 0
#define DIRECTION_DOWN 1
#define DIRECTION_RIGHT 2
#define DIRECTION_LEFT 3

//----------------------------------------------------------------------------------
// Structs
//----------------------------------------------------------------------------------

typedef struct
{
    Vector2 body[255];
    int direction;
    int size;
} Snake;

//----------------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------------

//
// Snake
//

Snake *InitSnake()
{
    Snake *snake = (Snake *)calloc(1, sizeof(Snake));
    snake->size = 1;
    snake->body[0] = (Vector2){400, 400};

    return snake;
}

void GrowSnake(Snake *snake)
{
    int size = snake->size;
    Vector2 *lastPart = &snake->body[size - 1];
    snake->body[size] = *lastPart;
    snake->size++;
}

void MoveSnake(Snake *snake)
{
    for (int i = snake->size - 1; i > 0; i--)
    {
        snake->body[i] = snake->body[i - 1];
    }

    if (snake->direction == DIRECTION_UP)
        snake->body[0] = (Vector2){snake->body[0].x, snake->body[0].y - GRID_SQUARE_SIZE};
    else if (snake->direction == DIRECTION_DOWN)
        snake->body[0] = (Vector2){snake->body[0].x, snake->body[0].y + GRID_SQUARE_SIZE};
    else if (snake->direction == DIRECTION_RIGHT)
        snake->body[0] = (Vector2){snake->body[0].x + GRID_SQUARE_SIZE, snake->body[0].y};
    else if (snake->direction == DIRECTION_LEFT)
        snake->body[0] = (Vector2){snake->body[0].x - GRID_SQUARE_SIZE, snake->body[0].y};

    if (snake->body[0].x > SCREEN_SIZE - GRID_SQUARE_SIZE)
        snake->body[0].x = 0;
    if (snake->body[0].x < 0)
        snake->body[0].x = SCREEN_SIZE - GRID_SQUARE_SIZE;
    if (snake->body[0].y > SCREEN_SIZE - GRID_SQUARE_SIZE)
        snake->body[0].y = 0;
    if (snake->body[0].y < 0)
        snake->body[0].y = SCREEN_SIZE - GRID_SQUARE_SIZE;
}

//
// Apple
//

Vector2 GenApple(Snake *snake)
{
    Vector2 vector2 = {GetRandomValue(0, GRID_SIZE - 1) * GRID_SQUARE_SIZE, GetRandomValue(0, GRID_SIZE - 1) * GRID_SQUARE_SIZE};

    for (int i = 0; i < snake->size; i++)
    {
        if (snake->body[i].x == vector2.x & snake->body[i].y == vector2.y)
            return GenApple(snake);
    }

    return vector2;
}

bool IsSnakeCollidingWithApple(Snake *snake, Vector2 apple)
{
    for (int i = 0; i < snake->size; i++)
    {
        if (snake->body[i].x == apple.x & snake->body[i].y == apple.y)
            return true;
    }

    return false;
}

//----------------------------------------------------------------------------------
// Main
//----------------------------------------------------------------------------------

int main()
{
    InitWindow(SCREEN_SIZE, SCREEN_SIZE, "Snake Game");
    SetTargetFPS(60);

    Snake *snake = InitSnake();
    Vector2 apple = GenApple(snake);
    float moveInterval = MOVE_INTERVAL;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground((Color){24, 24, 24, 255});

        // Snake Movement
        //
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
            snake->direction = DIRECTION_UP;
        else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
            snake->direction = DIRECTION_DOWN;
        else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
            snake->direction = DIRECTION_RIGHT;
        else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
            snake->direction = DIRECTION_LEFT;

        //
        // Slow the snake down
        //
        if (moveInterval == MOVE_INTERVAL)
        {
            MoveSnake(snake);
            moveInterval = 0;
        }
        else
        {
            moveInterval++;
        }

        if (IsSnakeCollidingWithApple(snake, apple))
        {
            GrowSnake(snake);
            apple = GenApple(snake);
        }

        //
        // Drawing
        //
        for (int i = snake->size - 1; i >= 0; i--)
        {
            if (i == 0)
                DrawRectangle(snake->body[i].x, snake->body[i].y, GRID_SQUARE_SIZE, GRID_SQUARE_SIZE, (Color){19, 120, 25, 255});
            else
                DrawRectangle(snake->body[i].x, snake->body[i].y, GRID_SQUARE_SIZE, GRID_SQUARE_SIZE, (Color){30, 179, 38, 255});
        }

        DrawRectangle(apple.x, apple.y, GRID_SQUARE_SIZE, GRID_SQUARE_SIZE, RED);

        EndDrawing();
    }

    free(snake);
    CloseWindow();
    return 0;
}