#include "EnemyBullet.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace std;

EnemyBullet :: EnemyBullet () {
    texture = NULL;
    rect = {0, 0, 20, 20};
    speed = 10;
    collide = false;
}

EnemyBullet :: ~EnemyBullet () {
    free ();
}

bool EnemyBullet :: loadTexture (SDL_Renderer* gRenderer, string path) {
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

void EnemyBullet :: free () {
    if (texture != NULL) {
        SDL_DestroyTexture (texture);
        texture = NULL;}
}

void EnemyBullet :: initialize (int x, int y)  {
    rect.x = x;
    rect.y = y;
    rect.w = 20;
    rect.h = 20;
}

void EnemyBullet :: update () {
    rect.y += speed; // bắn xuống 
}

void EnemyBullet :: render (SDL_Renderer* gRenderer) {
    
    SDL_RenderCopy (gRenderer, texture, NULL, &rect);
}

SDL_Rect EnemyBullet :: getRect() {
    return rect;
}

bool EnemyBullet :: isOutofBorder (int SCREEN_HEIGHT) {
    return rect.y > SCREEN_HEIGHT;
}

void EnemyBullet :: isCollided (bool _collide) {
    collide = _collide;
}

bool EnemyBullet :: collision () {
    return collide;
}
