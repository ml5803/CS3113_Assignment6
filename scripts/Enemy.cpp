#include <stdio.h>
#include "Enemy.h"

Enemy::Enemy(GLuint id, float w, float h) : Entity(id, w, h) {
    animUp = new int[1]{ 2 };
    animDown = new int[1]{ 0 };
    animRight = new int[1]{ 1 };
    animLeft = new int[1]{ 3 };
    animIndices = animUp;
    animFrames = 1;
    animIndex = 0;
    animTime = 0;
    animCols = 2;
    animRows = 2;
}

void Enemy::Shoot(Entity* target)
{
    //to be implemented
}

void Enemy::AIChaser(Entity* target){
    if (target == nullptr) return;
    
    //movement
    if (target->position.x > position.x){
        movement.x = 1;
    }else if (target->position.x < position.x){
        movement.x = -1;
    }
    
    if (target->position.y > position.y){
        movement.y = 1;
    }else if (target->position.y < position.y){
        movement.y = -1;
    }
    
    //orientation
    float diffX = position.x - target->position.x;
    float diffY = position.y - target->position.y;
    
    if (std::abs(diffX) > std::abs(diffY)){
        if (target->position.x > position.x){
            animIndices = animRight;
        }else{
            animIndices = animLeft;
        }
    }else{
        if (target->position.y> position.y){
            animIndices = animUp;
        }else{
            animIndices = animDown;
        }
    }
    
}

void Enemy::AIShooter(Entity* target){
    if (target == nullptr) return;

    float diffX = position.x - target->position.x;
    float diffY = position.y - target->position.y;
    
    if (std::abs(diffX) < 5 || std::abs(diffY) < 5){ //if I get close enough, stop moving, start shooting
        std::cout << "pew pew" << std::endl;
        movement = glm::vec3(0);
        Shoot(target);
    }else{
        //movement same as CHASER
        if (target->position.x > position.x){
            movement.x = 1;
        }else if (target->position.x < position.x){
            movement.x = -1;
        }
        
        if (target->position.y > position.y){
            movement.y = 1;
        }else if (target->position.y < position.y){
            movement.y = -1;
        }
        
        //orientation
        if (std::abs(diffX) > std::abs(diffY)){
            if (target->position.x > position.x){
                animIndices = animRight;
            }else{
                animIndices = animLeft;
            }
        }else{
            if (target->position.y> position.y){
                animIndices = animUp;
            }else{
                animIndices = animDown;
            }
        }
    }
}

void Enemy::Update(float deltaTime, Map* map, const std::vector<Entity*> objects, Entity* target)
{
    this->movement = glm::vec3(0);
    switch (entityType){;
        case CHASER:
            AIChaser(target);
            break;
        case SHOOTER:
            AIShooter(target);
            break;
        default:
            return;
    }
    this->movement = glm::normalize(this->movement);
    Entity::Update(deltaTime, map, objects, target);
}
