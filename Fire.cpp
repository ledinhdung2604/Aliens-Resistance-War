#include "Fire.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

using namespace std;

Fire :: Fire () {
    position = {0, 0, 15, 24};
    currentFrame = 0;
    prevTime = SDL_GetTicks ();
    delay = 100;
}

Fire :: ~Fire () {
    free ();
}

bool Fire :: loadTexture (SDL_Renderer* gRenderer, vector<string>& Paths) {
    for (auto& path : Paths) {
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
            textures.push_back(newTexture);
        }
        SDL_FreeSurface(loadedSurface);
    }
    }
    return !textures.empty();
}

void Fire :: free () {
    for (auto &texture : textures) {
        SDL_DestroyTexture (texture);
        texture = NULL;
    }
}

void Fire :: update () {
    Uint32 curTime = SDL_GetTicks ();
    if (curTime -prevTime >= delay) { // kiem tra xem khoang thoi tre da du chua
        // Cap nhat khung hinh
        currentFrame = (currentFrame + 1 ) % (int)textures.size(); // Khi den thoi gian tre thi tang khung hinh va luon dam bao no < hon size
        prevTime = curTime;
    }
}

void Fire :: render (SDL_Renderer* gRenderer, int x, int y) {
    if (textures.empty()) {
        return;
    }
    position.x = x;
    position.y = y;
    SDL_RenderCopy(gRenderer, textures[currentFrame], NULL, &position);
}