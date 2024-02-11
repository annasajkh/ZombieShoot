#ifdef _MSC_VER
#include <malloc.h>
#define alloca _malloca
#endif

#ifndef _MSC_VER
#include "stdlib.h"
#endif

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


static void spawn_bullet(void* sender)
{
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 bullet_dir = { 0, 0 };
        Vector2 mouse_position = GetMousePosition();

        bullet_dir.x = mouse_position.x - player->entity->position.x;
        bullet_dir.y = mouse_position.y - player->entity->position.y;

        bullet_dir = Vector2Normalize(bullet_dir);

        Bullet* bullet = malloc(sizeof(Bullet));

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
    Zombie* zombie = malloc(sizeof(Zombie));

    if (zombie == NULL)
    {
        printf("cannot allocate zombie");
        getchar();
        exit(1);
    }

    zombie_init(zombie, ((Player*)sender)->entity, (Vector2){ 100, 100 }, true);

    arrpush(zombies, *zombie);
}


static void init_game()
{
    // repopulate bullets if it exist
    if (bullets != NULL)
    {
        for (size_t i = 0; i < arrlen(bullets); i++)
        {
            Bullet bullet = bullets[i];

            bullet_delete(&bullet);
        }

        arrfree(bullets);

        bullets = NULL;
    }

    // repopulate zombies if it exist
    if (zombies != NULL)
    {
        for (size_t i = 0; i < arrlen(zombies); i++)
        {
            Zombie zombie = zombies[i];

            zombie_delete(&zombie);
        }
        
        arrfree(zombies);

        zombies = NULL;
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
    bullet_spawn_timer = alloca(sizeof(Timer));

    if (bullet_spawn_timer == NULL)
    {
        printf("cannot allocate bullet_spawn_timer");
        getchar();
        exit(1);
    }

    timer_init(bullet_spawn_timer, 0.1f, false, false, spawn_bullet);


    // zombie_spawn_timer
    zombie_spawn_timer = alloca(sizeof(Timer));

    if (zombie_spawn_timer == NULL)
    {
        printf("cannot allocate zombie_spawn_timer");
        getchar();
        exit(1);
    }

    timer_init(zombie_spawn_timer, 1.0f, false, false, spawn_zombie);
}


static void update()
{
    timer_update(bullet_spawn_timer, NULL);
    timer_update(zombie_spawn_timer, player);

    player_update(player);

    for (size_t i = 0; i < arrlen(zombies); i++)
    {
        Zombie zombie = zombies[i];

        if (CheckCollisionCircles(player->entity->position, player->radius, zombie.entity->position, zombie.radius))
        {
            Zombie zombie = zombies[i];

            player->intersecting_zombie = &zombie;
            player->is_intersecting_with_zombie = true;
        }
        else
        {
            player->intersecting_zombie = NULL;
            player->is_intersecting_with_zombie = false;
        }
    }


    timer_update(player->hurt_timer, player);

    if (player->health <= 0)
    {
        init_game();
    }

    for (size_t i = 0; i < arrlen(bullets); i++)
    {
        bullet_update(&(bullets[i]));
    }

    for (size_t i = arrlen(bullets); i-- > 0;)
    {
        Bullet bullet = bullets[i];

        if (bullet.entity->position.x > GetScreenWidth() + bullet.radius ||
            bullet.entity->position.x < -bullet.radius ||
            bullet.entity->position.y > GetScreenHeight() + bullet.radius ||
            bullet.entity->position.y < -bullet.radius)
        {
            arrdel(bullets, i);
            bullet_delete(&bullet);
        }
    }
}


static void draw_ui()
{
    DrawRectangle(10, 10, player->health, 30, GREEN);
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