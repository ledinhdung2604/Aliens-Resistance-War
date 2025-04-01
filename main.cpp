#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <algorithm>
#include <ctime>

#include "Player.h"
#include "Background.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Fire.h"
#include "Boss.h"
#include "Type0Bullet.h"


using namespace std;


//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

// khai bao bien dinh dang Player
Player spaceship;

// Khai bao bien dinh dang Background
Background universe;

// Khai bao bien dinh dang Bullet
Bullet attack;

Bullet reflectBullet;

// khai bao mot chuoi enenmy
vector <Enemy> aliens;

// Khai bao bien dinh dang Fire
Fire fume;

// Khai bao chuoi chua anh cua fire
vector <string> paths = {"Source/image/fire1.png", "Source/image/fire2.png", "Source/image/fire3.png", "Source/image/fire4.png", "Source/image/fire5.png"};

// Khai bao vector enemyBullet
vector <EnemyBullet> eStorage;

// Khai bao vector Boss
vector <Boss> bosses;

// Khai bao vector BossBullet
vector <Type0Bullet> bStorage0;

vector <Type1Bullet> bStorage1;

vector <Type2Bullet> bStorage2;

vector <Type3Bullet> bStorage3;

// Khai bao vector buff
vector <Buff*> buffs;

Uint32 currentFreeze;

Uint32 lastFreeze = 0;

// Vector reflect bullet
vector <Bullet> reflectAttack;

Uint32 buffPrevTime = 0;

Uint32 timeForNextSpawn;


const int middle = 10;

