#pragma once
#include <functional>
#include <queue>
#include <SDL.h>

struct Animation
{
	Animation() {}
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

	void EnqueueAnimation(Animation animation)
	{
		animationQueue.push(animation);
	}

	void PlayNextAnimation()
	{
		if (isAnimating || animationQueue.empty()) return;

		isAnimating = true;
		currentAnimation = animationQueue.front();
		animationQueue.pop();

		currentAnimation.Play();

		SDL_AddTimer(currentAnimation.duration, 
			[](Uint32 interval, void* animManager) -> Uint32 {
			AnimationManager::GetInstance().SetAnimatingState(false);
			AnimationManager::GetInstance().PlayCurrentEnd();
			AnimationManager::GetInstance().PlayNextAnimation();
			return 0;
			}, nullptr);
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