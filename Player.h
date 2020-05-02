#pragma once
#include "Entity.h"

class Player : public Entity {
public:
	int shootDirection = 3;
	int* animRight = NULL;
	int* animLeft = NULL;
	int* animUp = NULL;
	int* animDown = NULL;

	int* animIndices = NULL;
	int animFrames = 0;
	int animIndex = 0;
	float animTime = 0;
	int animCols = 1;
	int animRows = 1;

	Player(GLuint id, float w, float h);
	void Shoot();
	void Update(float deltaTime, Map* map, const std::vector<Entity*> objects = {}) override;
	void Render(ShaderProgram* program) override;
	void DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index);
};