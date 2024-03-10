#pragma once

#include "Entity.h"
#include "Zombie.h"
#include "../utils/Timer.h"

typedef struct Player
{
    Entity* entity;
    Zombie* intersecting_zombie;

    float radius;
    float health;
    float speed;

    bool is_on_the_heap;

    Timer* hurt_timer;
} Player;

void player_init(Player* player, Vector2 position, bool is_on_the_heap);

void player_update(Player* player);

void player_hurt(void* sender);

void player_draw(Player* player);

void player_delete(Player* player);