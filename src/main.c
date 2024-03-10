#ifdef _MSC_VER
#include <malloc.h>
#define alloca _malloca
#endif

#ifndef _MSC_VER
#include "stdlib.h"
#endif
#include <time.h>

#include <raylib.h>
#include <raymath.h>
#include "entities/Player.h"
#include "utils/Timer.h"

#include "entities/Bullet.h"
#include "entities/Zombie.h"

#define STB_DS_IMPLEMENTATION
#include "externals/stb_ds.h"

static Player* player;

static Bullet* bullets = NULL;
static Zombie* zombies = NULL;

static Timer* bullet_spawn_timer;
static Timer* zombie_spawn_timer;

static int score = 0;
static int highest_score = 0;


static void spawn_bullet(void* sender)
{
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 bullet_dir = { 0, 0 };
        Vector2 mouse_position = GetMousePosition();

        bullet_dir.x = mouse_position.x - player->entity->position.x;
        bullet_dir.y = mouse_position.y - player->entity->position.y;

        bullet_dir = Vector2Normalize(bullet_dir);

        Bullet* bullet = alloca(sizeof(Bullet));

        if (bullet == NULL)
        {
            printf("cannot allocate bullet");
            getchar();
            exit(1);
        }

        bullet_init(bullet, player->entity->position, bullet_dir, false);

        arrpush(bullets, *bullet);
    }
}


static void spawn_zombie(void* sender)
{
    Zombie* zombie = alloca(sizeof(Zombie));

    if (zombie == NULL)
    {
        printf("cannot allocate zombie");
        getchar();
        exit(1);
    }

    int randomWidth = rand() % GetScreenWidth();
    int randomHeight = rand() % GetScreenHeight();

    while (Vector2Distance((Vector2) { randomWidth, randomHeight }, player->entity->position) < 300)
    {
        randomWidth = rand() % GetScreenWidth();
        randomHeight = rand() % GetScreenHeight();
    }

    zombie_init(zombie, ((Player*)sender)->entity, (Vector2) { randomWidth, randomHeight }, false);

    arrpush(zombies, *zombie);
}


static void init_game()
{
    srand(time(NULL));

    if (score > highest_score)
    {
        highest_score = score;
    }

    score = 0;

    // release bullets if it exist
    if (bullets != NULL)
    {
        for (size_t i = 0; i < arrlen(bullets); i++)
        {
            Bullet bullet = bullets[i];

            bullet_delete(&bullet);
        }

        arrfree(bullets);
    }

    // release zombies if it exist
    if (zombies != NULL)
    {
        for (size_t i = 0; i < arrlen(zombies); i++)
        {
            Zombie zombie = zombies[i];

            zombie_delete(&zombie);
        }
        
        arrfree(zombies);
    }


    // player
    player = alloca(sizeof(Player));

    if (player == NULL)
    {
        printf("cannot allocate player");
        getchar();
        exit(1);
    }

    player_init(player, (Vector2) { 960 / 2, 540 / 2 }, false);


    // bullet_spawn_timer
    bullet_spawn_timer = malloc(sizeof(Timer));

    if (bullet_spawn_timer == NULL)
    {
        printf("cannot allocate bullet_spawn_timer");
        getchar();
        exit(1);
    }

    timer_init(bullet_spawn_timer, 0.1f, false, true, spawn_bullet);
    timer_start(bullet_spawn_timer);


    // zombie_spawn_timer
    zombie_spawn_timer = malloc(sizeof(Timer));

    if (zombie_spawn_timer == NULL)
    {
        printf("cannot allocate zombie_spawn_timer");
        getchar();
        exit(1);
    }

    timer_init(zombie_spawn_timer, 1.0f, false, true, spawn_zombie);
    timer_start(zombie_spawn_timer);
}


static void update()
{
    timer_update(bullet_spawn_timer, NULL);
    timer_update(zombie_spawn_timer, player);

    player->intersecting_zombie = NULL;

    for (size_t i = 0; i < arrlen(zombies); i++)
    {
        Zombie zombie = zombies[i];

        // Check collision for player and zombie
        if (CheckCollisionCircles(player->entity->position, player->radius, zombie.entity->position, zombie.radius))
        {
            player->intersecting_zombie = &zombie;
        }
    }

    // Why 2 separate loop? because it's more stable!

    if (zombies != NULL && bullets != NULL)
    {
        for (size_t i = arrlen(zombies); i-- > 0;)
        {
            Zombie zombie = zombies[i];

            for (size_t j = arrlen(bullets); j-- > 0;)
            {
                Bullet bullet = bullets[j];

                // Check collision for zombie and bullet
                if (CheckCollisionCircles(zombie.entity->position, zombie.radius, bullet.entity->position, bullet.radius))
                {
                    arrdel(bullets, j);
                    arrdel(zombies, i);
                    
                    zombie_spawn_timer->wait_time -= 0.01f; // make zombie spawn faster every time you killed one
                    score++;
                }
            }
        }
    }

    player_update(player);

    if (player->health <= 0)
    {
        init_game();
    }

    for (size_t i = 0; i < arrlen(bullets); i++)
    {
        bullet_update(&(bullets[i]));
    }

    for (size_t i = 0; i < arrlen(zombies); i++)
    {
        Zombie zombie = zombies[i];

        zombie_update(&zombie);
    }

    for (size_t i = arrlen(bullets); i-- > 0;)
    {
        Bullet bullet = bullets[i];

        if (bullet.entity->position.x > GetScreenWidth() + bullet.radius ||
            bullet.entity->position.x < -bullet.radius ||
            bullet.entity->position.y > GetScreenHeight() + bullet.radius ||
            bullet.entity->position.y < -bullet.radius)
        {
            bullet_delete(&bullet);
            arrdel(bullets, i);
        }
    }
}


static void draw_ui()
{
    DrawRectangle(10, 10, player->health, 30, GREEN);
    DrawText("Score: ", 10, 50, 30, WHITE);

    char score_str[10];
    sprintf(score_str, "%d", score);

    DrawText(score_str, 115, 50, 30, WHITE);

    DrawText("Highest Score: ", 10, 80, 30, WHITE);

    char highest_score_str[10];
    sprintf(highest_score_str, "%d", highest_score);

    DrawText(highest_score_str, 240, 80, 30, WHITE);

}


static void draw()
{
    for (size_t i = 0; i < arrlen(bullets); i++)
    {
        Bullet bullet = bullets[i];

        bullet_draw(&bullet);
    }

    player_draw(player);

    for (size_t i = 0; i < arrlen(zombies); i++)
    {
        Zombie zombie = zombies[i];

        zombie_draw(&zombie);
    }

    draw_ui();
}


int main()
{
    init_game();

    InitWindow(960, 540, "Zombie Shoot");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        update();

        BeginDrawing();
        ClearBackground(DARKGREEN);

        draw();

        EndDrawing();
    }

    player_delete(player);

    CloseWindow();
    return 0;
}