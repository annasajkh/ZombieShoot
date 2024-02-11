#include <stdlib.h>
#include <stdio.h>

#include "Entity.h"

void entity_init(Entity* entity, Vector2 position, Vector2 velocity, bool is_on_the_heap)
{
    if (entity == NULL)
    {
        printf("allocate entity first!\n");
        getchar();
        exit(1);
    }

    entity->position = position;
    entity->velocity = velocity;
    entity->is_on_the_heap = is_on_the_heap;
}

void entity_update(Entity* entity)
{
    entity->position.x += entity->velocity.x * GetFrameTime();
    entity->position.y += entity->velocity.y * GetFrameTime();
}

void entity_delete(Entity* entity)
{
    if (entity->is_on_the_heap)
    {
        free(entity);
    }
}