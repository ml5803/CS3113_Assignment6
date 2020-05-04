#pragma once
#include "Entity.h"
#include "Bullet.h"

class Player : public Entity {
public:
	Player(GLuint id, float w, float h);
	void Shoot();
	void Update(float deltaTime, Map* map, const std::vector<Entity*> objects = {}, Entity* target = nullptr) override;
	//void Render(ShaderProgram* program) override;
};
