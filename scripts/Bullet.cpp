#include <stdio.h>
#include "Bullet.h"

Bullet::Bullet(GLuint id, float w, float h) : Entity(id, w, h) {
    animIndices = animLeft;
    animFrames = 1;
    animIndex = 0;
    animTime = 0;
    animCols = 1;
    animRows = 1;
    
    entityType = PLAYER_BULLET;
}

void Bullet::Update(float deltaTime, Map* map, const std::vector<Entity*> object, Entity* target){
    Entity::Update(deltaTime, map, object, target);
}
