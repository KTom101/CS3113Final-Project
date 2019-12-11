#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
//#include <SDL_image.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

#include "Entity.h"
#include "Map.h"
#include "Util.h"
#include "Scene.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level0.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Scene *currentScene;
Scene *sceneList[4];
Mix_Music *music;
Mix_Chunk *selection;
Mix_Chunk *walk;

void SwitchToScene(Scene *scene) {
    currentScene = scene;
    currentScene->Initialize();
}


GLuint fontTextureID;


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("STEPPER!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    //audio
    int Mix_OpenAudio(int frequency, Uint16 format, int channels,
                      int chunksize);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("music.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 3);
    
    
    selection = Mix_LoadWAV("click.wav");
    Mix_VolumeChunk(selection, MIX_MAX_VOLUME / 4);
    
    walk = Mix_LoadWAV("dirtwalk2.wav");
    Mix_VolumeChunk(walk, MIX_MAX_VOLUME / 4);

    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    fontTextureID = Util::LoadTexture("font.png");
    
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    glClearColor(0.6f, 0.5f, 0.3f, 1.2f);
    
    sceneList[0] = new Level0();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    sceneList[3] = new Level3();
    SwitchToScene(sceneList[0]);
}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        //currentScene->state.player.Jump();
                        break;
                        
                }
                break;
        }
    }
    
    currentScene->state.player.velocity.x = 0;
    currentScene->state.player.velocity.y = 0;
    
    // Check for pressed/held keys below
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_LEFT])
    {
        Mix_PlayChannel(-1, walk, 0);
        currentScene->state.player.velocity.x = -3.0f;
    }
    else if  (keys[SDL_SCANCODE_RIGHT])
    {
        Mix_PlayChannel(-1, walk, 0);
        currentScene->state.player.velocity.x = 3.0f;
    }
    else if  (keys[SDL_SCANCODE_UP])
    {
        Mix_PlayChannel(-1, walk, 0);
        currentScene->state.player.velocity.y = 3.0f;
    }
    else if  (keys[SDL_SCANCODE_DOWN])
    {
        Mix_PlayChannel(-1, walk, 0);
        currentScene->state.player.velocity.y = -3.0f;
    }
    else if (keys[SDL_SCANCODE_RETURN]){
        if(currentScene == sceneList[0]){
            Mix_PlayChannel(-1, selection, 0);
            sceneList[0]->state.nextLevel = 1;
        }
    }
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    
    while (deltaTime >= FIXED_TIMESTEP) {
        
        currentScene->Update(FIXED_TIMESTEP);
        
        deltaTime -= FIXED_TIMESTEP;
    }
    
    accumulator = deltaTime;
    
    viewMatrix = glm::mat4(1.0f);
    
    if (currentScene->state.player.position.x > 5) {
        viewMatrix = glm::translate(viewMatrix,
                                    glm::vec3(-(currentScene->state.player.position.x), 3.75, 0));
    } else {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
    }
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    program.SetViewMatrix(viewMatrix);
    
    currentScene->Render(&program);
    
    
    if (currentScene == sceneList[0]){
        
        Util::DrawText(&program, fontTextureID, "STEPPER", 1.0f, -0.5f, glm::vec3(3, -1, 0));
        Util::DrawText(&program, fontTextureID, "press ENTER to start", 0.9f, -0.5f, glm::vec3(3, -2.0, 0));
    }
    
    if (currentScene->state.player.win){
        Mix_HaltMusic();
        Util::DrawText(&program, fontTextureID, "YOU WON!!", 1.0f, -0.5f, glm::vec3(3, -1, 0));
        
    }
    
    
    if (currentScene->state.player.lives == 0){
        Mix_HaltMusic();
        Util::DrawText(&program, fontTextureID, "YOU LOST!!", 1.0f, -0.5f, glm::vec3(2, -1, 0));
        //return;
    }
    
    
    if (currentScene != sceneList[0]){
        std::string livesLeft = std::to_string(currentScene->state.player.lives);
        Util::DrawText(&program, fontTextureID, livesLeft , 0.7f, -0.5f, glm::vec3(9, -1, 0));
    }
    
    if (currentScene == sceneList[1]){
        
        Util::DrawText(&program, fontTextureID, "Make it to the end", 0.75f, -0.5f, glm::vec3(1, -1.9, 0));
        Util::DrawText(&program, fontTextureID, "to go to the next level", 0.75f, -0.5f, glm::vec3(1,
        -2.3, 0));
        
    }
    
    if (currentScene == sceneList[2]){
        
        Util::DrawText(&program, fontTextureID, "Keep Going, You Can Do It", 0.75f, -0.5f, glm::vec3(1, -1.9, 0));
        
    }
    
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    Mix_FreeMusic(music);
    Mix_FreeChunk(selection);
    Mix_FreeChunk(walk);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        
        if (currentScene->state.nextLevel >= 0) SwitchToScene(sceneList[currentScene->state.nextLevel]);
        
        ProcessInput();
        Update();
        Render();
    }
    
    Shutdown();
    return 0;
}
