#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include "EnemyBullet.h"
#include "Buff.h"
using namespace std;

class Enemy {
    public:
    
    Enemy (int _x, int _y, int _speed, int _movingtype);
    ~Enemy ();

    bool loadTexture (SDL_Renderer* gRenderer, string path) ;

    void free ();

    void update ( int SCREEN_WIDTH, int SCREEN_HEIGHT, vector <EnemyBullet> &eStorage);

    void render (SDL_Renderer* gRenderer);

    SDL_Rect getRect () ;

    bool isAlive ();

    void takedamage (int damage, double damageBoost);

    void buffDrop (vector <Buff*> &buffs);

    bool FrozenOrNot ();

    void setFrozen (bool _isFrozen);

    private:
    SDL_Texture* texture;
    int x;
    int y;
    int speed;
    int movingtype; // chon cach di chuyen cho quai (0: di thang, 1: di hinh sin, 2: di zigzag)
    int frameCount; // dem khung hinh de tinh toan di chuyen
    int state;
    int health;
    bool alive;
    Uint32 ePrevTime;
    Uint32 cooldown;
    bool isFrozen;
};

#endif 