#include "Meteor.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>
#include <ctime>
#include <algorithm>

using namespace std;

Meteor :: Meteor (int _x, int _y, int _direction) {
    x = _x;
    y = _y;
    direction = _direction;
    collide = false;
    speed = 25;
    w = 40;
    h = 40;
}

Meteor :: ~Meteor () {
    free();
}

bool Meteor :: loadTexture (SDL_Renderer* gRenderer, string path) {
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
            w = loadedSurface->w;
            h = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }
    texture = newTexture;
    return texture != NULL;
}

void Meteor :: free () {
    if (texture != NULL) {
        SDL_DestroyTexture (texture);
        texture = NULL;
    }
}

void Meteor :: update () {
    switch (direction) {
        case 0: // top to bottom
            y += speed;

        case 1: // left to right
            x += speed;

        case 2: // bottom to top
            y -= speed;

        case 3: // right to left
            x -= speed;
    }
}

void Meteor :: render (SDL_Renderer* gRenderer) {
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderCopy(gRenderer, texture, nullptr, &rect);
}

SDL_Rect Meteor :: getRect () {
    return {x, y, w, h};
}

bool Meteor :: isOutOfBorder (int SCREEN_WIDTH, int SCREEN_HEIGHT) {
    switch (direction) {
        case 0:
            return y > SCREEN_HEIGHT;
        case 1:
            return x > SCREEN_WIDTH;
        case 2:
            return y < 0;
        case 3:
            return x < 0;
        default:
            return false;
    }
}

bool Meteor :: isCollided () {
    return collide;
}

void Meteor :: collision (bool _collide) {
    collide = _collide;
}