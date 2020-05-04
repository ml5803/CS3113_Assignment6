#include "Player.h"
#include "Bullet.h"

Player::Player(GLuint id, float w, float h) : Entity(id, w, h) {
	animUp = new int[1]{ 0 };
	animDown = new int[1]{ 1 };
	animRight = new int[1]{ 2 };
	animLeft = new int[1]{ 3 };
	animIndices = animLeft;
	animFrames = 1;
	animIndex = 0;
	animTime = 0;
	animCols = 2;
	animRows = 2;
    
    entityType = PLAYER;
}

void Player::Shoot()
{
	float distance = 3.0f;
 	switch (shootDirection) {
	case 0: // shooting up
		movement.y = -distance;
		break;
	case 1: // shooting down
		movement.y = distance;
		break;
	case 2: // shooting right
		movement.x = -distance;
		break;
	case 3: // shooting left
		movement.x = distance;
		break;
	default:
		break;
	}
	/*rotateDegree += 90;
	while (rotateDegree > 360) { rotateDegree -= 360; }
	while (rotateDegree < 0) { rotateDegree += 360; }
	movement = glm::vec3(cos(rotateDegree), sin(rotateDegree), 0);
	rotateDegree -= 90;*/
}

void Player::Update(float deltaTime, Map* map, const std::vector<Entity*> objects, Entity* target)
{
	Entity::Update(deltaTime, map, objects, target);
    movement = glm::vec3(0);
}
/*
void Player::Render(ShaderProgram* program)  {
	if (animIndices != NULL) {
		float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
		program->SetModelMatrix(modelMatrix);

		DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
		return;
	}

	Entity::Render(program);
}*/
