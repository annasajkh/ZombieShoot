#include "Zombie.h"

#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

void zombie_init(Zombie* zombie, Entity* target, Vector2 position, bool is_on_the_heap)
{
    if (zombie == NULL)
    {
        printf("allocate zombie first!\n");
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

    zombie->entity = entity;
    zombie->target = target;

    zombie->speed = 100;
    zombie->damage = 10;
    zombie->radius = 20;

    zombie->is_on_the_heap = is_on_the_heap;
}

void zombie_update(Zombie* zombie)
{
    Vector2 direction = { 0, 0 };

    if (zombie->target != NULL)
    {
        direction.x = zombie->target->position.x - zombie->entity->position.x;
        direction.y = zombie->target->position.y - zombie->entity->position.y;
    }

    direction = Vector2Normalize(direction);

    direction.x *= zombie->speed;
    direction.y *= zombie->speed;

    zombie->entity->velocity.x = direction.x;
    zombie->entity->velocity.y = direction.y;

    entity_update(zombie->entity);
}

void zombie_draw(Zombie* zombie)
{
    DrawCircleV(zombie->entity->position, zombie->radius, GREEN);
}

void zombie_delete(Zombie* zombie)
{
    entity_delete(zombie->entity);

    if (zombie->is_on_the_heap)
    {
        free(zombie);
    }
}