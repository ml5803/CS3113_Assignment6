#include <stdio.h>
#include "Bullet.h"

Bullet::Bullet(GLuint id) : Entity(id, 0.4f, 0.4f) {
	speed = 5.0f;
}

glm::vec3 Bullet::setBulletMovement(int shootDirection) {
	movement = glm::vec3(0);
	switch (shootDirection) {
	case 0: // shooting up
		movement.y = 1.0f;
		break;
	case 1: // shooting down
		movement.y = -1.0f;
		break;
	case 2: // shooting right
		movement.x = 1.0f;
		break;
	case 3: // shooting left
		movement.x = -1.0f;
		break;
	default:
		break;
	}
	return movement;
}

void Bullet::Update(float deltaTime, Map* map, const std::vector<Entity*> object, Entity* target){
	if (entityType == PLAYER_BULLET) {
		// Collide with enemies
		for (Entity* enemy : object) {
			if (CheckCollision(enemy)) {
				enemy->isActive = false;
				isActive = false;
				return;
			}
		}
	} else if (entityType == ENEMY_BULLET) {
		// Collide with player
		if (CheckCollision(target)) {
			target->isActive = false;
			isActive = false;
			return;
		}
	}
	
    Entity::Update(deltaTime, map, object, target);

	// Collide with map
	if (collidedTop || collidedBottom || collidedLeft || collidedRight) 
		isActive = false;
}
