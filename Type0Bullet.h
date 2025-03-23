#ifndef TYPE0BULLET_H
#define TYPE0BULLET_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

using namespace std;

class Type0Bullet {
    public:

    Type0Bullet (int _x, int _y, double _speedX, double _speedY);
    ~Type0Bullet ();

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