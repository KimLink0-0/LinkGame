// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkUserFacingExperience.h"

#include "CommonSessionSubsystem.h"

UCommonSession_HostSessionRequest* ULinkUserFacingExperience::CreateHostingRequest() const
{
	const FString ExperienceName = ExperienceID.PrimaryAssetName.ToString();
	
	// UCommonSession_HostSessionRequest 는 UObject
	// GC 의 대상이 된다. 함수 안에서 NewObject 로 생성했기 때문에 함수가 끝나면 GC 에 의해 정리될 수 있있으므로
	// Dangling 포인터 유무를 확인해야 한다.
	// 해당 객체 (Result) 는 현재 프레임 안에서 사용하기 때문에 GC 가 정리하기 전에 사용되고 사라질 예정
	// 그러므로 Dangling 을 걱정하지 않아도 된다.
	// 나중에 사용하려면 어딘가에 캐싱해두고 사용할 것 
	UCommonSession_HostSessionRequest* Result = NewObject<UCommonSession_HostSessionRequest>();
	Result->MapID = MapID;
	Result->ExtraArgs.Add(TEXT("Experience"), ExperienceName);
	
	return Result;
}
