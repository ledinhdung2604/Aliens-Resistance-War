#ifndef BULLET_H
#define BULLET_H

#include <SDL2/SDL.h>
#include <string>



class Bullet {
    public:
    Bullet ();
    ~Bullet ();

    // Tải texture từ file có hình ảnh của đạn
    bool loadTexture (SDL_Renderer* gRenderer, std::string path);

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
    bool isOutofBorder () ;

    // Kiem tra xem dan co cham vao enemy hay khong
    void isCollided (bool _collide) ;

    // Tra ve collide 
    bool collision () ;

    private:
    SDL_Texture* texture;
    SDL_Rect rect; // Vị trí và kích thước đạn
    int speed;
    bool collide;
};

#endif