#ifndef TYPE2BULLET_H
#define TYPE2BULLET_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

using namespace std;

class Type2Bullet {
    public:

    Type2Bullet (int _x, int _y, int& spaceshipX, int& spaceshipY);
    ~Type2Bullet ();

    void getSpaceship (int _spaceshipX, int _spaceshipY);

    bool loadTexture (SDL_Renderer* gRenderer, string path) ;

    void free ();

    void update ();

    void render (SDL_Renderer* gRenderer);

    SDL_Rect getRect ();
    
    bool isOutOfBorder (int SCREEN_WIDTH, int SCREEN_HEIGHT);

    void isCollided (bool _collide);

    bool collision ();



    private:
    SDL_Texture* texture;
    SDL_Rect rect;
    bool collide;
    double speedX;
    double speedY;
    
};

#endif