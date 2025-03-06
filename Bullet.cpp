#include "Bullet.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace std;

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 506;

Bullet :: Bullet () {
    texture = NULL;
    speed = 10;
    rect = {0, 0 , 20, 20};
}

Bullet :: ~Bullet () {
    free ();
}

void Bullet :: free () {
    if (texture == NULL) {
    SDL_DestroyTexture (texture);
    texture = NULL;}
}

bool Bullet :: loadTexture (SDL_Renderer* gRenderer, std::string path) {
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

void Bullet :: initialize (int x, int y) {
    rect.x = x;
    rect.y = y;
    
}

void Bullet :: update () {
    rect.y -= speed;
}

void Bullet :: render (SDL_Renderer* gRenderer) {
    
        SDL_RenderCopy (gRenderer, texture, NULL, &rect);
    
}

bool Bullet::isOutofBorder()  {
    return rect.y < 0;
}
