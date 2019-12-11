#include "Level3.h"


#define LEVEL3_WIDTH 15
#define LEVEL3_HEIGHT 8


unsigned int level3_data[] =
{
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void Level3::Initialize() {
    GLuint mapTextureID = Util::LoadTexture("tile.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 3, 1);
    
    state.player.entityType = PLAYER;
    state.player.isStatic = false;
    state.player.width = 1.0f;
    state.player.position = glm::vec3(2, -5, 0);
    state.player.acceleration = glm::vec3(0, 0, 0);
    state.player.textureID = Util::LoadTexture("idle.png");
    state.player.lives = 3;
    
    state.nextLevel = -1;
    
    state.enemy.entityType = ENEMY;
    state.enemy.isStatic = false;
    state.enemy.width = 1.0f;
    state.enemy.position = glm::vec3(8, -3, 0);
    state.enemy.velocity = glm::vec3(-1,0,0);
    state.enemy.acceleration = glm::vec3(0, 0, 0);
    state.enemy.textureID = Util::LoadTexture("evilred.png");
}

void Level3::Update(float deltaTime) {
    state.player.Update(deltaTime, &state.enemy, 1, state.map);
    
    if (state.player.lives >= 0){
        state.player.death = true;
    }
    
    if (state.player.position.x > 8){
         state.player.win = true;
    }
    if (state.enemy.death && state.player.death == false){
        state.player.win = true;
    }
    if (state.enemy.position.x == 9.0f) state.enemy.velocity.x = -1.0f;
    else if (state.enemy.position.x == 1.0f) state.enemy.velocity.x = 1.0f;
    state.enemy.Update(deltaTime, &state.player, 1, state.map);


    

}

void Level3::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player.Render(program);
    if (state.enemy.death != true) state.enemy.Render(program);
}
