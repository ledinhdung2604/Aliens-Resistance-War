#include "Buff.h"
#include <iostream>
#include <random>

Buff :: Buff (int _x, int _y) {
    x = _x;
    y = _y;
    texture = NULL;
    speed = 3.0;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution <int> dist (0, 7);
    type = static_cast<buffType>(dist(gen)); // tạo loại buff ngẫu nhiên
}

Buff :: ~Buff () {
    free ();
}

bool Buff :: loadTexture (SDL_Renderer* gRenderer, string path) {
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

        SDL_FreeSurface(loadedSurface);
    }
    texture = newTexture;
    return texture != NULL;
}

void Buff :: free () {
    if (texture != NULL) {
        SDL_DestroyTexture (texture);
        texture = NULL;
    }
}

void Buff :: update () {
    y += speed; // buff rơi xuống nhanh dần
    speed += 0.2;
}

void Buff :: render (SDL_Renderer* gRenderer) {
    if (!texture) {
        return;
    }

    SDL_Rect rect = {x, y, 30, 30};
    SDL_RenderCopy (gRenderer, texture, NULL, &rect);
}

SDL_Rect Buff :: getRect () {
    return {x, y, 30, 30 };
}

buffType Buff :: getType () {
    return type;
}

bool Buff :: isOutOfScreen (int SCREEN_HEIGHT) {
    return y > SCREEN_HEIGHT ;
}
