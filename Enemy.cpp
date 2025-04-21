#include "Enemy.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>
#include <ctime>
#include <algorithm>

using namespace std;

Enemy :: Enemy (int _x, int _y, int _speed, int _movingtype) {
    
    texture = NULL;
    x = _x;
    y = _y;
    speed = _speed;
    movingtype = _movingtype;
    state = 0;
    health = 10;
    alive = true;
    frameCount = 0;
    ePrevTime = 0;
    cooldown = 1500;
    isFrozen = false;
}

Enemy :: ~Enemy () {
    free ();
}

bool Enemy :: loadTexture (SDL_Renderer* gRenderer, string path) {
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

void Enemy :: free () {
    if (texture != NULL) {
        SDL_DestroyTexture (texture);
        texture = NULL;
    }
}


void Enemy :: update ( int SCREEN_WIDTH, int SCREEN_HEIGHT, vector <EnemyBullet> &eStorage) {
    int height = SCREEN_HEIGHT;

    if (!alive || isFrozen) {
        
        return; // neu quai chet hoac dong bang thi ko update j ca
    }

    
    
    frameCount++; //  tăng lên 1 sau mỗi khung hình và được sử dụng để tạo hiệu ứng chuyển động mượt mà
    if (state == 0) {
        // dau tien quai se di chuyen xuong vi tri SCREEN_HEIGHT / 3
        y += speed;
        if (y >= SCREEN_HEIGHT /  3) {
            state = 1; // chuyen trang thai
            
        }
    }

    else if (state == 1) {
    switch (movingtype) {
        case 0:
        x += speed;
        if (x >= SCREEN_WIDTH - 35 || x <= 0) {
            speed = -speed;
            
        }
        break;

        case 1:
        x += speed;
        y += (int)(3 * sin (frameCount / 10));
        /*sin: nham tao ra su dao dong tu -1 den 1
        frameCount giup chuyen dong muot hon
        5 * .. : bien do dao dong la 5*/

        if (x >= SCREEN_WIDTH - 35|| x <= 0) {
            speed = -speed;
            
        }
        break;

        case 2:
        if (frameCount % 60 == 0) { // thay doi sau 60 khung hinh
            speed = (rand() % 2 == 0) ? 7 : -7; // neu rand = 0 thi speed = 2 rand = 1 thi nguoc lai
        }
        x += speed;
        if (x >= SCREEN_WIDTH - 35 || x <= 0) {
            speed = -speed;
        }

        
        break;

        case 3:
        if (frameCount % 60 == 0) { // thay doi sau 60 khung hinh
            speed = (rand() % 2 == 0) ? 7 : -7; // neu rand = 0 thi speed = 2 rand = 1 thi nguoc lai
        }
        
        x += speed;
        y += (int)(3 * sin (frameCount / 10));
        /*sin: nham tao ra su dao dong tu -1 den 1
        frameCount giup chuyen dong muot hon
        5 * .. : bien do dao dong la 5*/

        if (x >= SCREEN_WIDTH - 35|| x <= 0) {
            speed = -speed;
            
        }
        break;
    }
    }
    
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - ePrevTime > cooldown) {
        EnemyBullet eAttack;
        eAttack.initialize (x + 9, y + 25);
        eStorage.push_back(eAttack);
        ePrevTime = currentTime;
    }
}

void Enemy :: render (SDL_Renderer* gRenderer) {
    if (alive) {
        SDL_Rect position = {x, y, 35, 35};
        SDL_RenderCopy(gRenderer, texture, NULL, &position);
        
    }
}

SDL_Rect Enemy :: getRect () {
    return {x, y, 35, 35};
}

bool Enemy :: isAlive () {
    return alive;
}

void Enemy :: takedamage (int damage, double damageBoost) {
    health -= (damage * damageBoost);
    if (health <= 0) {
        alive = false;
    }
}

void Enemy :: buffDrop (vector <Buff*> &buffs) {
    // 20% rơi buff
    if (rand() % 100 < 20) {
        Buff* buff = new Buff (x + 35/2 - 15, y );
        
            buffs.push_back (buff);
    }
}

bool Enemy :: FrozenOrNot () {
    return isFrozen; // Kiểm tra đóng băng
}

void Enemy :: setFrozen (bool _isFrozen) {
    isFrozen = _isFrozen;
}

