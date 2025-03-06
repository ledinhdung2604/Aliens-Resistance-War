#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <string>

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 506.25;

class Player {
    public:
    // Constructor and Destructor
    Player ();
    ~Player ();

    // Tải texture cho player
    bool loadTexture(SDL_Renderer* gRenderer, std::string path);

    // Giải phóng texture và đặt lại các biến thành viên.
    void free ();

    // Xử lý sự kiện bàn phím
    void handleEvent(SDL_Event& e);

    // Cập nhật vị trí của player
    void update();

    // Vẽ player lên màn hình
    void render(SDL_Renderer* gRenderer);

    // Lấy vị trí và kích thước của player
    SDL_Rect getPosition() ;

    // Lay toa do x cua player
    int getX();

    // Lay toa do y cua player
    int getY();

    // Bắn đạn
    void shoot();

    // Kiểm tra va chạm với đạn hoặc enemy
    bool checkCollision(const SDL_Rect& other) ;

    // Giảm máu khi bị trúng đạn
    void takeDamage(int damage);

    // Kiểm tra player còn sống hay không
    bool isAlive() const;

private:
    // Vị trí và kích thước của player
    SDL_Rect position = {SCREEN_WIDTH/2 - 20, SCREEN_HEIGHT - 40, 40, 40};

    // Texture của player
    SDL_Texture* texture;

    // Tốc độ di chuyển của player
    int speed;

    // Máu của player
    int health;

    // Trạng thái sống/chết của player
    bool alive;
};

#endif // Đảm bảo file Player.h chỉ được include một lần, tránh lỗi định nghĩa trùng lặp.