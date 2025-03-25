#ifndef TYPE3BULLET_H
#define TYPE3BULLET_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>

using namespace std;

class Type3Bullet {
    public:
    Type3Bullet (int _x, int _y, int spaceshipX, int spaceshipY);
    ~Type3Bullet ();


    bool loadTexture (SDL_Renderer* gRenderer, string path) ;

    void free ();

    void update ();

    void render (SDL_Renderer* gRenderer);

    SDL_Rect getRect ();
    
    bool isOutOfBorder (int SCREEN_WIDTH, int SCREEN_HEIGHT);

    void isCollided (bool _collide);

    bool collision ();

    int getBounceCount ();

    private:
    SDL_Texture* texture;
    SDL_Rect rect;
    bool collide;
    double speedX;
    double speedY;
    int bounceCount;
};

#endif