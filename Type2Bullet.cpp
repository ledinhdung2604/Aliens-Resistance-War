#include "Type2Bullet.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>

using namespace std;

Type2Bullet :: Type2Bullet (int _x, int _y, int& spaceshipX, int &spaceshipY) {
    rect.x = _x;
    rect.y = _y;
    texture = NULL;
    double disX = spaceshipX - rect.x;
    double disY = spaceshipY - rect.y;
    double distance = sqrt (disX * disX + disY * disY);
            
    if (distance != 0) {
        speedX = (disX / distance) * 15;
        speedY = (disY / distance) * 15;
    }
    
}

Type2Bullet :: ~Type2Bullet () {
    free ();
}



bool Type2Bullet :: loadTexture (SDL_Renderer* gRenderer, string path) {
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

void Type2Bullet :: free () {
    SDL_DestroyTexture (texture);
    texture = NULL;
}

void Type2Bullet :: update () {

    rect.x += speedX;
    rect.y += speedY;
}

void Type2Bullet :: render (SDL_Renderer* gRenderer) {
    SDL_RenderCopy (gRenderer, texture, NULL, &rect);
}

SDL_Rect Type2Bullet :: getRect () {
    return rect;
}

bool Type2Bullet :: isOutOfBorder (int SCREEN_WIDTH, int SCREEN_HEIGHT) {
    return rect.x > SCREEN_WIDTH || rect.x < 0 || rect.y > SCREEN_HEIGHT;
}

void Type2Bullet :: isCollided (bool _collide) {
    collide = _collide;
}

bool Type2Bullet :: collision (){
    return collide;
}
