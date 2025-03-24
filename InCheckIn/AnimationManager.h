#pragma once
#include <functional>
#include <queue>
#include <SDL.h>

class AnimationManager
{
public:
	static AnimationManager& GetInstance()
	{
		static AnimationManager animManager;
		return animManager;
	}

	void EnqueueAnimation(std::function<void()> animation)
	{
		animationQueue.push(animation);
	}

	void PlayNextAnimation()
	{
		if (isAnimating || animationQueue.empty()) return;

		isAnimating = true;
		auto animation = animationQueue.front();
		animationQueue.pop();

		animation();

		SDL_AddTimer(500, [](Uint32 interval, void* animManager) -> Uint32 {
			AnimationManager::GetInstance().SetAnimatingState(false);
			AnimationManager::GetInstance().PlayNextAnimation();
			return 0;
			}, nullptr);
	}

	void SetAnimatingState(bool isAnim) { isAnimating = isAnim; }

private:
	AnimationManager() {}
	~AnimationManager() {}

	std::queue<std::function<void()>> animationQueue;
	bool isAnimating = false;
};