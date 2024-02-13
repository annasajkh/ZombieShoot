#include "Player.h"

#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

void player_init(Player* player, Vector2 position, bool is_on_the_heap)
{
    if (player == NULL)
    {
        printf("allocate player first!\n");
        getchar();
        exit(1);
    }

    Entity* entity = malloc(sizeof(Entity));

    if (entity == NULL)
    { 
        printf("cannot allocate entity");
        getchar();
        exit(1);
    }

    entity_init(entity, position, (Vector2) { 0, 0 }, true);

    player->entity = entity;

    player->intersecting_zombie = NULL;
    player->radius = 20;
    player->health = 100;
    player->speed = 100;

    player->is_on_the_heap = is_on_the_heap;
    player->is_intersecting_with_zombie = false;

    player->hurt_timer = malloc(sizeof(Timer));

    timer_init(player->hurt_timer, 0.5f, false, true, player_hurt);
}

void player_hurt(void* sender)
{
    Player* player = (Player*)(sender);

    if (player->is_intersecting_with_zombie)
    {
        player->health -= player->intersecting_zombie->damage;
    }
}

void player_update(Player* player)
{
    Vector2 direction = { 0, 0 };

    if (IsKeyDown(KEY_A))
    {
        direction.x += -1;
    }
    if (IsKeyDown(KEY_D))
    {
        direction.x += 1;
    }
    if (IsKeyDown(KEY_W))
    {
        direction.y += -1;
    }
    if (IsKeyDown(KEY_S))
    {
        direction.y += 1;
    }

    direction = Vector2Normalize(direction);
    
    direction.x *= player->speed;
    direction.y *= player->speed;

    player->entity->velocity.x = direction.x;
    player->entity->velocity.y = direction.y;

    entity_update(player->entity);
}

void player_draw(Player* player)
{
    DrawCircleV(player->entity->position, player->radius, BLUE);
}

void player_delete(Player* player)
{
    entity_delete(player->entity);

    if (player->is_on_the_heap)
    {
        timer_delete(player->hurt_timer);
        free(player);
    }
}