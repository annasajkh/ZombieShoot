#pragma once

#include "Entity.h"

typedef struct Zombie
{
    Entity* entity;
    Entity* target;

    float speed;
    float damage;
    float radius;

    bool is_on_the_heap;
} Zombie;

void zombie_init(Zombie* zombie, Entity* target, Vector2 position, bool is_on_the_heap);

void zombie_update(Zombie* zombie);
void zombie_draw(Zombie* zombie);

void zombie_delete(Zombie* zombie);