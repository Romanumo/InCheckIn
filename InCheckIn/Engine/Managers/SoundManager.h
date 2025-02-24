#pragma once
#include <SDL_mixer.h>
#include <string>
#include <iostream>
#include <unordered_map>
#include "ResourceManager.h"

class SoundManager {
public:
    static SoundManager& GetInstance() {
        static SoundManager instance;
        return instance;
    }

    static void OpenAudio()
    {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
            std::cout << "SDL_mixer could not initialize! SDL_mixer Error: %s\n" << Mix_GetError() << std::endl;
        }
    }

    void PlayMusic(const std::string& path, int loops = -1) 
    {
        if (currentMusic) Mix_HaltMusic();

        currentMusic = ResourceManager<Mix_Music>::GetInstance().GetByPath(path, LoadMusic).get();
        if (currentMusic) Mix_PlayMusic(currentMusic, loops);
    }

    void PlaySFX(const std::string& path) 
    {
        Mix_Chunk* sound = ResourceManager<Mix_Chunk>::GetInstance().GetByPath(path, LoadSFX).get();
        Mix_PlayChannel(-1, sound, 0);
    }

    void SetMusicVolume(int volume) { Mix_VolumeMusic(volume); }
    void SetSFXVolume(int volume) { Mix_Volume(-1, volume); }

private:
    SoundManager() { Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048); }
    ~SoundManager() { Mix_CloseAudio(); }
    Mix_Music* currentMusic = nullptr;

    static std::shared_ptr<Mix_Music> LoadMusic(const std::string& path)
    {
        Mix_Music* music = Mix_LoadMUS(path.c_str());
        if (!music)
        {
            std::cout << "Failed Loading Music. Path: " << path << std::endl;

            return nullptr;
        }
        return std::shared_ptr<Mix_Music>(music, Mix_FreeMusic);
    }

    static std::shared_ptr<Mix_Chunk> LoadSFX(const std::string& path)
    {
        Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
        if (!sound)
        {
            std::cout << "Failed Loading Sound. Path: " << path << std::endl;

            return nullptr;
        }
        return std::shared_ptr<Mix_Chunk>(sound, Mix_FreeChunk);
    }
};
