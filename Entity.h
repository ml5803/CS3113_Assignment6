#pragma once
#include "Map.h"

class Entity {
public:
	glm::vec3 position;
	glm::vec3 movement;
	glm::vec3 acceleration;
	glm::vec3 velocity;

	float vertices[12];
	float width, height;

	bool isActive = true;
	GLuint textureID;
	glm::mat4 modelMatrix;

	bool collidedTop = false;
	bool collidedBottom = false;
	bool collidedLeft = false;
	bool collidedRight = false;

	Entity(GLuint id, float w = 1, float h = 1);
	bool CheckCollision(const Entity* other) const;
	void CheckCollisionsX(const std::vector<Entity*> platforms);
	void CheckCollisionsY(const std::vector<Entity*> platforms);
	void CheckCollisionsX(Map* map);
	void CheckCollisionsY(Map* map);
	virtual void Update(float deltaTime, Map* map, const std::vector<Entity*> objects = {});
	virtual void Render(ShaderProgram* program);
};
