#include <raylib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

// Structs
typedef struct
{
    int size;
    int speed;
    int mapWidth;
    int mapHeight;
    int windowWidth;
    int windowHeight;
}
Config;

typedef struct
{
    int x;
    int y;
}
Pos;

typedef enum
{
    RIGHT,
    DOWN,
    LEFT,
    UP,
    NONE
}
Direction;

typedef enum
{
    RUNNING,
    PAUSED,
    GAME_OVER
}
State;

// Global variables
Config config;
State state;
int score;
int high_score;
Pos *snake;
Pos food;
Direction input;
Direction direction;

// Function prototypes
void init(void);
void quit(void);
void save_high_score(void);
void toggle_pause(void);
void track_pause(void);
void track_restart(void);
void track_quit(void);
void track_arrow_keys(void);
void change_direction(void);
void spawn_food(void);
void eat(void);
void move(void);
void game_over(void);
void draw_game(void);
void draw_game_over(void);
void draw_paused(void);

int main(void)
{
    init();

    int tick = 0;

    while (!WindowShouldClose())
    {
        track_pause();
        track_restart();
        track_quit();
        track_arrow_keys();

        if (tick % config.speed == 0)
        {
            tick = 0;

            if (state == RUNNING)
            {
                change_direction();
                eat();
                move();
                game_over();
            }
        }

        tick++;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (state == RUNNING)
        {
            draw_game();
        }
        else if (state == PAUSED)
        {
            draw_game();
            draw_paused();
        }
        else
        {
            draw_game_over();
        }

        EndDrawing();
    }

    save_high_score();
    quit();
}

void game_over(void)
{
    const bool is_right_border = snake[0].x == config.mapWidth;
    const bool is_bottom_border = snake[0].y == config.mapHeight;
    const bool is_left_border = (snake[0].x + config.size) == 0;
    const bool is_top_border = (snake[0].y + config.size) == 0;
    const bool is_at_border = is_right_border || is_bottom_border || is_left_border || is_top_border;
    bool is_game_over = false;

    if (is_at_border)
    {
        is_game_over = true;
    }

    if (!is_game_over)
    {
        for (int i = 1; i < score; i++)
        {
            const bool is_self_coliding = snake[0].x == snake[i].x && snake[i].y == snake[0].y;

            if (is_self_coliding)
            {
                is_game_over = true;
                break;
            }
        }
    }

    if (is_game_over)
    {
        if (score > high_score)
        {
            high_score = score;
        }
        state = GAME_OVER;
    }
}

void move(void)
{
    for (int i = score - 1; i > 0; i--)
    {
        snake[i] = snake[i - 1];
    }

    if (direction == RIGHT)
    {
        snake[0].x += config.size;
    }
    if (direction == DOWN)
    {
        snake[0].y += config.size;
    }
    if (direction == LEFT)
    {
        snake[0].x -= config.size;
    }
    if (direction == UP)
    {
        snake[0].y -= config.size;
    }
}

void eat(void)
{
    const bool is_eating = snake[0].x == food.x && snake[0].y == food.y;
    if (!is_eating)
    {
        return;
    }

    score++;
    snake = realloc(snake, sizeof(Pos) * score);

    if (snake == NULL)
    {
        fprintf(stderr, "Failed to reallocate memory for snake\n");
        exit(EXIT_FAILURE);
    }

    spawn_food();
}

void spawn_food(void)
{
    int x = (rand() % (config.mapWidth / config.size)) * config.size;
    int y = (rand() % (config.mapHeight / config.size)) * config.size;

    for (int i = 0; i < score; i++)
    {
        while (snake[i].x == x && snake[i].y == y)
        {
            x = (rand() % (config.mapWidth / config.size)) * config.size;
            y = (rand() % (config.mapHeight / config.size)) * config.size;
        }
    }

    food.x = x;
    food.y = y;
}

void change_direction(void)
{
    if (input == RIGHT && direction != LEFT)
    {
        direction = input;
    }
    if (input == DOWN && direction != UP)
    {
        direction = input;
    }
    if (input == LEFT && direction != RIGHT)
    {
        direction = input;
    }
    if (input == UP && direction != DOWN)
    {
        direction = input;
    }
}

void draw_start(void)
{
    if (direction != NONE)
    {
        return;
    }

    const char *start = "Press ARROW to start";
    const int font_size = 20;

    DrawText(start, (config.mapWidth - MeasureText(start, font_size)) / 2,
             (config.mapHeight / 2 - font_size / 2) - 40, font_size, RED);
}

void draw_food(void)
{
    DrawRectangle(food.x, food.y, config.size, config.size, BLUE);
    DrawRectangleLines(food.x, food.y, config.size, config.size, BLACK);
}

