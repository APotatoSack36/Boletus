#include <SDL2/SDL.h>
#include <iostream>
#include <ctime>
#include <cstring>
#include <chrono>
#include <cstdint>

#include <prefabs.hpp>
#include <sprites.hpp>

#undef main

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_SCALAR 4

SDL_Window* window;
SDL_Renderer* renderer;

Object player;
Object slime;
Object hat;
Object grass[40] = {};
int fallTime;
signed short momentum[4] = {0, 0};

bool grounded;
double iVel = 0.0f;
double iPos = 0.0f;
bool jump;
bool lastJump;
bool jumpState;
bool gameState = true;
short playerDirection = 0;

uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void initEnvironment(){
    srand(time(0));
    for(int i = 0; i < 40; i++){
        //std::cout << rand() % 800 << std::endl;
        grass[i].instatiateObect(grassSprite, NULL);
        grass[i].position[0] = rand() % int(SCREEN_WIDTH/SCREEN_SCALAR);
        grass[i].position[1] = rand() % int(SCREEN_HEIGHT/SCREEN_SCALAR);
    }
}

void drawEnvironment(){
    for(int i = 0; i < 40; i++){
        grass[i].renderImg(renderer, 38, 139, 7, 255);
    }
}


int main(){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer);
    SDL_RenderSetScale(renderer, SCREEN_SCALAR, SCREEN_SCALAR);

    uint64_t progStartTime = timeSinceEpochMillisec();
    
    //Instantiate Objects
    player.instatiateObect(playerSpriteSheet[0], playerSpriteSheet);
    hat.instatiateObect(wizardHatSpriteSheet[0], wizardHatSpriteSheet);
    slime.instatiateObect(slimeSprite, NULL);
    //initEnvironment();

    while(gameState){
        //Reset Entire Screen
        //std::cout << timeSinceEpochMillisec() - progStartTime << std::endl;

        lastJump = jump;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            //Handle Quit Event
            if(event.type == SDL_QUIT){
                gameState = !gameState;
            }

            //Handle Key Events
            if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_w){momentum[0] = -1;}
                if(event.key.keysym.sym == SDLK_s){momentum[1] = 1;}
                if(event.key.keysym.sym == SDLK_a){
                    momentum[2] = -1;
                    playerDirection = -1;
                }
                if(event.key.keysym.sym == SDLK_d){
                    momentum[3] = 1;
                    playerDirection = 1;
                }
                if(event.key.keysym.sym == SDLK_SPACE){jump = 1;}
            }
            if(event.type == SDL_KEYUP){
                if(event.key.keysym.sym == SDLK_w){momentum[0] = 0;}
                if(event.key.keysym.sym == SDLK_s){momentum[1] = 0;}
                if(event.key.keysym.sym == SDLK_a){momentum[2] = 0;}
                if(event.key.keysym.sym == SDLK_d){momentum[3] = 0;}
                if(event.key.keysym.sym == SDLK_SPACE){jump = 0;}
            }
        }

        if(grounded && jump){
            iVel = .15;
            iPos = player.position[1];
            grounded = false;
            progStartTime = timeSinceEpochMillisec();
        }

        if(grounded != true){
            fallTime = timeSinceEpochMillisec() - progStartTime;
        }

        //Move Player
        player.position[1] = (.0002 * fallTime * fallTime) - (iVel*fallTime) + (iPos);
        player.position[0] += (momentum[2] + momentum[3])* .02;
        hat.position[0] = player.position[0];
        hat.position[1] = player.position[1] - 3;


        if(player.position[1] >= (SCREEN_HEIGHT / 4) - 4 && jump != true){
            progStartTime = timeSinceEpochMillisec();
            grounded = true;

            fallTime = 0;
            iVel = 0;
            iPos = (SCREEN_HEIGHT/4) - 4;
        }else{
            grounded = false;
        }

        if(lastJump != jump){
            jump = false;
        }

        slime.renderImg(renderer, 17, 255, 1, 127);
        if((momentum[0] + momentum[1]) != 0 || (momentum[2] + momentum[3]) != 0){
            player.renderAnim(renderer, 255, 255, 255, 255, playerSpriteSheet);
            std::memcpy(hat.sprite, wizardHatSpriteSheet[1], sizeof(wizardHatSpriteSheet[1])*2);
        }else{
            player.renderImg(renderer, 255, 255, 255, 255);
            std::memcpy(hat.sprite, wizardHatSpriteSheet[0], sizeof(wizardHatSpriteSheet[0])*2);
        }
        hat.renderImg(renderer, 70, 127, 255, 255);
        player.mirrorState[0] = playerDirection;
        hat.mirrorState[0] = playerDirection;
        
        //Render Changes
        SDL_RenderPresent(renderer);
    }

    //Handle the end.
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}