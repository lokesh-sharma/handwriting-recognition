#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
#include<SDL2/SDL.h>
#include<vector>
#include"Vector2D.h"

enum mouse_buttons
{
    LEFT=0,
    MIDDLE=1,
    RIGHT=2
};

class InputHandler
{
private:
    static InputHandler* pInstance;
    Vector2D* m_mousePosition;
    Vector2D* m_touchPosition;
    const Uint8* m_keystate;
    std::vector<bool>m_mouseButtonStates;
    bool m_FingerDown;
    InputHandler() : m_keystate(NULL)
    {
        m_FingerDown = false;
        for(int i = 0 ; i<3 ; i++)
            m_mouseButtonStates.push_back(false);
        m_mousePosition = new Vector2D(0 , 0);
        m_touchPosition = new Vector2D(0,0);

    }
    void onKeyDown();
    void onKeyUp();

    void onMouseMove(SDL_Event& event);
    void onMouseButtonDown(SDL_Event& event);
    void onMouseButtonUp(SDL_Event& event);
    void onDrag(SDL_Event& event);

public:
    static InputHandler* getInstance()
    {
        if(pInstance==0)
        {
            pInstance = new InputHandler();
            return pInstance;
        }
        else return pInstance;
    }
    void update();

    bool isKeyDown(SDL_Scancode key);
    void clean() {};
    bool getMouseButtonState(int button)
     { return m_mouseButtonStates[button];}
    bool getTouchState() { return m_FingerDown ; }
    Vector2D* getMousePosition()
    {
        return m_mousePosition;
    }
    Vector2D* getTouchPosition()
    {
        return m_touchPosition;
    }
};
typedef InputHandler TheInputHandler;
#endif // INPUTHANDLER_H
