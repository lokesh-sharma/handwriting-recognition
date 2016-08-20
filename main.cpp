#include<SDL2/SDL.h>
#include"Game.h"


int main(int argc , char* argv[])
{
    long framestart , frametime;
    TheGame::getInstance()->init("writing pad" , SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,TheGame::getInstance()->get_width() ,
            TheGame::getInstance()->get_height() , SDL_WINDOW_MAXIMIZED);
    while(TheGame::getInstance()->running())
    {
        //framestart = SDL_GetTicks();
        TheGame::getInstance()->handleEvents();
        TheGame::getInstance()->update();
        TheGame::getInstance()->render();
        //frametime = SDL_GetTicks()- framestart;
        //if(frametime< DELAY_TIME)
        //SDL_Delay(DELAY_TIME-frametime);
    }
    TheGame::getInstance()->clean();
    return 0;

}