bool init () { 
    bool success = true;
    if ( SDL_Init (SDL_INIT_VIDEO) < 0) // Hàm SDL_Init() được gọi để khởi tạo thư viện SDL.
    // Tham số SDL_INIT_VIDEO chỉ định rằng chúng ta chỉ muốn khởi tạo các chức năng liên quan đến video. 
    // Nếu hàm trả về giá trị nhỏ hơn 0, điều đó có nghĩa là khởi tạo thất bại.
    {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        success = false;
    }

    else {
        gWindow = SDL_CreateWindow("Aliens: Resistance War", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        // Nếu SDL khởi tạo thành công, một cửa sổ sẽ được tạo ra bằng hàm SDL_CreateWindow().
        /*Các tham số bao gồm tiêu đề cửa sổ, vị trí cửa sổ (ở đây là SDL_WINDOWPOS_UNDEFINED để hệ điều hành tự chọn vị trí)
        chiều rộng (SCREEN_WIDTH) và chiều cao (SCREEN_HEIGHT) của cửa sổ
        cờ SDL_WINDOW_SHOWN để hiển thị cửa sổ ngay lập tức.*/

        if (gWindow == NULL) {
            cout << "Window could not be created! SDL Error! " << SDL_GetError() << endl;
            success = false;
        } // Kiểm tra xem cửa sổ có được tạo thành công hay không. Nếu gWindow là NULL, điều đó có nghĩa là tạo cửa sổ thất bại.

        else {
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
            // SDL_CreateRenderer: Tạo renderer để vẽ đồ họa lên cửa sổ.
            if( gRenderer == NULL )
            {
                cout <<  "Renderer could not be created! SDL Error: %s\n", SDL_GetError() ;
                success = false;
            }
            else {
                SDL_SetRenderDrawColor( gRenderer, 0, 0, 255, 255 );
                
            }
            // SDL_SetRenderDrawColor: Đặt màu nền của renderer là màu xanh lam (0, 0, 255).
        
    }
   }
   return success; /*Nếu quá trình khởi tạo và tạo cửa sổ thành công, success sẽ là true. 
        Ngược lại, nếu có bất kỳ lỗi nào xảy ra, success sẽ là false*/
} //khoi tao mot cua so do hoa

bool loadMedia () {
    bool success = true;

    if (!fume.loadTexture(gRenderer, paths)) {
        cout << "Failed to load texture!\n";
        success = false;
    }

    if (!spaceship.loadTexture(gRenderer, "Source/image/DurrrSpaceShip.png")) {
        cout << "Failed to load texture!\n";
        success = false;
    }

    if (!universe.loadTexture(gRenderer, "Source/image/back.png")) {
        cout << "Failed to load texture!\n";
        success = false;
    }

    if (!attack.loadTexture(gRenderer, "Source/image/bullet.png")) {
        cout << "Failed to load texture!\n";
        success = false;
    }

    if (!reflectBullet.loadTexture(gRenderer, "Source/image/reflect.png")) {
        cout << "Failed to load texture!\n";
        success = false;
    }

    
    return success;
}

bool checkCollision (SDL_Rect& a, SDL_Rect& b) {
    // goi canh cua hinh chu nhat
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    // Tinh cac canh cua hinh chu nhat A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    // Tinh cac canh cua hinh chu nhat B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    // neu bat ky canh cua A nam ngoai 
    if (bottomA <= topB) {
        return false;
    }

    if (topA >= bottomB) {
        return false;
    }

    if (rightA <= leftB) {
        return false;
    }

    if (leftA >= rightB) {
        return false;
    }
    return true;
}

void  spawn (vector <Enemy> &aliens, string s[]) {
    int spawnX = rand () % (SCREEN_WIDTH - 35); // Chon vi tri o tren dinh tren cua man hinh
    int spawnY = 0;
    
    int speed = 7;
    int movingtype = rand() % 4; // Chon loai di chuyen random

    Enemy rebornEnemy (spawnX, spawnY, speed, movingtype);
    int index = rand() % 5; // chon index ngau nhien cho enemy

    if (rebornEnemy.loadTexture (gRenderer, s[index])){
        aliens.push_back (rebornEnemy);
    }
}

void bossSpawning (string bossFiles[]) {
    bool isBossAlive = false;
    for (auto& boss : bosses) {
        if (boss.isAlive()) {
            isBossAlive = true;
            break;
        }
    }

    if (!isBossAlive) {
        int spawnX = rand() % (SCREEN_WIDTH - 200); 
        int spawnY = -50; 
        int speed = 2; 
        int type = rand() % 4;

        Boss spawnBoss(spawnX, spawnY, speed, type);
        
        bosses.push_back(spawnBoss); 
        timeForNextSpawn = SDL_GetTicks() + (rand() % 30000 + 30000);
    }
}

void applyBuffToPlayer( Buff& buff) {
    Uint32 buffLength = 5000;
    switch (buff.getType()) {
        case HEAL:
            break;
        case SHIELD:
            buffLength = 7000;
            break;
        case DAMAGE_BOOST:
            buffLength = 8000;
            break;
        case ATTACK_SPEED:
            buffLength = 6000;
            break;
        case UP_MAX_HP:
            break;
        case REFLECT:
            buffLength = 10000;
            break;
        case FREEZE:
            buffLength = 5000;
            break;
        
        case SPEED_BOOST:
            buffLength = 5000;
            break;
    }

    spaceship.applyBuff (buff.getType(), buffLength);
}


void close () {
    spaceship.free ();
    universe.free();
    attack.free();
    for (auto& enemy : aliens) {
        enemy.free();
    }
    
    for (auto& eBullet : eStorage) {
        eBullet.free();
    }

    for (auto& boss : bosses) {
        boss.free();
    }

    for (auto& bBullet : bStorage0) {
        bBullet.free();
    }

    for (auto& bBullet : bStorage1) {
        bBullet.free();
    }

    for (auto& bBullet : bStorage2) {
        bBullet.free();
    }

    for (auto& bBullet : bStorage3) {
        bBullet.free();
    }

    for (auto& rBullet : reflectAttack) {
        rBullet.free();
    }

    for (auto& buff : buffs) {
        delete buff;
    }
    buffs.clear();

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;

    IMG_Quit();
    SDL_Quit();
}

int main (int argc, char * argv[]) {
    //Start up SDL and create window
    if( !init() )
    {
        cout << "Failed to initialize!\n" ;
    }
    else
    {
        //Load media
        if( !loadMedia() )
        {
            cout <<  "Failed to load media!\n" ;
        }

        else {
            string bossFiles[] = {"Source/image/boss1.png", "Source/image/boss2.png", "Source/image/boss3.png", "Source/image/boss4.png"};
            
            string s[] = {"Source/image/enemy1.png", "Source/image/enemy2.png", "Source/image/enemy3.png", "Source/image/enemy4.png", "Source/image/enemy5.png"};

            aliens.push_back(Enemy(0, 0, 7, 3));
            aliens.push_back(Enemy(145, 0, 7, 3));
            aliens.push_back(Enemy(390, 0, 7, 0));
            aliens.push_back(Enemy(515, 0, 7, 2));
            aliens.push_back(Enemy(725, 0, 7, 1));
            
            vector <Bullet> storage;

            timeForNextSpawn = SDL_GetTicks() + (rand() % 30000 + 30000);

            bool quit = false;

            SDL_Event e;
            
            Uint32 previousBullet = 0; // thoi diem ban vien dan truoc
            const Uint32 cooldown = 200; // Khoang thoi gian cho giua cac lan ban la 200 ms

            Uint32 timeLastDamage = 0;

            Uint32 currentFreeze = SDL_GetTicks ();


            // Trang thai cua cac phim
            bool keys[SDL_NUM_SCANCODES] = {false};

            while (!quit) {

                while (SDL_PollEvent (& e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }

                    else if (e.type == SDL_KEYDOWN) { // Xu li khi an phim
                        keys[e.key.keysym.scancode] = true;
                    }

                    else if (e.type == SDL_KEYUP) {
                        // Xu li khi tha phim
                        keys[e.key.keysym.scancode] = false;
                    }
                }
                
                spaceship.handleEvent(keys);
                spaceship.update ();

                if (keys[SDL_SCANCODE_SPACE]) {
                    Uint32 currentBullet = SDL_GetTicks (); // Lay thoi gian cua vien dan hien tai
                    if (currentBullet - previousBullet > cooldown) {
                    Bullet newatk = attack;
                    newatk.initialize (spaceship.getX() + middle, spaceship.getY());
                    storage.push_back (newatk);
                    previousBullet = currentBullet;}
                }

                // Cap nhat hieu ung
                fume.update();

                //Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF);
                SDL_RenderClear( gRenderer );

                // Render background
                universe.render(gRenderer);


                // Render enemy
                for (int i = 0; i < (int)aliens.size(); i++) {
                    if (aliens[i].loadTexture(gRenderer, s[i])) {
                        aliens[i].update ( SCREEN_WIDTH, SCREEN_HEIGHT, eStorage);
                        aliens[i].render (gRenderer);
                    }
                }

                // Cap nhat va render dan
                for (auto& eBullet : eStorage) {
                    if (eBullet.loadTexture(gRenderer, "Source/image/laserBullet.png")) {
                        eBullet.update();
                        eBullet.render(gRenderer);

                        SDL_Rect bulletRect = eBullet.getRect();
                        SDL_Rect playerRect = spaceship.getPosition();
                        if (checkCollision(bulletRect, playerRect)) {
                            if (spaceship.isReflect()) {
                                reflectBullet.initialize(eBullet.getRect().x, eBullet.getRect().y + 20);
                                reflectAttack.push_back(reflectBullet);
                            } else {
                                spaceship.takeDamage(10);
                            }
                            eBullet.isCollided(true);
                        }
                    }
                }


                // Render va spawn boss
                for (int i = 0; i < (int)bosses.size(); i++) {
                    if (bosses[i].loadTexture(gRenderer, bossFiles[bosses[i].getType()])) {
                        bosses[i].update(SCREEN_WIDTH, SCREEN_HEIGHT, 
                            bStorage0, 
                            bStorage1, 
                            bStorage2, bStorage3,
                            spaceship.getX(), spaceship.getY());
                        bosses[i].render(gRenderer); 
                    }
                }

                // Render va tao bullet
                for (auto& bBullet : bStorage0) {
                    
                        if (bBullet.loadTexture (gRenderer, "Source/image/bossShoot.png")) {
                            bBullet.update();
                            bBullet.render(gRenderer);

                            SDL_Rect bulletRect = bBullet.getRect();
                            SDL_Rect playerRect = spaceship.getPosition();
                            if (checkCollision(bulletRect, playerRect)) {
                                spaceship.takeDamage(20);
                                bBullet.isCollided(true);
                            }
                        }  
                    
                }

                for (auto& bBullet : bStorage1) {
                    
                    if (bBullet.loadTexture (gRenderer, "Source/image/bigShoot.png")) {
                        bBullet.update();
                        bBullet.render(gRenderer);

                        SDL_Rect bulletRect = bBullet.getRect();
                        SDL_Rect playerRect = spaceship.getPosition();
                        if (checkCollision(bulletRect, playerRect)) {
                            spaceship.takeDamage(50);
                            bBullet.isCollided(true);
                        }
                    }  
                
                }

                for (auto& bBullet : bStorage2) {
                    if (bBullet.chaseOrNot()) {
                        bBullet.getSpaceship (spaceship.getX(), spaceship.getY());
                    }

                    if (bBullet.loadTexture (gRenderer, "Source/image/chaseShoot.png")) {
                        bBullet.update();
                        bBullet.render(gRenderer);

                        SDL_Rect bulletRect = bBullet.getRect();
                        SDL_Rect playerRect = spaceship.getPosition();
                        if (checkCollision(bulletRect, playerRect)) {
                            spaceship.takeDamage(10);
                            bBullet.isCollided(true);
                        }
                    }  
                }


                for (auto& bBullet : bStorage3) {
                    
                    if (bBullet.loadTexture (gRenderer, "Source/image/energyball.png")) {
                        bBullet.update();
                        bBullet.render(gRenderer);

                        SDL_Rect bulletRect = bBullet.getRect();
                        SDL_Rect playerRect = spaceship.getPosition();
                        if (checkCollision(bulletRect, playerRect)) {
                            spaceship.takeDamage(15);
                            bBullet.isCollided(true);
                        }
                    }  
                
                }

                // Render spaceship
                spaceship.render(gRenderer);

                // Render fire under spaceship
                int fumeX = spaceship.getX() + middle + 2;
                int fumeY = spaceship.getY() + 40;
                fume.render (gRenderer, fumeX, fumeY);
                
                // Cập nhật và render đạn
                for (auto& bullet : storage) {
                    bullet.update(spaceship.getAttackSpeedUp());
                    bullet.render(gRenderer);
                
                    
                    for (auto& enemy : aliens) {
                        SDL_Rect bulletRect = bullet.getRect();
                        SDL_Rect enemyRect = enemy.getRect();
                        if (checkCollision(bulletRect, enemyRect)) {
                            enemy.takedamage(10, spaceship.getDamageIncrease()); // Quái vật bị tiêu diệt
                            bullet.isCollided(true); // Dan va cham voi quai

                            if (!enemy.isAlive()) {
                                enemy.buffDrop(buffs);
                            }

                        }
                    }

                    for (auto& boss : bosses) {
                        SDL_Rect bulletRect = bullet.getRect();
                        SDL_Rect bossRect = boss.getRect();
                        if (checkCollision(bulletRect, bossRect)) {
                            boss.takeDamage(20, spaceship.getDamageIncrease()); // Quái vật bị tiêu diệt
                            bullet.isCollided(true); // Dan va cham voi quai
                            if (!boss.isAlive()) {
                                boss.buffDrop(buffs);
                            }
                        }
                    }
                }

                for (auto iter = buffs.begin(); iter != buffs.end();) {
                    Buff* buff = *iter;
                    if (buff->loadTexture(gRenderer, "Source/image/buff.png")) {
                        buff->update ();

                        SDL_Rect buffRect = buff->getRect();
                        SDL_Rect spaceshipRect = spaceship.getPosition();

                        if (checkCollision(buffRect, spaceshipRect) ) {
                            applyBuffToPlayer (*buff);
                            delete buff;
                            iter = buffs.erase(iter);
                        }

                        else if (buff->isOutOfScreen(SCREEN_HEIGHT)) {
                            delete buff;
                            iter = buffs.erase(iter);
                        }

                        else {
                            buff->render(gRenderer);
                            ++iter;
                        }
                    }

                    else {
                        delete buff;
                    }
                }

                // Kiểm tra trạng thái FREEZE
                bool isFrozen = false;
                for (const auto& buff : spaceship.getActiveBuffs()) {
                    if (buff.type == FREEZE && SDL_GetTicks() - buff.activateTime <= buff.buffLength) {
                        isFrozen = true;
                        break;
                    }
                }

                // Áp dụng đóng băng cho enemy và boss
                for (auto& enemy : aliens) {
                    enemy.setFrozen(isFrozen);
                }
                for (auto& boss : bosses) {
                    boss.setFrozen(isFrozen);
                }
                
                // Kiem tra thoi gian hoat dong cua buff
                spaceship.updateBuffs ();

                // Phan dan
                for (auto& rBullet : reflectAttack) {
                    if (rBullet.loadTexture (gRenderer, "Source/image/reflect.png")) {
                        rBullet.update(spaceship.getAttackSpeedUp());
                        rBullet.render(gRenderer);
                
                        // Kiểm tra va chạm với enemy
                        for (auto& enemy : aliens) {
                            SDL_Rect reflectRect = rBullet.getRect();
                            SDL_Rect enemyRect = enemy.getRect();
                            if (checkCollision(reflectRect, enemyRect)) {
                                enemy.takedamage(10, spaceship.getDamageIncrease()); // Gây sát thương
                                rBullet.isCollided(true);
                            }
                        }

                        for (auto& boss : bosses) {
                            SDL_Rect reflectRect = rBullet.getRect();
                            SDL_Rect bossRect = boss.getRect();
                            if (checkCollision(reflectRect, bossRect)) {
                                boss.takeDamage(10, spaceship.getDamageIncrease()); // Gây sát thương
                                rBullet.isCollided(true);
                            }
                        }
                    }
                }


                // xoa dan da ra khoi man hinh
                storage.erase(remove_if(storage.begin(), storage.end(), [](Bullet& bullet) {
                    return bullet.isOutofBorder(SCREEN_HEIGHT) || bullet.collision(); // Kiểm tra đạn có ra khỏi màn hình không
                }), storage.end());

                eStorage.erase(remove_if(eStorage.begin(), eStorage.end(), [](EnemyBullet& eBullet) {
                    return eBullet.isOutofBorder(SCREEN_HEIGHT) || eBullet.collision(); // Kiểm tra đạn có ra khỏi màn hình không
                }), eStorage.end());

                bStorage0.erase(remove_if(bStorage0.begin(), bStorage0.end(), [](Type0Bullet& bBullet) {
                    return bBullet.isOutOfBorder(SCREEN_WIDTH, SCREEN_HEIGHT) || bBullet.collision(); // Kiểm tra đạn có ra khỏi màn hình không
                }), bStorage0.end());

                bStorage1.erase(remove_if(bStorage1.begin(), bStorage1.end(), [](Type1Bullet& bBullet) {
                    return bBullet.isOutOfBorder(SCREEN_WIDTH, SCREEN_HEIGHT) || bBullet.collision(); // Kiểm tra đạn có ra khỏi màn hình không
                }), bStorage1.end());

                bStorage2.erase(remove_if(bStorage2.begin(), bStorage2.end(), [](Type2Bullet& bBullet) {
                    return bBullet.isOutOfBorder(SCREEN_WIDTH, SCREEN_HEIGHT) || bBullet.collision(); // Kiểm tra đạn có ra khỏi màn hình không
                }), bStorage2.end());

                bStorage3.erase(remove_if(bStorage3.begin(), bStorage3.end(), [](Type3Bullet& bBullet) {
                    return bBullet.isOutOfBorder(SCREEN_WIDTH, SCREEN_HEIGHT) || bBullet.collision(); // Kiểm tra đạn có ra khỏi màn hình không
                }), bStorage3.end());

                reflectAttack.erase(remove_if(reflectAttack.begin(), reflectAttack.end(), [](Bullet& rBullet) {
                    return rBullet.isOutofBorder( SCREEN_HEIGHT) || rBullet.collision(); // Kiểm tra đạn có ra khỏi màn hình không
                }), reflectAttack.end());

                bosses.erase(remove_if(bosses.begin(), bosses.end(), []( Boss& boss) {
                    return !boss.isAlive();
                }), bosses.end());
                
                aliens.erase(remove_if(aliens.begin(), aliens.end(), []( Enemy& enemy) {
                    return !enemy.isAlive();
                }), aliens.end());


                // Neu so enemy < 5 thi spawn lai
                if (aliens.size() < 5) {
                    spawn (aliens, s);
                }

                // Spawn boss theo thoi diem
                if (SDL_GetTicks() > timeForNextSpawn) {
                    bossSpawning (bossFiles);
                }
                
                if (!spaceship.isAlive()) {
                    cout << "Game over!" << endl;
                    quit = true;
                }
                
                //Update screen
                SDL_RenderPresent( gRenderer );

                // Điều chỉnh tốc độ cập nhật
                SDL_Delay(20);
            }
        }
    }
    close ();
    return 0;
}