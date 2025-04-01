#include "Player.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

using namespace std;




// Constructor
Player :: Player () {
    // Khởi tạo các giá trị mặc định
    position = {SCREEN_WIDTH/2 - 20, SCREEN_HEIGHT - 40, 40, 40};
    texture = NULL;
    speed = 7;
    health = 500;
    alive = true;
    damageIncrease = 1.0;
    attackSpeedUp = 1.0;
    speedIncrease = 1.0;
    shieldActivate = false;
    reflectActivate = false;
    maxHealth = 500;
}

Player :: ~Player () {
    free ();
}

bool Player :: loadTexture (SDL_Renderer* gRenderer,  std::string path) {

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

        else {
            position.w = loadedSurface->w;
            position.h = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }
    texture = newTexture;
    return texture != NULL;
}

void Player :: free () {
    if (texture != NULL) {
        SDL_DestroyTexture (texture);
        position = {0, 0, 0, 0};
        texture = NULL;
    }
}

void Player :: handleEvent (bool keys[]) {
    double currentSpeed = speed * speedIncrease;
    // Di chuyen dua tren trang thai phim
    if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP]) {
        position.y -= currentSpeed;
    }

    if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_RIGHT]) {
        position.x -= currentSpeed;
    }

    if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_LEFT]) {
        position.x += currentSpeed;
    }

    if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN]) {
        position.y += currentSpeed;
    }
}

void Player :: update () {
    if (position.x > SCREEN_WIDTH - 40) {
        position.x = SCREEN_WIDTH - 40;
    }

    else if (position.x < 0) {
        position.x = 0;
    }

    if (position.y > SCREEN_HEIGHT - 40) {
        position.y = SCREEN_HEIGHT - 40;
    }

    else if (position.y < 0) {
        position.y = 0;
    }
}

void Player :: render (SDL_Renderer* gRenderer) {
    // Vẽ texture lên renderer tại vị trí và kích thước được xác định bởi position.
    SDL_RenderCopy(gRenderer, texture, NULL, &position);
}

SDL_Rect Player :: getPosition () {
    return position;
}

int Player :: getX () {
    return position.x;
}

int Player :: getY () {
    return position.y;
}

void Player :: takeDamage (int damage) {
    if (!shieldActivate) {
        health -= damage;
        if (health <= 0) {
            health = 0;
            alive = false;
        }
    }
}

bool Player :: isAlive () {
    return alive;
}

void Player :: applyBuff (buffType type, Uint32 buffLength) {
    switch (type) {
        case HEAL:
        health = min(health + 30, health);
        break;

        case SHIELD:
        shieldActivate = true;
        break;

        case DAMAGE_BOOST:
        damageIncrease = 3.0;
        break;

        case ATTACK_SPEED:
        attackSpeedUp = 2.0;
        break;

        case REFLECT:
        reflectActivate = true;
        break;

        case UP_MAX_HP:
        maxHealth += 5;
        break;

        case FREEZE:
        break;

        case SPEED_BOOST:
        speedIncrease = 2.0;
        break;
    }

    buffWork activateBuff;
    activateBuff.type = type;
    activateBuff.activateTime = SDL_GetTicks();
    activateBuff.buffLength = buffLength;
    activeBuffs.push_back (activateBuff);
}

void Player :: updateBuffs () {
    Uint32 currentBuff = SDL_GetTicks();

    for (auto iter = activeBuffs.begin(); iter != activeBuffs.end(); ) {
        if (currentBuff - iter->activateTime >= iter->buffLength) {
            switch (iter->type) {
                case SHIELD: 
                shieldActivate = false;
                break;

                case DAMAGE_BOOST:
                damageIncrease = 1.0;
                break;

                case ATTACK_SPEED:
                attackSpeedUp = 1.0;
                break;

                case REFLECT:
                reflectActivate = false;
                break;

                case SPEED_BOOST:
                speedIncrease = 1.0;
                break;
            }
            iter = activeBuffs.erase(iter);
        }

        else {
            iter++;
        }
    }
}

double Player :: getSpeedIncrease () {
    return speedIncrease;
}

bool Player :: isReflect () {
    return reflectActivate;
}

double Player :: getDamageIncrease () {
    return damageIncrease;
}

double Player :: getAttackSpeedUp () {
    return attackSpeedUp;
}

vector <buffWork> Player :: getActiveBuffs () {
    return activeBuffs;
}