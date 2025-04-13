#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
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
#include "Meteor.h"
#include "BlackHole.h"

/*Cơ chế game: Điều Khiển player tấn công boss và enemy, khi boss và enemy bị tiêu diệt sẽ rơi ra buff.
Có thêm thiên thạch và hố đen*/

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

// Khai báo bullet bị phản lại
Bullet reflectBullet;

// Khai bao dan player
vector <Bullet> storage;

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

// Thời gian đóng băng hiện tại
Uint32 currentFreeze;

// Thời gian đóng băng trước đó
Uint32 lastFreeze = 0;

// Vector reflect bullet
vector <Bullet> reflectAttack;

// Khai bao vector Meteor
vector <Meteor> meteors;

// Thoi diem cua lan spawn meteor truoc do
Uint32 lastMeteorSpawn = 0;

// Khoang thoi gian giua cac lan spawn meteor
Uint32 meteorTimeDiff = 0;

// Khai bao nhung thong tin ve BlackHole
vector <BlackHole> blackholes;
Uint32 lastHoleSpawn = 0; // Thời điểm spawn hole trước
Uint32 holeActiveInterval = 20000; // Khoảng thời gian hoạt động
Uint32 lastHoleDamage = 0; // thời gian gây dame trước đó
int holeDamageInterval = 1000; // khoảng thời gây dame (cứ 1s gây 1 dame)
Uint32 lastHoleFrame = 0; // Khung hình trước của hole

// Thong tin lien quan den buff
Uint32 buffPrevTime = 0; // Thời gian hiệu lực buff trước đó

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
        int spawnX = rand() % (SCREEN_WIDTH - 200);  // vị trí ngẫu nhiên trên đỉnh màn hình
        int spawnY = -50; 
        int speed = 2; 
        int type = rand() % 4;

        Boss spawnBoss(spawnX, spawnY, speed, type);
        
        bosses.push_back(spawnBoss); 
        timeForNextSpawn = SDL_GetTicks() + (rand() % 30000 + 30000); // Thời gian spawn trong khoảng 30 đến 60s
    }
}

void applyBuffToPlayer( Buff& buff) {
    Uint32 buffLength = 5000;
    switch (buff.getType()) {
        case HEAL:
            break;
        case SHIELD:
            buffLength = 7000; // Thời gian hoạt động của shield
            break;
        case DAMAGE_BOOST:
            buffLength = 8000;// Thời gian hoạt động của tăng dame
            break;
        case ATTACK_SPEED:
            buffLength = 6000; // Thời gian hoạt động của tốc độ đánh
            break;
        case UP_MAX_HP:
            break;
        case REFLECT:
            buffLength = 10000; // Thời gian hoạt động của phản đạn
            break;
        case FREEZE:
            buffLength = 5000; // Thời gian đóng băng
            break;
        
        case SPEED_BOOST:
            buffLength = 5000; // // Thời gian hoạt động của tốc độ di chuyển
            break;
    }

    spaceship.applyBuff (buff.getType(), buffLength); // áp dụng buff cho player
}


void spawnMeteor () {
    int direction = rand() % 4;
    int x, y;

    switch (direction) {
        case 0: // tren man hinh
            x = rand() % (SCREEN_WIDTH - 40);
            y = -40;
            break;
        case 1: // bên trái man hinh
            x = -40;
            y = rand() % (SCREEN_HEIGHT - 40);
            break;
        case 2: // dưới man hinh
            x = rand() % (SCREEN_WIDTH - 40);
            y = SCREEN_HEIGHT;
            break;
        case 3: // bên phải man hinh
            x = SCREEN_WIDTH;
            y = rand() % (SCREEN_HEIGHT - 40);
            break;
    }

    Meteor comingMeteor (x, y, direction) ;
    meteors.push_back (comingMeteor);

    meteorTimeDiff = rand () % 3000;
    lastMeteorSpawn = SDL_GetTicks();
}

