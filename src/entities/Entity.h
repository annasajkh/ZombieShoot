#pragma once

#include <raylib.h>

typedef struct Entity
{
    Vector2 position;
    Vector2 velocity;

    bool is_on_the_heap;
} Entity;

void entity_init(Entity* entity, Vector2 position, Vector2 velocity, bool is_on_the_heap);

void entity_update(Entity* entity);

void entity_delete(Entity* entity);