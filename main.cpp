#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
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
#include "Menu.h"

using namespace std;


//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

// Kahi bao bien cho font 
TTF_Font * gFont = NULL;

// Khai bao cien ve am thanh
Mix_Chunk* gSound = NULL;

// Khai Bao diem so
int playerPoint;

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

// Vector reflect bullet
vector <Bullet> reflectAttack;

// Khai bao vector Meteor
vector <Meteor> meteors;

// Thoi diem cua lan spawn meteor truoc do
Uint32 lastMeteorSpawn = 0;

// Khoang thoi gian giua cac lan spawn meteor
Uint32 meteorTimeDiff = 0;

// Thong tin lien quan den buff
Uint32 buffPrevTime = 0; // Thời gian hiệu lực buff trước đó

// Thoi gian spawn boss tiep theo
Uint32 timeForNextSpawn;

// Thoi spawn enemy tiep theo
Uint32 reviveCooldown = 2000;
Uint32 lastSurvive;

Uint32 buffTextStartTime = 0; // Thời điểm bắt đầu hiển thị tên buff
string currentBuffText = ""; // Tên buff hiện tại đang hiển thị
const Uint32 buffTextDuration = 2000; // Thời gian hiển thị tên buff (2 giây)


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
         // Khởi tạo SDL_mixer
         if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) { // Tan so: 44100 Hz, format 16 bit, am thanh 2 kenh, kich thuoc am thanh
            cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
            success = false;
        }

        if (TTF_Init() == -1) {
            cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << endl;
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
   }
   return success; /*Nếu quá trình khởi tạo và tạo cửa sổ thành công, success sẽ là true. 
        Ngược lại, nếu có bất kỳ lỗi nào xảy ra, success sẽ là false*/
} //khoi tao mot cua so do hoa

void renderScore (SDL_Renderer* gRenderer, TTF_Font* gFont, int point) {
    SDL_Color textColor = {255, 255, 255, 255}; // Lay mau trang
    string pointText = "Point: " + to_string(point); // Diem

    SDL_Surface* textSurface = TTF_RenderText_Solid (gFont, pointText.c_str(), textColor); // Tao surface voi font tren

    if (textSurface == NULL) {
        cout << "Failed to render text surface! SDL_ttf Error: " << TTF_GetError() << endl;
        return;
    }

    else {
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface (gRenderer, textSurface);

        if (textTexture == NULL) {
            cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << endl;
            SDL_FreeSurface(textSurface);
            return;
        }

        else {
            int textWidth = textSurface->w;
            int textHeight = textSurface->h;
            SDL_FreeSurface(textSurface);
    
            // Vị trí góc dưới bên phải
            SDL_Rect rect = {SCREEN_WIDTH - textWidth - 20, SCREEN_HEIGHT - textHeight - 20, textWidth, textHeight};
            SDL_RenderCopy(gRenderer, textTexture, NULL, &rect);
            SDL_DestroyTexture(textTexture);
        }
    }
}

void renderGameOver(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color textColor = {255, 0, 0, 255}; // Màu đỏ
    
    // Hiển thị dòng chữ "GAME OVER"
    SDL_Surface* gameOverSurface = TTF_RenderText_Solid(font, "GAME OVER", textColor);
    SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
    SDL_Rect gameOverRect = {
        SCREEN_WIDTH/2 - gameOverSurface->w/2,
        SCREEN_HEIGHT/2 - gameOverSurface->h - 20,
        gameOverSurface->w,
        gameOverSurface->h
    };
    SDL_FreeSurface(gameOverSurface);
    
    // Hiển thị hướng dẫn
    SDL_Surface* instructionSurface = TTF_RenderText_Solid(font, "Press R to try again or Q to quit", textColor);
    SDL_Texture* instructionTexture = SDL_CreateTextureFromSurface(renderer, instructionSurface);
    SDL_Rect instructionRect = {
        SCREEN_WIDTH/2 - instructionSurface->w/2,
        SCREEN_HEIGHT/2 + 20,
        instructionSurface->w,
        instructionSurface->h
    };
    SDL_FreeSurface(instructionSurface);
    
    // Vẽ lên màn hình
    SDL_RenderCopy(renderer, gameOverTexture, NULL, &gameOverRect);
    SDL_RenderCopy(renderer, instructionTexture, NULL, &instructionRect);
    
    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyTexture(instructionTexture);
}

