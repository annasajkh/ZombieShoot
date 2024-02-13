#include <raylib.h>
#include <stdlib.h>

#include "Bullet.h"
#include "Entity.h"

void bullet_init(Bullet* bullet, Vector2 position, Vector2 direction, bool is_on_the_heap)
{
    if (bullet == NULL)
    {
        printf("allocate bullet first!\n");
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

    bullet->entity = entity;

    bullet->radius = 5;
    bullet->speed = 1500;
    bullet->direction = direction;

    bullet->is_on_the_heap = is_on_the_heap;
}

void bullet_update(Bullet* bullet)
{
    bullet->entity->velocity.x = bullet->direction.x * bullet->speed;
    bullet->entity->velocity.y = bullet->direction.y * bullet->speed;

    entity_update(bullet->entity);
}

void bullet_draw(Bullet* bullet)
{
    DrawCircleV(bullet->entity->position, bullet->radius, BLACK);
}

void bullet_delete(Bullet* bullet)
{
    entity_delete(bullet->entity);
    
    if (bullet->is_on_the_heap)
    {
        free(bullet);
    }
}