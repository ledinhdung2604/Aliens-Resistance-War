#include "Type1Bullet.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>

using namespace std;

Type1Bullet :: Type1Bullet (int _x, int _y, double _speed) {
    texture = NULL;
    rect.x = _x;
    rect.y = _y;
    speed = _speed;
}

Type1Bullet :: ~Type1Bullet () {
    free ();
}

bool Type1Bullet :: loadTexture (SDL_Renderer* gRenderer, string path) {
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

void Type1Bullet :: free () {
    SDL_DestroyTexture (texture);
    texture = NULL;
}

void Type1Bullet :: update () {
    rect.y += speed;
}

void Type1Bullet :: render (SDL_Renderer* gRenderer) {
    SDL_RenderCopy (gRenderer, texture, NULL, &rect);
}

SDL_Rect Type1Bullet :: getRect () {
    return rect;
}

bool Type1Bullet :: isOutOfBorder (int SCREEN_WIDTH, int SCREEN_HEIGHT) {
    return rect.y > SCREEN_HEIGHT;
}

void Type1Bullet :: isCollided (bool _collide) {
    collide = _collide;
}

bool Type1Bullet :: collision (){
    return collide;
}