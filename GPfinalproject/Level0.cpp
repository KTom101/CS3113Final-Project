#include "Level0.h"


#define LEVEL0_WIDTH 18
#define LEVEL0_HEIGHT 8

unsigned int level0_data[] =
{
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
};

void Level0::Initialize() {
    GLuint mapTextureID = Util::LoadTexture("tile.png");
    state.map = new Map(LEVEL0_WIDTH, LEVEL0_HEIGHT, level0_data, mapTextureID, 1.0f, 3, 1);
    state.player.entityType = PLAYER;
    state.player.isStatic = false;
    state.player.width = 1.0f;
    state.player.position = glm::vec3(7, -6, 0);
    state.player.acceleration = glm::vec3(0, 0, 0);
    state.player.textureID = Util::LoadTexture("idle.png");
    state.player.lives = 3;

    
    state.nextLevel = -1;
}

void Level0::Update(float deltaTime) {
    state.player.Update(deltaTime, NULL, 0, state.map);
}

void Level0::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player.Render(program);
}
