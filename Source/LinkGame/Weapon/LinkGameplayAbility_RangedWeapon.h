// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment/LinkGameplayAbility_FromEquipment.h"
#include "LinkGameplayAbility_RangedWeapon.generated.h"

class ULinkRangedWeaponInstance;

UENUM(BlueprintType)
enum class ELinkAbilityTargetingSource : uint8
{
	CameraTowardsFocus,
};

UCLASS()
class LINKGAME_API ULinkGameplayAbility_RangedWeapon : public ULinkGameplayAbility_FromEquipment
{
	GENERATED_BODY()
public:
	struct FRangedWeaponFiringInput
	{
		FVector StartTrace;
		FVector EndAim;
		FVector AimDir;
		ULinkRangedWeaponInstance* WeaponData;
		bool bCanPlayBulletFX = false;
		
		FRangedWeaponFiringInput() : 
			StartTrace(ForceInitToZero),
			EndAim(ForceInitToZero),
			AimDir(ForceInitToZero)
		{
		}
	};
	
	ULinkGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintCallable)
	void StartRangedWeaponTargeting();
	
	void PerformLocalTargeting(TArray<FHitResult>& OutHits);
	FTransform GetTargetingTransform(APawn* SourcePawn, ELinkAbilityTargetingSource Source);
	FVector GetWeaponTargetingSourceLocation() const;
	void TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData, TArray<FHitResult>& OutHits);
	FHitResult DoSingleBulletTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHits) const;
	FHitResult WeaponTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHitResults) const;
	ECollisionChannel DetermineTraceChannel(FCollisionQueryParams& TraceParams, bool bIsSimulated) const;
	void AddAdditionalTraceIgnoreActors(FCollisionQueryParams& TraceParams) const;
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag);

	/** called when target data is ready */
	UFUNCTION(BlueprintImplementableEvent)
	void OnRangeWeaponTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData);

	ULinkRangedWeaponInstance* GetWeaponInstance();
};
