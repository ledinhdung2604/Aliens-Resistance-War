#include "Background.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

using namespace std;

// Constructor
Background :: Background () {
    texture = NULL;
    mWidth = 0;
    mHeight = 0;
}

Background :: ~Background () {
    free ();
}

void Background :: free () {
    SDL_DestroyTexture (texture);
    texture = NULL;
    mWidth = 0;
    mHeight = 0;
}

bool Background :: loadTexture (SDL_Renderer* gRenderer, std::string path) {
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
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }
    texture = newTexture;
    return texture != NULL;
}

void Background :: render (SDL_Renderer* gRenderer) {
    // Vẽ texture lên renderer. 
    SDL_Rect destRect = {0, 0, mWidth, mHeight};
    SDL_RenderCopy(gRenderer, texture, NULL, NULL);
}
