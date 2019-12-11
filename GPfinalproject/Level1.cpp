#include "Level1.h"
#define LEVEL1_WIDTH 29
#define LEVEL1_HEIGHT 8

unsigned int level1_data[] =
{
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3,
    3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3,
    3, 0, 0, 3, 0, 0, 3, 0, 0, 3, 0, 0, 3, 0, 0, 3, 0, 0, 3, 0, 0, 3, 0, 0, 3, 3, 3, 3, 3,
    3, 0, 0, 3, 0, 0, 3, 0, 0, 3, 0, 0, 3, 0, 0, 3, 0, 0, 3, 0, 0, 3, 0, 0, 3, 3, 3, 3, 3,
    3, 0, 0, 3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 3, 0, 0, 3, 3, 3, 3, 3,
    3, 0, 0, 3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 3, 0, 0, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};

void Level1::Initialize() {
    GLuint mapTextureID = Util::LoadTexture("tile.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 3, 1);
    
    state.player.entityType = PLAYER;
    state.player.isStatic = false;
    state.player.width = 1.0f;
    state.player.position = glm::vec3(1, -6, 0);
    state.player.acceleration = glm::vec3(0, 0, 0);
    state.player.textureID = Util::LoadTexture("idle.png");
    state.player.lives = 3;
    
    state.nextLevel = -1;
}

void Level1::Update(float deltaTime) {
    state.player.Update(deltaTime, NULL, 0, state.map);

    
    if (state.player.position.x > 22){
        state.nextLevel = 2;
    }
}

void Level1::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player.Render(program);
}
