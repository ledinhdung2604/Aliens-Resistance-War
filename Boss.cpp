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
}

Boss :: ~Boss () {
    free ();
}

bool Boss :: loadTexture (SDL_Renderer* gRenderer, string path) {
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
    SDL_DestroyTexture (texture);
    texture = NULL;
}

void Boss :: update (int SCREEN_WIDTH, int SCREEN_HEIGHT, 
    vector<Type0Bullet>& bStorage0, 
    vector <Type1Bullet> & bStorage1, 
    vector <Type2Bullet> & bStorage2,
    vector <Type3Bullet> & bStorage3,
    int spaceshipX, int spaceshipY){

    if (!alive) {
        return; // neu quai chet thi loai bo
    }

    x += speed;
    if (x > SCREEN_WIDTH - BOSS_WIDTH || x < 0) {
        speed = -speed;    
    }

    if (type == 0 ) {
        Uint32 currentBullet = SDL_GetTicks();
        // Tao cac huong ngau nhien
        if (currentBullet - bPrevAttackTime0 >= cooldown) {
            random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution <double> angleRange (0, M_PI);

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

        if (currentBullet - bPrevAttackTime1 >= cooldown - 500) {
            Type1Bullet bAttack(x + 50, y + 190, spaceshipX, spaceshipY );
            bStorage1.push_back(bAttack);
            bPrevAttackTime1 = currentBullet;
        }
    }

    else if (type == 2 ) {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - bPrevAttackTime2 >= (cooldown)) {
            bulletsPerShot = 3;
            startTime = currentTime;
            bPrevAttackTime2 = currentTime;
        }

        if (bulletsPerShot > 0) {
            Uint32 timeDiff = currentTime - startTime;
            int bulletShot = 3 - bulletsPerShot;

            if (timeDiff >= bulletShot * 100) {
                Type2Bullet bAttack (x + 90, y + 190, spaceshipX, spaceshipY);
                bStorage2.push_back (bAttack);
                bulletsPerShot--;
            }
        }
    }

    else if (type == 3) {
        Uint32 currentBullet = SDL_GetTicks();

        if (currentBullet - bPrevAttackTime3 >= cooldown - 1500) {
            Type3Bullet bAttack(x + 90, y + 190, spaceshipX, spaceshipY );
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

void Boss :: takeDamage (int damage) {
    health -= damage;
    if (health <= 0) {
        alive = false;
    }
}

int Boss :: getType () {
    return type;
}