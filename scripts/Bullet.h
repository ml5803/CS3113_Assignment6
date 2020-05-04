#pragma once
#include "Enemy.h"
#include "Player.h"

class Bullet : public Entity {
public:
    Bullet(GLuint id);
    void Update(float deltaTime, Map* map, const std::vector<Entity*> objects = {}, Entity* target = nullptr) override;
};
