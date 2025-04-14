#include "Menu.h"
#include <iostream>

using namespace std;

Menu :: Menu () {
    backgroundTexture = NULL;
    buttonTexture = NULL;
    playRect = {(900 * 2 / 3) - 50, 506/3, 200, 70};
    inMenu = true;
}

Menu :: ~Menu () {
    free();
}

bool Menu :: loadTexture (SDL_Renderer* gRenderer, string backPath, string playPath) {
    bool success = true;

    SDL_Surface* surface = IMG_Load(backPath.c_str());
    if (surface == NULL) {
        cout << "Failed to load menu background! SDL_Error: " << IMG_GetError() << endl;
        success = false;
    } else {
        backgroundTexture = SDL_CreateTextureFromSurface(gRenderer, surface);
        SDL_FreeSurface(surface);
        if (backgroundTexture == NULL) {
            cout << "Failed to create menu texture! SDL_Error: " << SDL_GetError() << endl;
            success = false;
        }
    }
    
    // Tải nút Play
    surface = IMG_Load(playPath.c_str());
    if (surface == NULL) {
        cout << "Failed to load play button! SDL_Error: " << IMG_GetError() << endl;
        success = false;
    } else {
        buttonTexture = SDL_CreateTextureFromSurface(gRenderer, surface);
        SDL_FreeSurface(surface);
        if (buttonTexture == NULL) {
            cout << "Failed to create play button texture! SDL_Error: " << SDL_GetError() << endl;
            success = false;
        }
    }

    return success;
}

void Menu :: free () {
    if (backgroundTexture != NULL) {
        SDL_DestroyTexture (backgroundTexture);
        backgroundTexture = NULL;
    }

    if (buttonTexture != NULL) {
        SDL_DestroyTexture (buttonTexture);
        buttonTexture = NULL;
    }
}

void Menu :: render (SDL_Renderer* gRenderer) {
    SDL_RenderCopy (gRenderer, backgroundTexture, NULL, NULL);
    SDL_RenderCopy (gRenderer, buttonTexture, NULL, &playRect);
}

bool Menu :: handleEvent (SDL_Event &e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_GetMouseState (&x, &y);
        if (checkButtonClick (x, y)) {
            inMenu = false;
            return true; // da click
        }
    }
    return false;
}

bool Menu :: checkButtonClick (int x, int y) {
    return (x >= playRect.x && x <= playRect.x + playRect.w && y >= playRect.y && y <= playRect.y + playRect.h);
}

bool Menu :: menuOrGame () {
    return inMenu;
}

void Menu :: gameStart () {
    inMenu = false;
}