void renderBuffText(SDL_Renderer* renderer, TTF_Font* font, const string& text, Uint32 startTime) {
    if (SDL_GetTicks() - startTime > buffTextDuration) {
        return;
    }

    // Font tren qua lon nen phai tao mot font ms nho hon
    TTF_Font* smallFont = TTF_OpenFont("Source/image/game.ttf", 16); // Font nhỏ hơn
    if (!smallFont) {
        smallFont = font; // Nếu không tải được thì dùng font gốc
    }

    SDL_Color textColor = {255, 255, 0, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(smallFont, text.c_str(), textColor);
    
    if (textSurface) {
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (textTexture) {
            SDL_Rect rect = {
                20,
                SCREEN_HEIGHT - 40,  
                textSurface->w,
                textSurface->h
            };
            
            Uint32 timeLeft = buffTextDuration - (SDL_GetTicks() - startTime);
            float alpha = (timeLeft > 500) ? 255 : (timeLeft * 255 / 500);
            SDL_SetTextureAlphaMod(textTexture, static_cast<Uint8>(alpha));
            
            SDL_RenderCopy(renderer, textTexture, NULL, &rect);
            SDL_DestroyTexture(textTexture);
        }
        SDL_FreeSurface(textSurface);
    }

    // Đóng font nhỏ nếu đã tạo
    if (smallFont != font) {
        TTF_CloseFont(smallFont);
    }
}

string getBuffName(buffType type) {
    switch (type) {
        case HEAL: return "HEAL";
        case SHIELD: return "SHIELD";
        case DAMAGE_BOOST: return "DAMAGE BOOST";
        case ATTACK_SPEED: return "ATTACK SPEED";
        case UP_MAX_HP: return "MAX HP UP";
        case REFLECT: return "REFLECT";
        case FREEZE: return "FREEZE";
        case SPEED_BOOST: return "SPEED BOOST";
        default: return "Buff does not exist!";
    }
}

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

    gFont = TTF_OpenFont("Source/image/game.ttf", 24);  // kiem tra ma font chu
    if (gFont == NULL) {
        cout << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << endl;
        success = false;
    }

    // Tải âm thanh va chạm
    gSound = Mix_LoadWAV("Source/image/bulletSound.wav");
    if (gSound == NULL) {
        cout << "Failed to load hit sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
        success = false;
    }
    else {
        Mix_VolumeChunk(gSound, 64);  // Đặt âm lượng 50%
    }
    
    return success;
}

void renderHealthBar(SDL_Renderer* renderer, int x, int y, int width, int height, float healthPercent, bool isBoss) {
    // Màu thanh máu (đỏ cho boss, xanh lá cho player)
    if (isBoss) {
        // Vẽ khung viền vang
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_Rect borderRect = {x - 2, y - 2, width + 4, height + 4};
        SDL_RenderDrawRect(renderer, &borderRect);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Đỏ
    } else {
        // Vẽ khung viền trắng
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect borderRect = {x - 2, y - 2, width + 4, height + 4};
        SDL_RenderDrawRect(renderer, &borderRect);
        
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Xanh lá
    }
    
    // Tính toán chiều rộng thanh máu dựa trên phần trăm máu
    int healthWidth = static_cast<int>(width * healthPercent);
    if (healthWidth < 0) healthWidth = 0;
    
    SDL_Rect healthRect = {x, y, healthWidth, height};
    SDL_RenderFillRect(renderer, &healthRect);
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

    lastSurvive = SDL_GetTicks();
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
    
    // Thong tin hien thi buff
    currentBuffText = getBuffName(buff.getType());
    buffTextStartTime = SDL_GetTicks();
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

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    TTF_CloseFont(gFont);
    gFont = NULL;
    gRenderer = NULL;
    gWindow = NULL;

    Mix_FreeChunk(gSound);
    gSound = NULL;
    
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
}

