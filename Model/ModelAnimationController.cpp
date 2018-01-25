#include "../stdafx.h"
#include "ModelAnimationController.h"
#include "ModelAnimation.h"

ModelAnimationController::ModelAnimationController()
{
	currentAnimation = NULL;
	currentKeyFrame = 0;
	nextKeyFrame = 0;
	keyFrameFactor = 0.0f;
	frameTimer = 0.0f;

	useQuaternionKeyFrames = true;

	animationMode = AnimationMode::Stop;
}

ModelAnimationController::~ModelAnimationController()
{
	for each(Pair temp in animations)
		SAFE_DELETE(temp.second);
	
	currentAnimation = NULL;
}

void ModelAnimationController::AddAnimation(ModelAnimation * animation)
{
	bool isExist = false;
	for (size_t i = 0; i < animations.size(); i++)
	{
		if (animations[i].first == animation->GetName())
		{
			isExist = true;
			
			break;
		}
	}
	assert(isExist == false);

	animations.push_back(Pair(animation->GetName(), animation));
}

ModelAnimation * ModelAnimationController::GetCurrentAnimation()
{
	if(currentAnimation != NULL)
		return currentAnimation;

	return NULL;
}

void ModelAnimationController::SetCurrentAnimation(wstring animationName)
{
	UINT count = -1;
	int temp = animations.size(); // 1
	for (size_t i = 0; i < animations.size(); i++)
	{
		if (animations[i].first == animationName)
		{
			count = i;
			break;
		}
	}
	
	if (count != -1)
		currentAnimation = animations[count].second;
	else
		currentAnimation = NULL;

	Stop();
}

void ModelAnimationController::SetCurrentAnimation(int index)
{
	currentAnimation = animations[index].second;
	
	Stop();
}

UINT ModelAnimationController::GetAnimationCount()
{
	return animations.size();
}

ModelAnimation * ModelAnimationController::GetAnimation(int index)
{
	return animations[index].second;
}

void ModelAnimationController::Play()
{
	animationMode = AnimationMode::Play;
}

void ModelAnimationController::Pause()
{
	animationMode = AnimationMode::Pause;
}

void ModelAnimationController::Stop()
{
	animationMode = AnimationMode::Stop;
	frameTimer = 0.0f;

	SetCurrentKeyFrame(0);
}

void ModelAnimationController::SetCurrentKeyFrame(int keyFrame)
{
	currentKeyFrame = (keyFrame) % currentAnimation->GetKeyFrames();
	nextKeyFrame = (currentKeyFrame + 1) % currentAnimation->GetKeyFrames();
	
	keyFrameFactor = 0.0f;
}

void ModelAnimationController::Update()
{
	if (currentAnimation == NULL || animationMode != AnimationMode::Play)
		return;

	/*
		frameTimer : 애니메이션 총 구동 시간 (???)
		invFrameRate : 현재 동작하는 애니메이션의 구동 시간 (???)
		currentKeyFrame : 현재 프레임 구간, 매 업데이트시 1씩 증가함
		nextKeyFrame : 다음 키프레임 구간
	*/
	frameTimer += Frames::TimeElapsed();

	float invFrameRate = 1.0f / currentAnimation->GetFrameRate();
	while (frameTimer > invFrameRate)
	{
		int test = currentAnimation->GetKeyFrames();
		currentKeyFrame = (currentKeyFrame + 1) % currentAnimation->GetKeyFrames();
		nextKeyFrame = (currentKeyFrame + 1) % currentAnimation->GetKeyFrames();
		
		frameTimer -= invFrameRate;
	}
	//TODO currentKeyFrame이 0일때 idle 상태로

	keyFrameFactor = frameTimer / invFrameRate;
	int a = 0;
}
