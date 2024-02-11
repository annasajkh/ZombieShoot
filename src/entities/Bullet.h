#pragma once

#include <raylib.h>

#include "Entity.h"

typedef struct Bullet
{
    Entity* entity;

    float radius;
    float speed;
    Vector2 direction;

    bool is_on_the_heap;
} Bullet;

void bullet_init(Bullet* bullet, Vector2 position, Vector2 direction, bool is_on_the_heap);

void bullet_update(Bullet* bullet);
void bullet_draw(Bullet* bullet);

void bullet_delete(Bullet* bullet);