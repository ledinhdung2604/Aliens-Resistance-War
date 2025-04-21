#include "Type3Bullet.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>

using namespace std;

Type3Bullet :: Type3Bullet (int _x, int _y, int spaceshipX, int spaceshipY) {

    // làm đạn bắn về hướng player
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
    bounceCount = 0;
}

Type3Bullet :: ~Type3Bullet () {
    free ();
}



bool Type3Bullet :: loadTexture (SDL_Renderer* gRenderer, string path) {
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

void Type3Bullet :: free () {
    if (texture != NULL) {
        SDL_DestroyTexture (texture);
        texture = NULL;
    }
}

void Type3Bullet :: update () {

    rect.x += speedX;
    rect.y += speedY;
}

void Type3Bullet :: render (SDL_Renderer* gRenderer) {
    SDL_RenderCopy (gRenderer, texture, NULL, &rect);
}

SDL_Rect Type3Bullet :: getRect () {
    return rect;
}

bool Type3Bullet :: isOutOfBorder (int SCREEN_WIDTH, int SCREEN_HEIGHT) {
    bool hitBorder = false;
    if (rect.x > SCREEN_WIDTH || rect.x < 0 || rect.y > SCREEN_HEIGHT || rect.y < 0) {
        if (bounceCount < 4)  { // nảy 4 lần ms ra khỏi màn hình
            if (rect.x <= 0 || rect.x >= SCREEN_WIDTH - 20) {
                speedX = -speedX;
            }

            if (rect.y <= 0 || rect.y >= SCREEN_HEIGHT - 20) {
                speedY = -speedY;
            }
            bounceCount++;
            hitBorder = false;
        }

        else {
            hitBorder = true;
        }
    }
    return hitBorder;
}

void Type3Bullet :: isCollided (bool _collide) {
    collide = _collide;
}

bool Type3Bullet :: collision (){
    return collide;
}

int Type3Bullet :: getBounceCount () {
    return bounceCount;
}
