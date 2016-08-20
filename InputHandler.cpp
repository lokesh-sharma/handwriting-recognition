#include "InputHandler.h"
#include"Game.h"

InputHandler* InputHandler::pInstance = 0;

void InputHandler::update()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT:
            TheGame::getInstance()->quit();
            break;
        case SDL_MOUSEBUTTONUP:
            onMouseButtonUp(event);
            break;
        case SDL_MOUSEBUTTONDOWN:
            onMouseButtonDown(event);
            break;
        case SDL_MOUSEMOTION:
            onMouseMove(event);
        case SDL_KEYDOWN:
            onKeyDown();
            break;
        case SDL_KEYUP:
            onKeyUp();
            break;
        case SDL_FINGERDOWN:
            m_FingerDown = true;
            break;
        case SDL_FINGERUP:
            m_FingerDown = false;
            break;
        case SDL_FINGERMOTION:
            onDrag(event);
            break;
        default : break;

        }

    }

}
bool InputHandler::isKeyDown(SDL_Scancode key)
{
    if(m_keystate!= 0)
    {
        if(m_keystate[key]==1)
            return true;
        else return false;
    }
    return false;
}
void InputHandler::onKeyDown()
{
     m_keystate = SDL_GetKeyboardState(0);
}
void InputHandler::onKeyUp()
{
     m_keystate = SDL_GetKeyboardState(0);
}
void InputHandler::onMouseButtonDown(SDL_Event& event)
{
        if(event.button.button==SDL_BUTTON_LEFT)
                m_mouseButtonStates[LEFT]=true;
        if(event.button.button==SDL_BUTTON_RIGHT)
                m_mouseButtonStates[RIGHT]=true;
        if(event.button.button==SDL_BUTTON_MIDDLE)
                m_mouseButtonStates[MIDDLE]=true;
}
void InputHandler::onMouseButtonUp(SDL_Event& event)
{
        if(event.button.button==SDL_BUTTON_LEFT)
                m_mouseButtonStates[LEFT]=false;
        if(event.button.button==SDL_BUTTON_RIGHT)
                m_mouseButtonStates[RIGHT]=false;
        if(event.button.button==SDL_BUTTON_MIDDLE)
                m_mouseButtonStates[MIDDLE]=false;
}
void InputHandler::onMouseMove(SDL_Event& event)
{
        m_mousePosition->setX(event.motion.x);
        m_mousePosition->setY(event.motion.y);
}
void InputHandler::onDrag(SDL_Event& event)
{
        m_touchPosition->setX(event.tfinger.x);
        m_touchPosition->setX(event.tfinger.y);
}