void resetGame() {
    playerPoint = 0;

    // Reset player
    spaceship = Player();
    if (!spaceship.loadTexture(gRenderer, "Source/image/DurrrSpaceShip.png")) {
        cout << "Failed to reload player texture!\n";
    }

    // Reset timers
    lastMeteorSpawn = 0;
    timeForNextSpawn = SDL_GetTicks() + (rand() % 30000 + 30000);
    // Reset buff text
    currentBuffText = "";
    buffTextStartTime = 0;

    // spawn lai enemy
    string s[] = {"Source/image/enemy1.png", "Source/image/enemy2.png", 
                 "Source/image/enemy3.png", "Source/image/enemy4.png", 
                 "Source/image/enemy5.png"};
    aliens.push_back(Enemy(0, 0, 7, 3));
    aliens.push_back(Enemy(145, 0, 7, 3));
    aliens.push_back(Enemy(390, 0, 7, 0));
    aliens.push_back(Enemy(515, 0, 7, 2));
    aliens.push_back(Enemy(725, 0, 7, 1));

    // Load textures cho enemy
    for (int i = 0; i < min((int)aliens.size(), 5); i++) {
        if (!aliens[i].loadTexture(gRenderer, s[i])) {
            cout << "Failed to reload enemy texture " << i << endl;
        }
    }
}

