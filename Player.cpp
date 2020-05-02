#include "Player.h"

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

void Player::Update(float deltaTime, Map* map, const std::vector<Entity*> objects)
{
	Entity::Update(deltaTime, map, objects);

	if (animIndices != NULL) {
		if (glm::length(movement) != 0) {
			animTime += deltaTime;

			if (animTime >= 0.25f)
			{
				animTime = 0.0f;
				animIndex++;
				if (animIndex >= animFrames)
				{
					animIndex = 0;
				}
			}
		}
		else {
			animIndex = 0;
		}
	}
}

void Player::Render(ShaderProgram* program)  {
	if (animIndices != NULL) {
		float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
		program->SetModelMatrix(modelMatrix);

		DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
		return;
	}

	Entity::Render(program);
}

void Player::DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index)
{
	float u = (float)(index % animCols) / (float)animCols;
	float v = (float)(index / animCols) / (float)animRows;

	float width = 1.0f / (float)animCols;
	float height = 1.0f / (float)animRows;

	float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
		u, v + height, u + width, v, u, v };

	float vertices[] = { -0.7, -0.7, 0.7, -0.7, 0.7, 0.7, -0.7, -0.7, 0.7, 0.7, -0.7, 0.7 };

	glBindTexture(GL_TEXTURE_2D, textureID);

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}