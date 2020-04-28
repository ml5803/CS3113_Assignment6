#include "Player.h"

Player::Player(GLuint id, float w, float h) : Entity(id, w, h) {
	//state.player->animRight = new int[2]{ 2, 3 };
	//state.player->animLeft = new int[2]{ 0, 1 };
	//state.player->animIndices = state.player->animRight;
	//state.player->animFrames = 2;
	//state.player->animIndex = 0;
	//state.player->animTime = 0;
	//state.player->animCols = 2;
	//state.player->animRows = 2;
}

Player::Player(const Entity& entity) : Entity(entity) {}

void Player::Aim(bool clockwise)
{
	if (clockwise) rotateDegree -= 1;
	else rotateDegree += 1;
}

void Player::Shoot()
{
	rotateDegree += 90;
	while (rotateDegree > 360) { rotateDegree -= 360; }
	while (rotateDegree < 0) { rotateDegree += 360; }
	movement = glm::vec3(cos(rotateDegree), sin(rotateDegree), 0);
	rotateDegree -= 90;
}

void Player::Update(float deltaTime, Map* map, const std::vector<Entity*> objects)
{
	Entity::Update(deltaTime, map, objects);
	float degree = rotateDegree * 0.0003;
	modelMatrix *= glm::rotate(modelMatrix, glm::degrees(degree), glm::vec3(0.0f, 0.0f, 1.0f));

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