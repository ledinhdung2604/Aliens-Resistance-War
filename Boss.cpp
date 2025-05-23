#include "Boss.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <random>

using namespace std;

const int BOSS_WIDTH = 200;
const int BOSS_HEIGHT = 200;


Boss :: Boss (int _x, int _y, int _speed, int _type) {
    texture = NULL;
    x = _x;
    y = _y;
    speed = _speed;
    type = _type;
    alive = true;
    health = 1000;
    bPrevAttackTime0 = 0;
    bPrevAttackTime1 = 0;
    bPrevAttackTime2 = 0;
    bPrevAttackTime3 = 0;
    cooldown = 3000;
    bulletForATurn = 0;
    timeBetweenBullet = 300;
    isFrozen = false;
    maxHealth = 1000;
}

Boss :: ~Boss () {
    free ();
}

bool Boss :: loadTexture (SDL_Renderer* gRenderer, string path) {
    free();
    
    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load (path.c_str());

    if (loadedSurface == NULL) {
        cout << "Unable to load image" << path.c_str() << IMG_GetError();
    }

    else {
        // Thiết lập một màu trong suốt (color key) cho hình ảnh.
        SDL_SetColorKey (loadedSurface, SDL_TRUE, SDL_MapRGB (loadedSurface->format, 0, 0xFF, 0xFF));

        // Tạo texture từ surface đã tải.
        newTexture = SDL_CreateTextureFromSurface (gRenderer, loadedSurface);

        if (newTexture == NULL) {
            cout << "Unable to create texture: " << path.c_str() << SDL_GetError();
        }


        SDL_FreeSurface(loadedSurface);
    }
    texture = newTexture;
    return texture != NULL;
}

void Boss :: free () {
    if (texture != NULL) {
        SDL_DestroyTexture (texture);
        texture = NULL;
    }
}

void Boss :: update (int SCREEN_WIDTH, int SCREEN_HEIGHT, 
    vector<Type0Bullet>& bStorage0, 
    vector <Type1Bullet> & bStorage1, 
    vector <Type2Bullet> & bStorage2,
    vector <Type3Bullet> & bStorage3,
    int spaceshipX, int spaceshipY){

    if (!alive || isFrozen) {
        return; 
    }

    x += speed;
    if (x > SCREEN_WIDTH - BOSS_WIDTH || x < 0) {
        speed = -speed;    
    }

    if (type == 0 ) {
        Uint32 currentBullet = SDL_GetTicks();
        // Tao cac huong ngau nhien
        if (currentBullet - bPrevAttackTime0 >= (cooldown - 1000)) {
            random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution <double> angleRange (0, M_PI); // bắn ngẫu nhiên ra hướng ngẫu nhiên trên góc 180

            for (int i = 0; i < 25; i++) {
                double angle = angleRange (gen);
                double velocity = 7;
                double velocityX = cos (angle) * velocity;
                double velocityY = sin (angle) * velocity;

                // Tao dan 
                Type0Bullet bAttack (x + 90, y + 190, velocityX, velocityY);
                bStorage0.push_back (bAttack);
            }

            bPrevAttackTime0 = currentBullet;
        }
    }

    else if (type == 1) {
        Uint32 currentBullet = SDL_GetTicks();

        if (currentBullet - bPrevAttackTime1 >= (cooldown - 1500)) {
            Type1Bullet bAttack(x + 50, y + 190, spaceshipX, spaceshipY ); // Tạo đạn
            bStorage1.push_back(bAttack);
            bPrevAttackTime1 = currentBullet;
        }
    }

    else if (type == 2 ) {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - bPrevAttackTime2 >= (cooldown - 1500)) {
            bulletsPerShot = 3; // Bắn 3 viên 1 lần
            startTime = currentTime;
            bPrevAttackTime2 = currentTime;
        }

        if (bulletsPerShot > 0) {
            Uint32 timeDiff = currentTime - startTime;
            int bulletShot = 3 - bulletsPerShot;

            if (timeDiff >= bulletShot * 300) { // khoảng cách mỗi 3 viên là 300ms
                Type2Bullet bAttack (x + 90, y + 190, spaceshipX, spaceshipY);
                bStorage2.push_back (bAttack);
                bulletsPerShot--;
            }
        }
    }

    else if (type == 3) {
        Uint32 currentBullet = SDL_GetTicks();

        if (currentBullet - bPrevAttackTime3 >= (cooldown - 2000)) {
            Type3Bullet bAttack(x + 90, y + 190, spaceshipX, spaceshipY ); // tạo đạn
            bStorage3.push_back(bAttack);
            bPrevAttackTime3 = currentBullet;
        }
    }

}

void Boss :: render (SDL_Renderer* gRenderer) {
    if (alive) {
        SDL_Rect position = {x, y, 200, 200}; 
        SDL_RenderCopy(gRenderer, texture, NULL, &position);
    }
}

SDL_Rect Boss :: getRect () {
    return {x, y, 200, 200};
}

bool Boss :: isAlive () {
    return alive;
}

void Boss :: setAlive (bool _alive) {
    alive = _alive;
}

void Boss :: takeDamage (int damage, double damageBoost) {
    health -= (damage * damageBoost);
    if (health <= 0) {
        alive = false;
    }
}

int Boss :: getType () {
    return type;
}

void Boss :: buffDrop (vector <Buff*> &buffs) {
    int numOfBuffs = 1; // 100% rơi 1 buff
    if (rand() % 100 < 50) { // 50% rơi 2 buff
        numOfBuffs++;
    }
    if (rand() % 100 < 10) { // 10% rơi 3 buff
        numOfBuffs++;
    }

    for (int i = 0; i < numOfBuffs; i++) {
        int spawnX = (rand() % 100) - 50; 
        int spawnY = (rand() % 100) - 50;
        Buff* buff = new Buff (x + 100 + spawnX, y + 100 + spawnY);
        buffs.push_back (buff);
    }
}

bool Boss :: FrozenOrNot () {
    return isFrozen; // KIểm tra đóng băng
}

void Boss :: setFrozen (bool _isFrozen) {
    isFrozen = _isFrozen;
}