#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

using namespace std;

class Menu {
    public:
    Menu ();
    ~Menu ();

    bool loadTexture (SDL_Renderer* gRenderer, string backPath, string playPath);
    void free();
    void render (SDL_Renderer* gRenderer);
    bool handleEvent (SDL_Event &e); // Xu li su kien bam chuot vao man hinh
    bool menuOrGame ();  // Kiem tra xem dang o man hinh menu hoac game
    void gameStart ();   // thay doi de game bat dau
    bool checkButtonClick (int x, int y);  // Kiem tra an nut vao play button chua

    private:
    SDL_Texture* backgroundTexture;
    SDL_Texture* buttonTexture;
    SDL_Rect playRect;
    bool inMenu;
};

#endif