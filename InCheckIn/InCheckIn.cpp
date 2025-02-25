#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Engine/Managers/Window.h"
#include "Engine/Managers/SoundManager.h"
#include "Engine/Managers/Globals.h"
#include "Engine/GameObject.h"
#include "Engine/UIFactory.h"
#include "Card.h"

using namespace std;

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
#ifdef SHOW_DEBUG_HELPERS
    Utils::CheckSDLErrors("SDL_Init");
#endif

    IMG_Init(IMG_INIT_PNG);
#ifdef SHOW_DEBUG_HELPERS
    Utils::CheckSDLErrors("IMG_Init");
#endif

    TTF_Init();
#ifdef SHOW_DEBUG_HELPERS
    Utils::CheckSDLErrors("TTF_Init");
#endif 

    SoundManager::OpenAudio();

    Engine::Window window;

    SDL_Event event;
    bool shouldQuit = false;

    //SoundManager::GetInstance().PlayMusic(Config::BCG_MUSIC);

    std::vector<Engine::GameObject*> cards;
    for (int i = 0;i < 5;i++)
    {
        cards.push_back(new Card(0,0));
    }
    Engine::GameObject* hand = Engine::UIFactory::GetRow(200, 300, cards);

    while (!shouldQuit)
    {
        //Handle Input
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                shouldQuit = true;
            }
            hand->HandleEvent(event);
        }

        //Update Objects

        //Render Frame (DOnt forget to put render after window render)
        window.Render();
        hand->Render(window.GetSurface());
        window.UpdateFrame();
    }

    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}