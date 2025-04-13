#ifndef BLACKHOLE_H
#define BLACKHOLE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "Player.h"

using namespace std;

class BlackHole {
    public:
    BlackHole (int _x, int _y);
    ~BlackHole ();

    bool loadTexture (SDL_Renderer* gRenderer, string path);
    void free ();
    void render (SDL_Renderer* gRenderer);
    SDL_Rect getRect ();
    bool despawnTime ();

    bool playerInRange (int spaceshipX, int spaceshipY);

    private:
    SDL_Texture* texture;
    SDL_Rect rect;
    Uint32 spawnTime;
    Uint32 activeTime = 5000;
    int gravityRadius = 200;
    double gravityForce = 40.0;
};

#endif