void spawnBlackHole () {
    // spawn black hole ngẫu nhiên ở nửa dưới màn hình
    int x = rand() % (SCREEN_WIDTH - 200);
    int y = rand() % (SCREEN_HEIGHT - 200) + (SCREEN_HEIGHT/2);

    BlackHole comingBlackHole (x, y);
    blackholes.push_back (comingBlackHole);
    holeActiveInterval = rand() % 10000 + 10000;
    lastHoleSpawn = SDL_GetTicks ();
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

    for (auto& meteor : meteors) {
        meteor.free();
    }

    for (auto& blackhole : blackholes) {
        blackhole.free();
    }

    aliens.clear();
    storage.clear();
    eStorage.clear();
    bosses.clear();
    bStorage0.clear();
    bStorage1.clear();
    bStorage2.clear();
    bStorage3.clear();
    reflectAttack.clear();
    meteors.clear();
    blackholes.clear();

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
            // Ảnh boss
            string bossFiles[] = {"Source/image/boss1.png", "Source/image/boss2.png", "Source/image/boss3.png", "Source/image/boss4.png"};
            
            // ảnh enemy 
            string s[] = {"Source/image/enemy1.png", "Source/image/enemy2.png", "Source/image/enemy3.png", "Source/image/enemy4.png", "Source/image/enemy5.png"};

            aliens.push_back(Enemy(0, 0, 7, 3));
            aliens.push_back(Enemy(145, 0, 7, 3));
            aliens.push_back(Enemy(390, 0, 7, 0));
            aliens.push_back(Enemy(515, 0, 7, 2));
            aliens.push_back(Enemy(725, 0, 7, 1));

            timeForNextSpawn = SDL_GetTicks() + (rand() % 30000 + 30000);

            bool quit = false;

            SDL_Event e;
            
            Uint32 previousBullet = 0; // thoi diem ban vien dan truoc
            const Uint32 cooldown = 200; // Khoang thoi gian cho giua cac lan ban la 200 ms

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
                    newatk.initialize (spaceship.getX() + middle, spaceship.getY()); // cho đạn ở vị trí giữa player
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
                                reflectBullet.initialize(eBullet.getRect().x, eBullet.getRect().y + 20); // Tạo đạn phản khik player có buff
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

                // Render va tao bullet của boss
                for (auto& bBullet : bStorage0) {
                    
                        if (bBullet.loadTexture (gRenderer, "Source/image/bossShoot.png")) {
                            bBullet.update();
                            bBullet.render(gRenderer);

                            SDL_Rect bulletRect = bBullet.getRect();
                            SDL_Rect playerRect = spaceship.getPosition();
                            // kiểm tra va chạm
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
                
                // Cập nhật và render đạn spaceship
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


                // Render và cập nhật buff bằng iterator
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
                
                // Render và cập nhật meteor
                for (auto& meteor : meteors) {
                    if (meteor.loadTexture (gRenderer, "Source/image/meteor.png")) {
                        meteor.update();
                        meteor.render(gRenderer);

                        SDL_Rect meteorRect = meteor.getRect();
                        SDL_Rect playerRect = spaceship.getPosition();
                        if (checkCollision(meteorRect, playerRect)) {
                            spaceship.takeDamage(5); // Gây 5 sát thương
                            meteor.collision(true);
                        }
                    }
                }

                // tao luc hut
                bool playerInRange = false;
                for (auto& blackhole : blackholes) {
                    if (blackhole.playerInRange (spaceship.getX(), spaceship.getY())) {
                        playerInRange = true;
                    }
                }

                // Điều chỉnh tốc độ khi có hố đen
                spaceship.setGravitySpeed (playerInRange);

                // Render blackhole va kiem tra va cham voi player
                for (auto& blackhole : blackholes) {
                    if (blackhole.loadTexture (gRenderer, "Source/image/blackhole.png")) {
                        blackhole.render (gRenderer);

                        SDL_Rect blackholeRect = blackhole.getRect ();
                        SDL_Rect spaceshipRect = spaceship.getPosition ();

                        if (checkCollision (blackholeRect, spaceshipRect)) {
                            if (SDL_GetTicks() - lastHoleDamage > holeDamageInterval) {
                                spaceship.takeDamage (1);
                                lastHoleDamage = SDL_GetTicks ();
                            }
                        }
                    }
                }

                // xoa dan da ra khoi man hinh
                storage.erase(remove_if(storage.begin(), storage.end(), [](Bullet& bullet) {
                    if (bullet.isOutofBorder(SCREEN_HEIGHT) || bullet.collision()) {
                        bullet.free();
                    }
                    return bullet.isOutofBorder(SCREEN_HEIGHT) || bullet.collision(); // Kiểm tra đạn có ra khỏi màn hình không
                }), storage.end());

                eStorage.erase(remove_if(eStorage.begin(), eStorage.end(), [](EnemyBullet& eBullet) {
                    if (eBullet.isOutofBorder(SCREEN_HEIGHT) || eBullet.collision()) {
                        eBullet.free();
                    }
                    return eBullet.isOutofBorder(SCREEN_HEIGHT) || eBullet.collision(); // Kiểm tra đạn có ra khỏi màn hình không
                }), eStorage.end());

                bStorage0.erase(remove_if(bStorage0.begin(), bStorage0.end(), [](Type0Bullet& bBullet) {
                    if (bBullet.isOutOfBorder(SCREEN_WIDTH, SCREEN_HEIGHT) || bBullet.collision()) {
                        bBullet.free();
                    }
                    return bBullet.isOutOfBorder(SCREEN_WIDTH, SCREEN_HEIGHT) || bBullet.collision(); // Kiểm tra đạn có ra khỏi màn hình không
                }), bStorage0.end());

                bStorage1.erase(remove_if(bStorage1.begin(), bStorage1.end(), [](Type1Bullet& bBullet) {
                    if (bBullet.isOutOfBorder(SCREEN_WIDTH, SCREEN_HEIGHT) || bBullet.collision()) {
                        bBullet.free();
                    }
                    return bBullet.isOutOfBorder(SCREEN_WIDTH, SCREEN_HEIGHT) || bBullet.collision(); // Kiểm tra đạn có ra khỏi màn hình không
                }), bStorage1.end());

                bStorage2.erase(remove_if(bStorage2.begin(), bStorage2.end(), [](Type2Bullet& bBullet) {
                    if (bBullet.isOutOfBorder(SCREEN_WIDTH, SCREEN_HEIGHT) || bBullet.collision()) {
                        bBullet.free();
                    }
                    return bBullet.isOutOfBorder(SCREEN_WIDTH, SCREEN_HEIGHT) || bBullet.collision(); // Kiểm tra đạn có ra khỏi màn hình không
                }), bStorage2.end());

                bStorage3.erase(remove_if(bStorage3.begin(), bStorage3.end(), [](Type3Bullet& bBullet) {
                    return bBullet.isOutOfBorder(SCREEN_WIDTH, SCREEN_HEIGHT) || bBullet.collision(); // Kiểm tra đạn có ra khỏi màn hình không
                }), bStorage3.end());

                reflectAttack.erase(remove_if(reflectAttack.begin(), reflectAttack.end(), [](Bullet& rBullet) {
                    if (rBullet.isOutofBorder( SCREEN_HEIGHT) || rBullet.collision()) {
                        rBullet.free();
                    }
                    return rBullet.isOutofBorder( SCREEN_HEIGHT) || rBullet.collision(); // Kiểm tra đạn có ra khỏi màn hình không
                }), reflectAttack.end());

                bosses.erase(remove_if(bosses.begin(), bosses.end(), []( Boss& boss) {
                    if (!boss.isAlive()) {
                        boss.free();
                    }
                    return !boss.isAlive();
                }), bosses.end());
                
                aliens.erase(remove_if(aliens.begin(), aliens.end(), []( Enemy& enemy) {
                    if (!enemy.isAlive()) {
                        enemy.free();
                    }
                    return !enemy.isAlive();
                }), aliens.end());

                meteors.erase(remove_if(meteors.begin(), meteors.end(), [](Meteor& meteor) {
                    if (meteor.isOutOfBorder(SCREEN_WIDTH, SCREEN_HEIGHT) || meteor.isCollided()) {
                        meteor.free();
                    }
                    return meteor.isOutOfBorder(SCREEN_WIDTH, SCREEN_HEIGHT) || meteor.isCollided(); // Kiểm tra đạn có ra khỏi màn hình không
                }), meteors.end());

                blackholes.erase(remove_if(blackholes.begin(), blackholes.end(), [](BlackHole& blackhole) {
                    if (blackhole.despawnTime()) {
                        blackhole.free();
                    }
                    return blackhole.despawnTime(); 
                }), blackholes.end());

                // Neu so enemy < 5 thi spawn lai
                if (aliens.size() < 5) {
                    spawn (aliens, s);
                }

                // Spawn boss theo thoi diem
                if (SDL_GetTicks() > timeForNextSpawn) {
                    bossSpawning (bossFiles);
                }
                
                // Spawn Meteor trong khoảng 0s đến 3s
                if (SDL_GetTicks () - lastMeteorSpawn > meteorTimeDiff) {
                    spawnMeteor();
                }
                
                // Spawn hole sau một khoảng thời gian 20s
                if (SDL_GetTicks () - lastHoleSpawn > holeActiveInterval) {
                    spawnBlackHole ();
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