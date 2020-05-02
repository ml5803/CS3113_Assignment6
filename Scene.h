#pragma once
#include <ctime>
#include <string>
#include "Player.h"

struct GameState {
	time_t startTime;
	time_t timer = -1;
	
	Map* map;
	Player* player;
	std::vector<Entity*> enemies;
};

class Scene {
public:
	GameState state;
	glm::mat4 viewMatrix;

	Scene(unsigned int* l);
	~Scene();
	bool IsGame() const;
	void Initialize(bool isGame = false);
	int Update(float deltaTime);
	void Render(ShaderProgram* program, const std::string& gameState);
private:
	unsigned int* levelData;
	int width = 21, height = 16;

	bool isGame;
	GLuint fontTextureID;
};