int main (int argc, char * argv[]) {
    //Start up SDL and create window
    if( !init() )
    {
        cout << "Failed to initialize!\n" ;
    }
    else
    {
        // Tao menu
        Menu menu;
        if (!menu.loadTexture(gRenderer, "Source/image/ARWBackground.png", "Source/image/ARWPlayButton.png")) {
            cout << "Failed to load menu media!\n";
            return -1;
        }

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


            // Trang thai cua cac phim
            bool keys[SDL_NUM_SCANCODES] = {false};

            while (!quit) {
                while (SDL_PollEvent (& e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }

                    if (menu.menuOrGame()) {
                        menu.handleEvent(e);
                    }

                    else {
                        if (e.type == SDL_KEYDOWN) { // Xu li khi an phim
                            keys[e.key.keysym.scancode] = true;
                        }

                        else if (e.type == SDL_KEYUP) {
                            // Xu li khi tha phim
                            keys[e.key.keysym.scancode] = false;
                        }
                    }
                }

                if (menu.menuOrGame()) {
                    // Hiển thị menu
                    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
                    SDL_RenderClear(gRenderer);
                    menu.render(gRenderer);
                    SDL_RenderPresent(gRenderer);
                }

                else {
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
                    if (eBullet.loadTexture(gRenderer, "Source/image/laserBullet.png") ) {
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

                if (!bosses.empty()) {
                    float bossHealthPercent = static_cast<float>(bosses[0].getHealth()) / bosses[0].getMaxHealth();
                    renderHealthBar(gRenderer, SCREEN_WIDTH/2 - 200, 20, 400, 20, bossHealthPercent, true);
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
                float playerHealthPercent = static_cast<float>(spaceship.getHealth()) / spaceship.getMaxHealth();
                renderHealthBar(gRenderer, SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT - 40, 200, 20, playerHealthPercent, false);

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

                            // Phát âm thanh khi đạn trúng enemy
                            Mix_PlayChannel(-1, gSound, 0); // -1 la kenh, 0 la so vong lap cua am thanh

                            if (!enemy.isAlive()) {
                                playerPoint += 5;
                                enemy.buffDrop(buffs);
                            }

                        }
                    }

                    for (auto& boss : bosses) {
                        SDL_Rect bulletRect = bullet.getRect();
                        SDL_Rect bossRect = boss.getRect();
                        if (checkCollision(bulletRect, bossRect)) {
                            boss.takeDamage(10, spaceship.getDamageIncrease()); // Quái vật bị tiêu diệt
                            bullet.isCollided(true); // Dan va cham voi quai

                            // Phát âm thanh khi đạn trúng boss
                            Mix_PlayChannel(-1, gSound, 0); // -1 la kenh, 0 la so vong lap cua am thanh

                            if (!boss.isAlive()) {
                                playerPoint += 20;
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

                                // Phát âm thanh khi đạn trúng enemy
                                Mix_PlayChannel(-1, gSound, 0); // -1 la kenh, 0 la so vong lap cua am thanh
                            }
                        }

                        for (auto& boss : bosses) {
                            SDL_Rect reflectRect = rBullet.getRect();
                            SDL_Rect bossRect = boss.getRect();
                            if (checkCollision(reflectRect, bossRect)) {
                                boss.takeDamage(10, spaceship.getDamageIncrease()); // Gây sát thương
                                rBullet.isCollided(true);

                                // Phát âm thanh khi đạn trúng enemy
                                Mix_PlayChannel(-1, gSound, 0); // -1 la kenh, 0 la so vong lap cua am thanh
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
                            spaceship.takeDamage(10); // Gây 10 sát thương
                            meteor.collision(true);
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


                // Neu so enemy < 5 thi spawn lai
                if (aliens.size() < 5 && SDL_GetTicks() - lastSurvive > reviveCooldown) {
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
                
                
                
                if (!spaceship.isAlive()) {
                    bool gameOver = true;

                    Mix_Chunk* endGame = NULL;

                    // Tải âm thanh va chạm
                    endGame = Mix_LoadWAV("Source/image/gameover.wav");
                    if (endGame == NULL) {
                        cout << "Failed to load hit sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
                    }
                    else {
                        Mix_VolumeChunk(endGame, 64);  // Đặt âm lượng 50%
                    }

                    Mix_PlayChannel (-1, endGame, 0);
                    
                    // Xoa toan bo ngay khi player die
                    storage.clear();
                    eStorage.clear();
                    bStorage0.clear();
                    bStorage1.clear();
                    bStorage2.clear();
                    bStorage3.clear();
                    reflectAttack.clear();
                    aliens.clear();
                    bosses.clear();
                    meteors.clear();
    
                    // Clear buffs
                    for (auto& buff : buffs) {
                        delete buff;
                    }
                    buffs.clear();
                
                    while (gameOver && !quit) {
                        while (SDL_PollEvent(&e) != 0) {
                            if (e.type == SDL_QUIT) {
                                quit = true;
                                gameOver = false;
                            }
                            else if (e.type == SDL_KEYDOWN) {
                                if (e.key.keysym.sym == SDLK_r) { // Press R to restart
                                    resetGame();
                                    endGame = NULL;
                                    gameOver = false;
                                }
                                else if (e.key.keysym.sym == SDLK_q) { // Press Q to quit
                                    quit = true;
                                    endGame = NULL;
                                    gameOver = false;
                                }
                            }
                        }
                
                        // Render black screen
                        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
                        SDL_RenderClear(gRenderer);
                
                        // Render game over text
                        renderGameOver(gRenderer, gFont);
                
                        // Render final score
                        string scoreText = "Final Score: " + to_string(playerPoint);
                        SDL_Color white = {255, 255, 255, 255};
                        SDL_Surface* scoreSurface = TTF_RenderText_Solid(gFont, scoreText.c_str(), white);
                        if (scoreSurface) {
                            SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(gRenderer, scoreSurface);
                            SDL_Rect scoreRect = {
                                SCREEN_WIDTH/2 - scoreSurface->w/2,
                                SCREEN_HEIGHT/2 + 60,
                                scoreSurface->w,
                                scoreSurface->h
                            };
                            SDL_RenderCopy(gRenderer, scoreTexture, NULL, &scoreRect);
                            SDL_FreeSurface(scoreSurface);
                            SDL_DestroyTexture(scoreTexture);
                        }
                
                        SDL_RenderPresent(gRenderer);
                        SDL_Delay(20); 
                    }
                }

                if (currentBuffText != "" && SDL_GetTicks() - buffTextStartTime <= buffTextDuration) {
                    renderBuffText(gRenderer, gFont, currentBuffText, buffTextStartTime);
                } else {
                    currentBuffText = ""; // Reset khi hết thời gian
                }

                // Render point
                renderScore (gRenderer, gFont, playerPoint);
                
                //Update screen
                SDL_RenderPresent( gRenderer );

                // Điều chỉnh tốc độ cập nhật
                SDL_Delay(20);
            }
            }
            
        }
    }
    close ();
    return 0;
}