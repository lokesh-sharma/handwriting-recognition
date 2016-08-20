#ifndef GAME_H
#define GAME_H
#include<SDL2/SDL.h>
#include<vector>
#include"Alphabets.h"

class Game
{
private:
    bool m_bRunning;
    bool space_button;
    bool space_button2;
    int height ;
    int width;
    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;
    SDL_Texture* ptexture;
    Uint32* pixels ;
    std::vector<Alphabets*> alphabets;
    static Game* pInstance;

    std::vector<std::vector<bool> > positions;
    Game(){
        height = 200;
        width = 740;
        space_button = false;
        space_button2 = false;
        for(int i = 0 ; i<width ; i++)
        {
            positions.push_back( std::vector<bool>(height , false));
        }
    }
public:
    static Game* getInstance()
    {
        if(pInstance==0)
        {
            pInstance = new Game();
            return pInstance;
        }
        else return pInstance;
    }
    bool init(const char* title , int xpos , int ypos , int height,
              int width ,int flags);
    void handleEvents();
    void clean();
    void quit();
    void update();
    void render();
    void show_initial_instruction();
    int get_width() const {return width; }
    int get_height() const {return height; }
    std::vector<int> intersect_y(int x , int y1 , int y2);
    SDL_Renderer* getRenderer() { return m_pRenderer;}
    bool running(){ return m_bRunning;}

};
typedef Game TheGame;
#endif // GAME_H
