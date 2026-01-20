// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatureAction_WorldActionBase.h"

void UGameFeatureAction_WorldActionBase::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	// ExperienceManagerComponent 에서 GameFeatureAction 을 활성화 하면서, Context 에 World 를 넣음
	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
	{
		// WorldContext 들 중에 현재 적용하고 싶은 Feature 가 있는 World 와 일치할 경우, AddToWorld 를 실행
		if (Context.ShouldApplyToWorldContext(WorldContext))
		{
			AddToWorld(WorldContext, Context);
		}
	}
}
