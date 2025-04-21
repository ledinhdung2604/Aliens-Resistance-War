#include "Type0Bullet.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>

using namespace std;

Type0Bullet :: Type0Bullet (int _x, int _y, double _speedX, double _speedY) {
    texture = NULL;
    rect.x = _x;
    rect.y = _y;
    speedX = _speedX;
    speedY = _speedY;
}

Type0Bullet :: ~Type0Bullet () {
    free ();
}

bool Type0Bullet :: loadTexture (SDL_Renderer* gRenderer, string path) {
    free ();
    
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

void Type0Bullet :: free () {
    if (texture != NULL) {
        SDL_DestroyTexture (texture);
        texture = NULL;
    }
}

void Type0Bullet :: update () {
    rect.x += speedX;
    rect.y += speedY;
}

void Type0Bullet :: render (SDL_Renderer* gRenderer) {
    SDL_RenderCopy (gRenderer, texture, NULL, &rect);
}

SDL_Rect Type0Bullet :: getRect () {
    return rect;
}

bool Type0Bullet :: isOutOfBorder (int SCREEN_WIDTH, int SCREEN_HEIGHT) {
    return rect.x > SCREEN_WIDTH || rect.x < 0 || rect.y > SCREEN_HEIGHT;
}

void Type0Bullet :: isCollided (bool _collide) {
    collide = _collide;
}

bool Type0Bullet :: collision (){
    return collide;
}
