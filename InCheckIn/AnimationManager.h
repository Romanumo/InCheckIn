#pragma once
#include <functional>
#include <queue>
#include <SDL.h>

inline Uint32 ANIM_EVENT = (Uint32)-1;

struct Animation
{
	Animation() { }

	Animation(int duration) 
	{
		onPlayed = nullptr;
		onEnd = nullptr;
		this->duration = duration;
	}

	Animation(std::function<void()> play, std::function<void()> end, int duration)
	{
		onPlayed = play;
		onEnd = end;
		this->duration = duration;
	}

	void Play()
	{
		if (onPlayed) onPlayed();
	}

	void EndAnimation()
	{
		if (onEnd) onEnd();
	}

	bool IsEmpty() const { return !onPlayed && !onEnd && duration < 0; }

	std::function<void()> onPlayed;
	std::function<void()> onEnd;
	int duration;
};

class AnimationManager
{
public:
	static AnimationManager& GetInstance()
	{
		static AnimationManager animManager;
		return animManager;
	}

	void PlayDelayedAnimation(std::function<void()> onPlay, int delay)
	{
		auto* heapFunc = new std::function<void()>(std::move(onPlay));

		SDL_TimerID id = SDL_AddTimer(delay, [](Uint32, void* p) -> Uint32 {
			SDL_Event e{};
			e.type = ANIM_EVENT;
			e.user.data1 = p;
			if (SDL_PushEvent(&e) <= 0) 
				delete static_cast<std::function<void()>*>(p);
			return 0;                            
		}, heapFunc);

		if (id == 0) delete heapFunc;
	}

	void EnqueueAnimation(Animation animation)
	{
		animationQueue.push(animation);
	}

	void EnqueuePause(int duration)
	{
		animationQueue.push(Animation(duration));
	}

	void PlayNextAnimation()
	{
		if (isAnimating || animationQueue.empty()) return;

		isAnimating = true;
		currentAnimation = animationQueue.front();
		animationQueue.pop();

		currentAnimation.Play();
	}

	void Update(int ms)
	{
		if (!currentAnimation.IsEmpty())
		{
			currentAnimation.duration -= ms;
			if (currentAnimation.duration > 0) return;

			AnimationManager::GetInstance().SetAnimatingState(false);
			AnimationManager::GetInstance().PlayCurrentEnd();
			AnimationManager::GetInstance().PlayNextAnimation();
		}
	}

	void SetAnimatingState(bool isAnim) { isAnimating = isAnim; }

private:
	AnimationManager() {}
	~AnimationManager() {}

	std::queue<Animation> animationQueue;
	Animation currentAnimation;
	bool isAnimating = false;

	void PlayCurrentEnd()
	{
		currentAnimation.EndAnimation();
	}
};