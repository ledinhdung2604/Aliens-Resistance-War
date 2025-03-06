#include "Player.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

using namespace std;




// Constructor
Player :: Player () {
    // Khởi tạo các giá trị mặc định
    position = {SCREEN_WIDTH/2 - 20, SCREEN_HEIGHT - 40, 40, 40};
    texture = NULL;
    speed = 10;
    health = 100;
    alive = true;
}

Player :: ~Player () {
    free ();
}

bool Player :: loadTexture (SDL_Renderer* gRenderer,  std::string path) {

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
            position.w = loadedSurface->w;
            position.h = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }
    texture = newTexture;
    return texture != NULL;
}

void Player :: free () {
    if (texture == NULL) {
        SDL_DestroyTexture (texture);
        position = {0, 0, 0, 0};
        texture = NULL;
        speed = 50;
        health = 100;
        alive = true;
    }
}

void Player :: handleEvent (SDL_Event &e) {
        switch (e.key.keysym.sym) {
            case SDLK_w:
            case SDLK_UP:
            position.y -= speed;
            break;

            case SDLK_s:
            case SDLK_DOWN:
            position.y += speed;
            break;

            case SDLK_a:
            case SDLK_LEFT:
            position.x -= speed;
            break;

            case SDLK_d:
            case SDLK_RIGHT:
            position.x += speed;
            break;
        }
    
}

void Player :: update () {
    if (position.x > SCREEN_WIDTH - 40) {
        position.x = SCREEN_WIDTH - 40;
    }

    else if (position.x < 0) {
        position.x = 0;
    }

    if (position.y > SCREEN_HEIGHT - 40) {
        position.y = SCREEN_HEIGHT - 40;
    }

    else if (position.y < 0) {
        position.y = 0;
    }
}

void Player :: render (SDL_Renderer* gRenderer) {
    // Vẽ texture lên renderer tại vị trí và kích thước được xác định bởi position.
    SDL_RenderCopy(gRenderer, texture, NULL, &position);
}

SDL_Rect Player :: getPosition () {
    return position;
}

int Player :: getX () {
    return (double)position.x;
}

int Player :: getY () {
    return (double)position.y;
}