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
// khai bao mot chuoi enenmy
vector <Enemy> aliens;

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
    if (!spaceship.loadTexture(gRenderer, "Source/image/DurrrSpaceShip.png")) {
        cout << "Failed to load texture!\n";
        success = false;
    }

    if (!universe.loadTexture(gRenderer, "Source/image/back.png")) {
        cout << "Failed to load texture!\n";
        success = false;
    }

    if (!attack.loadTexture(gRenderer, "Source/image/laserBullet.png")) {
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

void close () {
    spaceship.free ();
    universe.free();
    attack.free();
    for (auto& enemy : aliens) {
        enemy.free();
    }

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
            
            string s[] = {"Source/image/enemy1.png", "Source/image/enemy2.png", "Source/image/enemy3.png", "Source/image/enemy4.png", "Source/image/enemy5.png"};

            aliens.push_back(Enemy(0, 0, 7, 2));
            aliens.push_back(Enemy(145, 0, 7, 1));
            aliens.push_back(Enemy(390, 0, 7, 0));
            aliens.push_back(Enemy(515, 0, 7, 2));
            aliens.push_back(Enemy(725, 0, 7, 1));
            
            vector <Bullet> storage;

            bool quit = false;

            SDL_Event e;
            
            Uint32 previousBullet = 0; // thoi diem ban vien dan truoc
            const Uint32 cooldown = 200; // Khoang thoi gian cho giua cac lan ban la 200 ms

            while (!quit) {
                while (SDL_PollEvent (& e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }

                    else if (e.type == SDL_KEYDOWN) {
                        spaceship.handleEvent(e);
                        spaceship.update();
                        if (e.key.keysym.sym == SDLK_SPACE) {
                            Uint32 currentBullet = SDL_GetTicks (); // Lay thoi gian cua vien dan hien tai
                            if (currentBullet - previousBullet > cooldown) {
                            Bullet newatk = attack;
                            newatk.initialize (spaceship.getX() + middle, spaceship.getY());
                            storage.push_back (newatk);
                            previousBullet = currentBullet;}
                        }
                    }
                }
                

                //Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF);
                SDL_RenderClear( gRenderer );

                // Render background
                universe.render(gRenderer);

                // Render enemy
                for (int i = 0; i < (int)aliens.size(); i++) {
                    if (aliens[i].loadTexture(gRenderer, s[i])) {
                    aliens[i].update (SCREEN_WIDTH, SCREEN_HEIGHT);
                    aliens[i].render (gRenderer);}
                    
                }

                // Render spaceship
                spaceship.render(gRenderer);
                
                // Cập nhật và render đạn
                for (auto& bullet : storage) {
                    bullet.update();
                    bullet.render(gRenderer);
                    
                    for (auto& enemy : aliens) {
                        SDL_Rect bulletRect = bullet.getRect();
                        SDL_Rect enemyRect = enemy.getRect();
                        if (enemy.isAlive() && checkCollision(bulletRect, enemyRect)) {
                            enemy.setAlive(false); // Quái vật bị tiêu diệt
                            bullet.isCollided(true); // Dan va cham voi quai
                        }
                    }
                    
                }

                // xoa dan da ra khoi man hinh
                storage.erase(remove_if(storage.begin(), storage.end(), [](Bullet& bullet) {
                    return bullet.isOutofBorder() || bullet.collision(); // Kiểm tra đạn có ra khỏi màn hình không
                }), storage.end());

                aliens.erase(std::remove_if(aliens.begin(), aliens.end(), [](const Enemy& enemy) {
                    return !enemy.isAlive();
                }), aliens.end());
                
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