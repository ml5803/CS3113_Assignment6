#pragma once
#include <ctime>
#include <string>
#include "Player.h"

struct GameState {
	bool isGame;
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

	Scene(int w, int h, unsigned int* l);
	~Scene();
	void Initialize(bool isGame = false);
	int Update(float deltaTime);
	void Render(ShaderProgram* program, const std::string& gameState);
private:
	int width, height;
	unsigned int* levelData;
	GLuint fontTextureID;
};