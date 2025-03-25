#ifndef ENEMYBULLET_H
#define ENEMYBULLET_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

using namespace std;

class EnemyBullet {
    public:
    
    EnemyBullet ();
    ~EnemyBullet ();

    // Tải texture từ file có hình ảnh của đạn
    bool loadTexture (SDL_Renderer* gRenderer, string path);

    void free ();

    // Khởi tạo đạn tại vị trí của spaceship
    void initialize (int x, int y) ;
    
    // Cập nhật vị trí của đạn
    void update ();

    // Vẽ đạn lên màn hình
    void render (SDL_Renderer* gRenderer);

    // Lay hinh chu nhat cua dan
    SDL_Rect getRect ()  ;

    // Tra ve trang thai vien dan
    bool isOutofBorder (int SCREEN_HEIGHT) ;

    // Kiem tra xem dan co cham vao enemy hay khong
    void isCollided (bool _collide) ;

    // Tra ve collide 
    bool collision ();

    private:
    SDL_Texture* texture;
    SDL_Rect rect; // Vị trí và kích thước đạn
    int speed;
    bool collide;
};

#endif