#pragma once

#include "Entity.h"

#include "AnimationSkeleton.h"
#include "AnimationState.h"

namespace Bat
{
	class AnimationComponent : public Component<AnimationComponent>
	{
	public:
		SkeletonPose bind_pose;
		SkeletonPose current_pose;
		std::vector<AnimationState> states;
		std::vector<BoneData> bones;
		std::vector<AnimationClip> clips;
	};
}