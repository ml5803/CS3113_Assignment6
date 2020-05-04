#pragma once
#include "Entity.h"
#include "Player.h"
#include "Bullet.h"


class Enemy : public Entity {
public:
    Enemy(GLuint id, float w, float h);
    void Shoot(Entity* target);
    void Update(float deltaTime, Map* map, const std::vector<Entity*> objects = {}, Entity* target = nullptr) override;
    //void Render(ShaderProgram* program) override;
    void AIChaser(Entity* target);
    void AIShooter(Entity* target);
};

