#include "Enemy.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>
#include <ctime>

using namespace std;

Enemy :: Enemy (int _x, int _y, int _speed, int _movingtype) {
    
    texture = NULL;
    x = _x;
    y = _y;
    speed = _speed;
    movingtype = _movingtype;
    state = 0;
    alive = true;
}

Enemy :: ~Enemy () {
    free ();
}

bool Enemy :: loadTexture (SDL_Renderer* gRenderer, string path) {
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
    SDL_DestroyTexture (texture);
    texture = NULL;
}

void Enemy :: spawn (int spawnX, int spawnY) {
    x = spawnX;
    y = spawnY;
    
}



void Enemy :: update (int SCREEN_WIDTH, int SCREEN_HEIGHT) {
    if (!alive) {
        return; // neu quai chet thi loai bo
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
            speed = (rand() % 2 == 0) ? 7 : -7; // neu rand = 0 thi speed = 5 rand = 1 thi nguoc lai
        }
        x += speed;
        if (x >= SCREEN_WIDTH - 35 || x <= 0) {
            speed = -speed;
        }

        
        break;
    }
    }
    
}

void Enemy :: render (SDL_Renderer* gRenderer) {
    if (alive) {
        SDL_Rect position = {x, y, 35, 35};
        SDL_RenderCopy(gRenderer, texture, nullptr, &position);
        
    } 
}

SDL_Rect Enemy :: getRect () const{
    return {x, y, 35, 35};
}

bool Enemy :: isAlive () const{
    return alive;
}

void Enemy :: setAlive (bool _alive) {
    alive = _alive;
}