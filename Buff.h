#ifndef BUFF_H
#define BUFF_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

using namespace std;

enum buffType {
    HEAL,
    SHIELD,
    DAMAGE_BOOST,
    ATTACK_SPEED,
    UP_MAX_HP,
    SPEED_BOOST,
    REFLECT,
    FREEZE
};

class Buff {
    public:
    Buff (int _x, int _y);
    ~Buff ();

    bool loadTexture (SDL_Renderer* gRenderer, string path );
    void free ();
    void update ();
    void render (SDL_Renderer* gRenderer);
    SDL_Rect getRect ();
    buffType getType ();
    bool isOutOfScreen (int SCREEN_HEIGHT);

    private:
    SDL_Texture* texture;
    int x, y;
    buffType type;
    double speed;
};

#endif