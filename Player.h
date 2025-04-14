#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <string>
#include "Buff.h"
#include <vector>
#include <algorithm>

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 506.25;

// Trạng thái hoạt động của buff
struct buffWork {
    buffType type;
    Uint32 activateTime;
    Uint32 buffLength;
};

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
    void handleEvent(bool keys[]);

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

    // Giảm máu khi bị trúng đạn
    void takeDamage(int damage);

    // Kiểm tra player còn sống hay không
    bool isAlive();

    // Áp dụng buff lên player
    void applyBuff (buffType type, Uint32 buffLength);

    // Thời gian hoạt động của buff trên player
    void updateBuffs ();

    // Lấy độ tăng vận tốc
    double getSpeedIncrease ();

    // Lay trang thai phan dan
    bool isReflect();

    // Lay damage nhan them cua player
    double getDamageIncrease ();

    // lay attack speed cua player
    double getAttackSpeedUp ();

    vector <buffWork> getActiveBuffs ();

    // Lay toc do khi black hole xuat hien
    void setGravitySpeed (bool affect);

    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }

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


    vector <buffWork> activeBuffs;
    double damageIncrease;
    double attackSpeedUp;
    double speedIncrease;
    bool shieldActivate;
    bool reflectActivate;
    int maxHealth;
    double gravityFactor;
};

#endif // Đảm bảo file Player.h chỉ được include một lần, tránh lỗi định nghĩa trùng lặp.