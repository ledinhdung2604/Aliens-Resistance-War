#ifndef METEOR_H
#define METEOR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

using namespace std;

class Meteor {
    public:
    Meteor (int _x, int _y, int _direction);
    ~Meteor ();

    bool loadTexture (SDL_Renderer* gRenderer, string path);
    void free ();
    void update ();
    void render (SDL_Renderer* gRenderer);
    SDL_Rect getRect ();
    bool isOutOfBorder (int SCREEN_WIDTH, int SCREEN_HEIGHT);
    bool isCollided ();
    void collision (bool _collide);

    private:
    SDL_Texture* texture;
    int x, y;
    int w, h;
    int speed;
    int direction;
    bool collide;
};

#endif