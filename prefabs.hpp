#include <SDL2/SDL.h>
#include <cstring>
#include <cmath>


class Object{
    public:
        double position[2] = {100, 75};
        short mirrorState[2] = {0, 0};
        bool sprite[4][4] = {};
        bool spriteSheet[4][4][4] = {};
        bool animation = 0;
        int animationIndex = 0;


    void setPosition(int x, int y){
        position[0] = x;
        position[1] = y;
    }

    void instatiateObect(bool spriteArray[][4], bool spriteSheetArray[][4][4]){
        if(spriteArray != NULL){
            std::memcpy(sprite, spriteArray, sizeof(&spriteArray)*2);
        }
        if(spriteSheetArray != NULL){
            std::memcpy(spriteSheet, spriteSheetArray, sizeof(&spriteSheetArray)*8);
        }
    }
    
    void renderImg(SDL_Renderer* rend, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                if (sprite[i][j]){
                    SDL_SetRenderDrawColor(rend, r, g, b, a);
                    int z = 3 - j;
                    if(mirrorState[0] < 0){
                        SDL_RenderDrawPoint(rend, int(position[0] + z), int(position[1] + i));
                    }else{
                        SDL_RenderDrawPoint(rend, int(position[0] + j), int(position[1] + i));
                    }
                }
            }
        }
    }

    void renderAnim(SDL_Renderer* rende, uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool spriteSheet[][4][4]){
        if(animationIndex % 100 == 0){
            std::memcpy(sprite, spriteSheet[animationIndex/100], sizeof(&spriteSheet)*2);
            //std::cout << animationIndex/100 << std::endl;
        }
        animationIndex += 1;
        if(animationIndex >= 400){
            animationIndex = 0;
        }
        renderImg(rende, r, g, b, a);
    }
};