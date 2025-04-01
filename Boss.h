#ifndef BOSS_H
#define BOSS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include "Type0Bullet.h"
#include "Type1Bullet.h"
#include "Type2Bullet.h"
#include "Type3Bullet.h"
#include "Buff.h"

using namespace std;

class Boss  {
    public:

    Boss (int _x, int _y, int _speed, int _type) ;
    ~Boss ();

    bool loadTexture (SDL_Renderer* gRenderer, string path) ;

    void free ();

    void update ( int SCREEN_WIDTH, int SCREEN_HEIGHT, vector<Type0Bullet>& bStorage0, 
        vector <Type1Bullet> & bStorage1, 
        vector <Type2Bullet> & bStorage2,
        vector <Type3Bullet> & bStorage3,
        int playerX, int playerY);

    void render (SDL_Renderer* gRenderer);

    SDL_Rect getRect () ;

    bool isAlive () ;

    void setAlive (bool _alive);

    void takeDamage(int damage, double damageBoost);

    int getType();

    void buffDrop (vector <Buff*> &buffs);

    bool FrozenOrNot ();

    void setFrozen (bool _isFrozen);

    private:
    SDL_Texture* texture;
    int x;
    int y;
    int speed;
    int type;
    bool alive;
    int health;
    Uint32 bPrevAttackTime0;
    Uint32 bPrevAttackTime1;
    Uint32 bPrevAttackTime2;
    Uint32 bPrevAttackTime3;
    Uint32 cooldown;
    int bulletForATurn;
    int bulletsPerShot;
    Uint32 startTime;
    Uint32 timeBetweenBullet;
    bool isFrozen;
};

#endif