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

    // Tra ve trang thai vien dan
    bool isOutofBorder () ;

    private:
    SDL_Texture* texture;
    SDL_Rect rect; // Vị trí và kích thước đạn
    int speed;
};

#endif