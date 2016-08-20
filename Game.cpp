#include "Game.h"
#include<iostream>
#include<SDL_image.h>
#include<SDL2/SDL.h>
#include"InputHandler.h"
#include"Vector2D.h"
#include<ctime>
#include<cstring>

Game* Game::pInstance = 0 ;
bool Game::init(const char* title , int xpos , int ypos , int height,
           int width ,int flags)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) >=0 )
    {
        m_pWindow = SDL_CreateWindow(title,xpos, ypos , height , width
                ,flags);
        if(!m_pWindow==0){
            m_pRenderer = SDL_CreateRenderer(m_pWindow , -1,0);
           if(!m_pRenderer==0)
             {
                show_initial_instruction();
                ptexture = SDL_CreateTexture(m_pRenderer , SDL_PIXELFORMAT_ARGB8888,
                                                SDL_TEXTUREACCESS_STATIC,740 , 200);
                pixels = new Uint32[width*height];
                memset(pixels , 200 , width*height*sizeof(Uint32));

             }
           else {std::cerr<<"renderer creation failed\n";return false;}
        }
        else {std::cerr<<"window creation failed\n";return false;}
        m_bRunning = true;
        return true;
    }
    else return false;
}
void Game::render()
{
    SDL_RenderClear(m_pRenderer);

    SDL_RenderCopy(m_pRenderer , ptexture , NULL  , NULL);
    if(TheInputHandler::getInstance()->isKeyDown(SDL_SCANCODE_SPACE)
       && space_button2 == false){
        space_button2 = true;
       for(int i = 0 ; i<alphabets.size() ; i++)
          std::cout<<alphabets[i]->check_alphabet();
        std::cout<<" ";

       for(int i = 0 ; i< alphabets.size() ; i++)
         delete alphabets[i];
       alphabets.clear();

       memset(pixels , 200 , width*height*sizeof(Uint32));

       for(int i = 0 ; i< width ; i++){
            for(int j = 0 ; j< height ; j++)
            positions[i][j] = false;
       }

    }
    if(!TheInputHandler::getInstance()->isKeyDown(SDL_SCANCODE_SPACE))
        space_button2 = false;

    SDL_RenderPresent(m_pRenderer);
}
void Game::clean()
{
    //std::cerr<<"cleaning the game\n";
    SDL_DestroyWindow(m_pWindow);
    SDL_DestroyRenderer(m_pRenderer);
    SDL_DestroyTexture(ptexture);
    alphabets.clear();
    delete[] pixels;
    SDL_Quit();
}
void Game::handleEvents()
{
    TheInputHandler::getInstance()->update();
}
void Game::update()
{

    static Vector2D m_pos0 = *(TheInputHandler::getInstance()->getMousePosition());
    static Vector2D t_pos0 = *(TheInputHandler::getInstance()->getTouchPosition());
    Vector2D m_pos = *(TheInputHandler::getInstance()->getMousePosition());
    Vector2D t_pos = *(TheInputHandler::getInstance()->getTouchPosition());
    if(TheInputHandler::getInstance()->getMouseButtonState(LEFT))
    {
       Vector2D result = m_pos - m_pos0;
          float len = 0.5;
          float cos0 = result.getX()/result.length();
          float sin0 = result.getY()/result.length();
          while(len < result.length())
          {
              positions[m_pos0.getX()+len*cos0][m_pos0.getY()+len*sin0] = true;
              pixels[int(m_pos0.getY()+len*sin0)*width +
              int(m_pos0.getX()+len*cos0)] = 0;
              len++;
          }
    }
    m_pos0 = m_pos;
     if(TheInputHandler::getInstance()->getTouchState())
    {
       Vector2D result = t_pos - t_pos0;
          float len = 0.5;
          float cos0 = result.getX()/result.length();
          float sin0 = result.getY()/result.length();
          while(len < result.length())
          {
              positions[t_pos0.getX()+len*cos0][t_pos0.getY()+len*sin0] = true;
              pixels[int(t_pos0.getY()+len*sin0)*width +
              int(t_pos0.getX()+len*cos0)] = 0;
              len++;
          }
    }
    if(TheInputHandler::getInstance()->isKeyDown(SDL_SCANCODE_SPACE)
        && space_button == false){
        space_button = true;
        Alphabets* alp1 = new Alphabets(positions , 0);
        while(alp1->get_ending()!= -1 )
        {
            alphabets.push_back(alp1);
            alp1 = new Alphabets(positions , alp1->get_ending());

        }
    }
    if(!TheInputHandler::getInstance()->isKeyDown(SDL_SCANCODE_SPACE))
        space_button = false;
    clock_t endd = clock();

    SDL_UpdateTexture(ptexture , NULL , pixels, width*(sizeof (Uint32)) );

}
void Game::quit()
{
    m_bRunning = false;
}
std::vector<int> Game::intersect_y(int x , int y1 , int y2)
{
    std::vector<int> intersections;
    for(int j = y1 ; j< y2 ; j++)
    {
        if(positions[x][j]==true)
            intersections.push_back(j);
    }
    return intersections;
}
void Game::show_initial_instruction()
{
    std::cerr<<"Enter letters on writing pad with sufficient space\n";
    std::cerr<<"So that they do not overlap\n";
    std::cerr<<"the word you write will be displayed on screen\n";
    std::cerr<<"by default but you can redirect it to text file\n";
    std::cerr<<"by using redirection when starting the program\n";
    std::cerr<<"\nPress 'space' to enter new word\n\n";

}
