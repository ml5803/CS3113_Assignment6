#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Scene.h"

using namespace std;

GLuint LoadTexture(const char* filePath) {
	int w, h, n;
	unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

	if (image == NULL) {
		cerr << "Unable to load image. Make sure the path is correct\n";
		assert(false);
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_image_free(image);
	return textureID;
}

void DrawText(ShaderProgram* program, GLuint fontTextureID, string text,
	float size, float spacing, glm::vec3 position) {

	float width = 1.0f / 16.0f;
	float height = 1.0f / 16.0f;
	std::vector<float> vertices;
	std::vector<float> texCoords;

	for (int i = 0; i < text.size(); i++) {
		int index = (int)text[i];
		float offset = (size + spacing) * i;
		float u = (float)(index % 16) / 16.0f;
		float v = (float)(index / 16) / 16.0f;
		vertices.insert(vertices.end(), {
			offset + (-0.5f * size), 0.5f * size,
			offset + (-0.5f * size), -0.5f * size,
			offset + (0.5f * size), 0.5f * size,
			offset + (0.5f * size), -0.5f * size,
			offset + (0.5f * size), 0.5f * size,
			offset + (-0.5f * size), -0.5f * size,
			});
		texCoords.insert(texCoords.end(), {
			u, v,
			u, v + height,
			u + width, v,
			u + width, v + height,
			u + width, v,
			u, v + height,
			});
	}
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
	program->SetModelMatrix(modelMatrix);

	glUseProgram(program->programID);

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
	glEnableVertexAttribArray(program->texCoordAttribute);

	glBindTexture(GL_TEXTURE_2D, fontTextureID);
	glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}

void Scene::Initialize(bool isGame) {
	resetGame();
	this->isGame = isGame;
	fontTextureID = LoadTexture("./src/font.png");

	// Initialize map
	GLuint mapTextureID = LoadTexture("./src/tileset.png");
	state.map = new Map(width, height, levelData, mapTextureID, 1.0f, 4, 2);

	if (isGame) {
		//Get initial time
		state.startTime = time(NULL);
        state.lastWaveTime = time(NULL);

		// Initialize player
		state.player = new Player(LoadTexture("./src/player.png"), 1.0f, 1.2f);
		state.player->position = glm::vec3(10, -7.5, 0);

        // Initialize 4 chasers at each corner
        for (size_t i = 0; i < spawnLocations.size(); i++){
            state.enemies.push_back(new Enemy(LoadTexture("./src/enemy.png"), 1.0f, 1.0f));
            state.enemies[i]->position = glm::vec3(spawnLocations[i].first,spawnLocations[i].second,0);
            state.enemies[i]->entityType = Enemy::CHASER;
        }
        
        // Test shooter
        //state.enemies.push_back(new Enemy(LoadTexture("./src/enemy.png"), 1.0f, 1.0f));
        //state.enemies[0]->position = glm::vec3(5,-7.5,0);
        //state.enemies[0]->entityType = Enemy::SHOOTER;
    }
}

int Scene::Update(float deltaTime) {
	if (isGame) {
		// Update player
        state.player->Update(deltaTime, state.map);

        // Spawn new enemies every 5 seconds
        std::cout << state.enemies.size() << std::endl;
        time_t currTime = time(NULL);
        srand(time(NULL));
        if (currTime - state.lastWaveTime > 5){
            // Spawn randomly at spawn points
            for (size_t i = 0; i < spawnLocations.size(); i++){
                int randProb = rand() % 10;
                Enemy* tempEnemy;
                if (randProb > 6){
                    tempEnemy = new Enemy(LoadTexture("./src/enemy.png"), 1.0f, 1.0f);
                    tempEnemy->position = glm::vec3(spawnLocations[i].first,spawnLocations[i].second,0);
                    tempEnemy->entityType = Enemy::CHASER;
                    state.enemies.push_back(tempEnemy);
                }
                std::cout << spawnLocations[i].first << "," << spawnLocations[i].second << std::endl;

            }
            state.lastWaveTime = currTime;
        }

		// Update bullets
		for (size_t i = 0; i < state.bullets.size(); i++) {
			state.bullets[i]->Update(deltaTime, state.map, state.enemies, state.player);
		}
		
		// Update enemies 
        for (Entity* enemy : state.enemies){
			// Losing condition: collide with an enemy
			if (enemy->CheckCollision(state.player)) return -1; 

			enemy->Update(deltaTime, state.map, state.enemies, state.player);
        }
        
		// Winning condition: time is up
		state.timer = 120 - (time(NULL) - state.startTime);
		if (state.timer <= 0) 
			return 1;
		// Losing condition: collide with ENEMY_BULLET
		else if (!state.player->isActive) 
			return -1;
	}

	return 0;
}

void Scene::Render(ShaderProgram *program, const string& menuText) {

	if (isGame)
		DrawText(program, fontTextureID, "Timer: " + to_string(state.timer) + "s", 0.7f, -0.35f, glm::vec3(1.5, -1.5, 0));
	else {
		DrawText(program, fontTextureID, "PLANTS vs ZOMBIES?", 1.0f, -0.4f, glm::vec3(4.5, -4, 0));
		DrawText(program, fontTextureID, menuText, 1.0f, -0.4f, glm::vec3(4, -5, 0));
	}

	state.map->Render(program);
	
	if (isGame) {
		state.player->Render(program);
		// Render AI enemies
		for (Entity* enemy : state.enemies) {
			enemy->Render(program);
		}
        // Render Player Bullets
        for (Bullet* bullet : state.bullets){
            bullet->Render(program);
        }
	}
}

void Scene::resetGame() {
	delete state.map;
	delete state.player;
	
	for (size_t i = 0; i < state.bullets.size(); ++i) {
		delete state.bullets[i];
	}
	state.bullets.clear();

	for (size_t i = 0; i < state.enemies.size(); ++i) {
		delete state.enemies[i];
	}
	state.enemies.clear();
}

Scene::Scene(unsigned int* l) : levelData(l) {}
Scene::~Scene() { resetGame(); }
bool Scene::IsGame() const { return isGame; }

void Scene::makePlayerBullet(){
    Bullet* tempBullet = new Bullet(LoadTexture("./src/bullet.png"));
    tempBullet->position = state.player->position;
    tempBullet->movement = -(state.player->movement);
	tempBullet->entityType = Entity::PLAYER_BULLET;
    state.bullets.push_back(tempBullet);
}
