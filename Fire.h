#ifndef FIRE_H
#define FIRE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>

using namespace std;

class Fire {
    public:
    Fire ();
    ~Fire ();

    bool loadTexture (SDL_Renderer* gRenderer, vector<string>& paths) ;

    void free ();

    void update ();

    void render (SDL_Renderer* gRenderer, int x, int y);

    private:
    vector<SDL_Texture*> textures;
    SDL_Rect position;
    int currentFrame; // Frame hien tai
    Uint32 prevTime; // Thoi gian cua frame truoc
    Uint32 delay; // khoang tre giua cac lan phong lua
};

#endif