void draw_snake(void)
{
    for (int i = 0; i < score; i++)
    {
        DrawRectangle(snake[i].x, snake[i].y, config.size, config.size, RED);
        DrawRectangleLines(snake[i].x, snake[i].y, config.size, config.size, BLACK);
    }
}

void draw_score(void)
{
    char score_text[50];
    snprintf(score_text, sizeof(score_text), "Score: %d", score);

    char high_score_text[50];
    snprintf(high_score_text, sizeof(high_score_text), "High Score: %d", high_score);

    DrawText(score_text, 10, config.windowHeight - 30, 20, BLACK);
    DrawText(high_score_text, config.windowWidth - MeasureText(high_score_text, 20) - 10,
             config.windowHeight - 30, 20, BLACK);
}

void draw_border(void)
{
    DrawRectangleLines(0, 0, config.mapWidth, config.mapHeight, GRAY);
}

void draw_game(void)
{
    draw_border();
    draw_start();
    draw_food();
    draw_snake();
    draw_score();
}

void draw_game_over(void)
{
    char score_text[50];
    snprintf(score_text, sizeof(score_text), "Score: %d", score);

    const char *game_over = "GAME OVER!";
    const char *retry = "Press R to retry or ESC to quit";

    const int go_font_size = 40;
    const int s_font_size = 20;
    const int r_font_size = 20;

    const int go_text_width = MeasureText(game_over, go_font_size);
    const int s_text_width = MeasureText(score_text, s_font_size);
    const int r_text_width = MeasureText(retry, r_font_size);

    DrawText(game_over, (config.mapWidth - go_text_width) / 2,
             config.mapHeight / 2 - go_font_size / 2, go_font_size, RED);
    DrawText(score_text, (config.mapWidth - s_text_width) / 2,
             (config.mapHeight / 2 - s_font_size / 2) + 40, s_font_size, RED);
    DrawText(retry, (config.mapWidth - r_text_width) / 2,
             (config.mapHeight / 2 - r_font_size / 2) + 100, r_font_size, RED);
}

void draw_paused(void)
{
    const char *paused = "PAUSED";
    const char *resume = "Press P to resume";

    const int p_font_size = 40;
    const int r_font_size = 20;

    const int p_text_width = MeasureText(paused, p_font_size);
    const int r_text_width = MeasureText(resume, r_font_size);

    DrawText(paused, (config.mapWidth - p_text_width) / 2,
             config.mapHeight / 2 - p_font_size / 2, p_font_size, BLACK);
    DrawText(resume, (config.mapWidth - r_text_width) / 2,
             (config.mapHeight / 2 - r_font_size / 2) + 40, r_font_size, BLACK);
}

void track_arrow_keys(void)
{
    if (IsKeyPressed(KEY_RIGHT))
    {
        input = RIGHT;
    }
    if (IsKeyPressed(KEY_DOWN))
    {
        input = DOWN;
    }
    if (IsKeyPressed(KEY_LEFT))
    {
        input = LEFT;
    }
    if (IsKeyPressed(KEY_UP))
    {
        input = UP;
    }
}

void init_game(void)
{
    input = NONE;
    direction = NONE;
    state = RUNNING;
    score = 1;
    snake = malloc(sizeof(Pos) * score);

    if (snake == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for snake\n");
        exit(EXIT_FAILURE);
    }

    Pos head = {config.mapWidth / 2, config.mapHeight / 2};
    snake[0] = head;

    spawn_food();
}

void init_config(void)
{
    srand(time(NULL));

    // Custom config
    config.size = 10;
    config.speed = 3;
    config.mapWidth = 440;
    config.mapHeight = 400;
    config.windowWidth = 440;
    config.windowHeight = 440;

    // Raylib
    InitWindow(config.windowWidth, config.windowHeight, "CNAKE");
    SetTargetFPS(60);
}

void init_high_score(void)
{
    FILE *file = fopen("./db/high_score.txt", "r");

    if (file == NULL)
    {
        fprintf(stderr, "Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d", &high_score);
    fclose(file);
}

void init(void)
{
    init_config();
    init_high_score();
    init_game();
}

void track_restart(void)
{
    if (IsKeyPressed(KEY_R) && state == GAME_OVER)
    {
        free(snake);
        init_game();
    }
}

void track_pause(void)
{
    if (IsKeyPressed(KEY_P))
    {
        toggle_pause();
    }
}

void track_quit(void)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        quit();
    }
}

void toggle_pause(void)
{
    if (state == PAUSED)
    {
        state = RUNNING;
    }
    else
    {
        state = PAUSED;
    }
}

void quit(void)
{
    free(snake);
    CloseWindow();
    exit(EXIT_SUCCESS);
}

void save_high_score(void)
{
    FILE *file = fopen("./db/high_score.txt", "w");

    if (file == NULL)
    {
        fprintf(stderr, "Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%d", high_score);
    fclose(file);
}
