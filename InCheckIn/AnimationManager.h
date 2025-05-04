#pragma once
#include <functional>
#include <queue>
#include <SDL.h>

struct Animation
{
	Animation() {}

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

	void PlaySingleAnimation(Animation animation)
	{
		Animation* anim = &animation;

		SDL_AddTimer(1,
			[](Uint32 interval, void* animation) -> Uint32 {
				Animation* anim = static_cast<Animation*>(animation);
				anim->Play();

				SDL_AddTimer(anim->duration, 
					[](Uint32 interval, void* animation) -> Uint32 {
						static_cast<Animation*>(animation)->onEnd();
						delete animation;
						return 0;
					}, &animation);

				return 0;
			}, anim);
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