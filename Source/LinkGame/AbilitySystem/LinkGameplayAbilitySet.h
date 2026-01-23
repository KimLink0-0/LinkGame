// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySet.h"
#include "LinkGameplayAbilitySet.generated.h"

class ULinkAbilitySystemComponent;
class ULinkGameplayAbility;

// Gameplay Ability 의 Wrapper 클래스
// 추가적인 커스터마이징 가능
USTRUCT(BlueprintType)
struct FLinkAbilitySet_GameplayAbility
{
	GENERATED_BODY()
public:
	/* Ability Core Section */
	// Ability
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULinkGameplayAbility> Ability = nullptr;
	// Input 처리를 위한 Tag
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;
	
	/* Custom Section */
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;
};

USTRUCT(BlueprintType)
struct FLinkAbilitySet_GrantedHandles
{
	GENERATED_BODY()
	
public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void TakeFromAbilitySystem(ULinkAbilitySystemComponent* InAbilitySystemComponent);
	
protected:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
};


UCLASS()
class LINKGAME_API ULinkGameplayAbilitySet : public UGameplayAbilitySet
{
	GENERATED_BODY()
public:
	ULinkGameplayAbilitySet(const FObjectInitializer& Initializer = FObjectInitializer::Get());
	
	void GiveToAbilitySystem(ULinkAbilitySystemComponent* AbilitySystemComponent, FLinkAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr);
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
	TArray<FLinkAbilitySet_GameplayAbility> GrantedGameplayAbilities;
};
