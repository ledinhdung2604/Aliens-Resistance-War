#include "BlackHole.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>
#include <ctime>
#include <algorithm>

using namespace std;

BlackHole :: BlackHole (int _x, int _y) {
    rect.x = _x;
    rect.y = _y;
    rect.w = 100;
    rect.h = 100;
    spawnTime = SDL_GetTicks ();
} 

BlackHole :: ~BlackHole () {
    free();
}

bool BlackHole :: loadTexture (SDL_Renderer* gRenderer, string path) {
    free();

    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load (path.c_str());

    if (loadedSurface == NULL) {
        cout << "Unable to load image" << path.c_str() << IMG_GetError();
    }

    else {
        // Thiết lập một màu trong suốt (color key) cho hình ảnh.
        SDL_SetColorKey (loadedSurface, SDL_TRUE, SDL_MapRGB (loadedSurface->format, 0, 0xFF, 0xFF));

        // Tạo texture từ surface đã tải.
        newTexture = SDL_CreateTextureFromSurface (gRenderer, loadedSurface);

        if (newTexture == NULL) {
            cout << "Unable to create texture: " << path.c_str() << SDL_GetError();
        }

        else {
            rect.w = loadedSurface->w;
            rect.h = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }
    texture = newTexture;
    return texture != NULL;
}

void BlackHole :: free () {
    if (texture != NULL) {
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
}

void BlackHole :: render (SDL_Renderer* gRenderer) {
    SDL_RenderCopy(gRenderer, texture, NULL, &rect);
}

SDL_Rect BlackHole :: getRect () {
    return rect;
}

bool BlackHole :: despawnTime () {
    return SDL_GetTicks () - spawnTime > activeTime;
}


bool BlackHole :: playerInRange (int spaceshipX, int spaceshipY) {
    // kiểm tra player có trong phạm vi hoạt động của hole không
    int disX = rect.x + rect.w/2 - spaceshipX;
    int disY = rect.y + rect.h/2 - spaceshipY;
    return (disX * disX + disY * disY) < (gravityRadius * gravityRadius);
}