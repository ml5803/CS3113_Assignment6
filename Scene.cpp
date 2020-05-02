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
	this->isGame = isGame;
	fontTextureID = LoadTexture("./src/font.png");

	// Initialize map
	GLuint mapTextureID = LoadTexture("./src/tileset.png");
	state.map = new Map(width, height, levelData, mapTextureID, 1.0f, 4, 2);

	if (isGame) {
		//Get initial time
		state.startTime = time(NULL);

		// Initialize player
		state.player = new Player(LoadTexture("./src/player.png"), 1.0f, 1.2f);
		state.player->position = glm::vec3(10, -7.5, 0);

		// To be added =========================================================
		// Initialize enemies
		//Entity* enemy = new Entity(LoadTexture("./src/enemy.png"), 1.5f, 2.5f);
		//state.enemies.push_back(enemy);
	}
}

int Scene::Update(float deltaTime) {
	if (isGame) {
		// Update player
		state.player->Update(deltaTime, state.map);

		// To be added =========================================================
		// Update enemies 

		// Winning condition: time is up
		state.timer = 120 - (time(NULL) - state.startTime);
		if (state.timer <= 0) 
			return 1;
		// Losing condition 
		else if (false) { 
			// To be added =====================================================
			return -1;
		}
	}
	return 0;
}

void Scene::Render(ShaderProgram *program, const string& gameState) {

	if (isGame)
		DrawText(program, fontTextureID, "Timer: " + to_string(state.timer) + "s", 0.7f, -0.35f, glm::vec3(1.5, -1.5, 0));
	else
		DrawText(program, fontTextureID, gameState, 1.0f, -0.4f, glm::vec3(4, -4, 0));
	
	state.map->Render(program);
	
	if (isGame) {
		state.player->Render(program);
		// To be added =========================================================
		// Render AI enemies
		for (Entity* enemy : state.enemies) {
			enemy->Render(program);
		}
	}
}

Scene::Scene(unsigned int* l) : levelData(l) {}

Scene::~Scene() {
	delete state.player;
	delete state.map;
	for (size_t i = 0; i < state.enemies.size(); ++i) {
		delete state.enemies[i];
	}
}

bool Scene::IsGame() const { return isGame; }
