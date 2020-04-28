#include "Entity.h"
using namespace std;

Entity::Entity(GLuint id, float w, float h)
{
	textureID = id;
	width = w; height = h;

	position = glm::vec3(0);
	movement = glm::vec3(0);
	modelMatrix = glm::mat4(1.0f);

	vertices[0] = position.x - w / 2;
	vertices[1] = position.y - h / 2;
	vertices[2] = position.x + w / 2;
	vertices[3] = position.y - h / 2;
	vertices[4] = position.x + w / 2;
	vertices[5] = position.y + h / 2;
	vertices[6] = position.x - w / 2;
	vertices[7] = position.y - h / 2;
	vertices[8] = position.x + w / 2;
	vertices[9] = position.y + h / 2;
	vertices[10] = position.x - w / 2;
	vertices[11] = position.y + h / 2;
}

bool Entity::CheckCollision(const Entity* other) const
{
	if (!isActive || !other->isActive) return false;

	float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
	float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
	return (xdist < 0 && ydist < 0);
}

void Entity::Update(float deltaTime, Map* map, const vector<Entity*> objects)
{
	if (!isActive) return;

	position.y += movement.y * deltaTime; // Move on Y
	CheckCollisionsY(map);
	CheckCollisionsY(objects); // Fix if needed

	position.x += movement.x * deltaTime; // Move on X
	CheckCollisionsX(map);
	CheckCollisionsX(objects); // Fix if needed

	movement = glm::vec3(0);

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::Render(ShaderProgram* program)
{
	float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
	program->SetModelMatrix(modelMatrix);

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glDrawArrays(GL_TRIANGLES, 0, 6);

}

void Entity::CheckCollisionsX(const vector<Entity*> platforms)
{
	for (Entity* object: platforms)
	{
		if (CheckCollision(object))
		{
			float xdist = fabs(position.x - object->position.x);
			float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
			if (velocity.x > 0) {
				position.x -= penetrationX;
				velocity.x = 0;
			}
			else if (velocity.x < 0) {
				position.x += penetrationX;
				velocity.x = 0;
			}
		}
	}
}

void Entity::CheckCollisionsY(const vector<Entity*> platforms)
{
	for (Entity* object : platforms)
	{
		if (CheckCollision(object))
		{
			float ydist = fabs(position.y - object->position.y);
			float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
			if (velocity.y > 0) {
				position.y -= penetrationY;
				velocity.y = 0;
			}
			else if (velocity.y < 0) {
				position.y += penetrationY;
				velocity.y = 0;
			}
		}
	}
}

void Entity::CheckCollisionsY(Map* map)
{
	// Probes for tiles
	glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
	glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
	glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);

	glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
	glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
	glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);

	float penetration_x = 0;
	float penetration_y = 0;
	if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
		position.y -= penetration_y;
		velocity.y = 0;
		collidedTop = true;
	}
	else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
		position.y -= penetration_y;
		velocity.y = 0;
		collidedTop = true;
	}
	else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
		position.y -= penetration_y;
		velocity.y = 0;
		collidedTop = true;
	}
	if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
		position.y += penetration_y;
		velocity.y = 0;
		collidedBottom = true;
	}
	else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
		position.y += penetration_y;
		velocity.y = 0;
		collidedBottom = true;
	}
	else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
		position.y += penetration_y;
		velocity.y = 0;
		collidedBottom = true;
	}
}

void Entity::CheckCollisionsX(Map* map)
{
	// Probes for tiles
	glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
	glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);

	float penetration_x = 0;
	float penetration_y = 0;
	if (map->IsSolid(left, &penetration_x, &penetration_y) && velocity.x < 0) {
		position.x += penetration_x;
		velocity.x = 0;
		collidedLeft = true;
	}

	if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
		position.x -= penetration_x;
		velocity.x = 0;
		collidedRight = true;
	}
}