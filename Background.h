#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL2/SDL.h>
#include <string>

class Background {
    public:
    Background ();
    ~Background ();

    void free ();

    bool loadTexture (SDL_Renderer* gRenderer, std::string path) ;

    void render (SDL_Renderer * gRenderer);

    private:
    SDL_Texture * texture;
    int mWidth;
    int mHeight;
};
#endif