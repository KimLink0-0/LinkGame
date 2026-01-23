// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkGameplayAbility_RangedWeapon.h"

#include "AbilitySystemComponent.h"
#include "LinkRangedWeaponInstance.h"
#include "AbilitySystem/LinkGameplayAbilityTargetData_SingleTargetHit.h"
#include "Physics/LinkCollisionChannels.h"

ULinkGameplayAbility_RangedWeapon::ULinkGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer) : Super (ObjectInitializer)
{
}

void ULinkGameplayAbility_RangedWeapon::StartRangedWeaponTargeting()
{
	check(CurrentActorInfo);
	
	AActor* AvatarActor = CurrentActorInfo->AvatarActor.Get();
	check(AvatarActor);
	
	UAbilitySystemComponent* MyAbilityComponent = CurrentActorInfo->AbilitySystemComponent.Get();
	check(MyAbilityComponent);
	
	TArray<FHitResult> FoundHits;
	PerformLocalTargeting(FoundHits);
	
	FGameplayAbilityTargetDataHandle TargetData;
	TargetData.UniqueId = 0;
	
	if (FoundHits.Num() > 0)
	{
		const int32 CartridgeID = FMath::Rand();
		for (const FHitResult& FoundHit : FoundHits)
		{
			FLinkGameplayAbilityTargetData_SingleTargetHit* NewTargetData = new FLinkGameplayAbilityTargetData_SingleTargetHit();
			NewTargetData->HitResult = FoundHit;
			NewTargetData->CartridgeID = CartridgeID;
			TargetData.Add(NewTargetData);
		}
	}
	
	OnTargetDataReadyCallback(TargetData, FGameplayTag());
}

void ULinkGameplayAbility_RangedWeapon::PerformLocalTargeting(TArray<FHitResult>& OutHits)
{
	APawn* const AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());
	
	ULinkRangedWeaponInstance* WeaponData = GetWeaponInstance();
	if (AvatarPawn && AvatarPawn->IsLocallyControlled() && WeaponData)
	{
		FRangedWeaponFiringInput InputData;
		InputData.WeaponData = WeaponData;
		InputData.bCanPlayBulletFX = true;
		
		const FTransform TargetTransform = GetTargetingTransform(AvatarPawn, ELinkAbilityTargetingSource::CameraTowardsFocus);
		InputData.AimDir = TargetTransform.GetUnitAxis(EAxis::X);
		InputData.StartTrace = TargetTransform.GetTranslation();
		InputData.EndAim = InputData.StartTrace + InputData.AimDir * WeaponData->MaxDamageRange;
#if 1
		{
			static float DebugThickness = 2.0f;
			DrawDebugLine(GetWorld(), InputData.StartTrace, InputData.StartTrace + (InputData.AimDir * 100.0f), FColor::Yellow, false, 10.0f, 0, DebugThickness);
		}
#endif 
		TraceBulletsInCartridge(InputData, OutHits);
	}
}

FTransform ULinkGameplayAbility_RangedWeapon::GetTargetingTransform(APawn* SourcePawn, ELinkAbilityTargetingSource Source)
{
	check(SourcePawn);
	check(Source == ELinkAbilityTargetingSource::CameraTowardsFocus);

	AController* Controller = SourcePawn->Controller;
	if (Controller == nullptr)
	{
		return FTransform();
	}

	double FocalDistance = 1024.0f;
	FVector FocalLoc;
	FVector CamLoc;
	FRotator CamRot;

	APlayerController* PC = Cast<APlayerController>(Controller);
	check(PC);
	PC->GetPlayerViewPoint(CamLoc, CamRot);

	FVector AimDir = CamRot.Vector().GetSafeNormal();
	FocalLoc = CamLoc + (AimDir * FocalDistance);

	const FVector WeaponLoc = GetWeaponTargetingSourceLocation();
	FVector FinalCamLoc = FocalLoc + (((WeaponLoc - FocalLoc) | AimDir) * AimDir);

#if 1
	{
		// WeaponLoc 
		DrawDebugPoint(GetWorld(), WeaponLoc, 10.0f, FColor::Red, false, 60.0f);
		// CamLoc
		DrawDebugPoint(GetWorld(), CamLoc, 10.0f, FColor::Yellow, false, 60.0f);
		// FinalCamLoc
		DrawDebugPoint(GetWorld(), FinalCamLoc, 10.0f, FColor::Magenta, false, 60.0f);

		// (WeaponLoc - FocalLoc)
		DrawDebugLine(GetWorld(), FocalLoc, WeaponLoc, FColor::Yellow, false, 60.0f, 0, 2.0f);
		// (AimDir)
		DrawDebugLine(GetWorld(), CamLoc, FocalLoc, FColor::Blue, false, 60.0f, 0, 2.0f);

		// Project Direction Line
		DrawDebugLine(GetWorld(), WeaponLoc, FinalCamLoc, FColor::Red, false, 60.0f, 0, 2.0f);
	}
#endif

	return FTransform(CamRot, FinalCamLoc);
}

FVector ULinkGameplayAbility_RangedWeapon::GetWeaponTargetingSourceLocation() const
{
	APawn* const AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());
	check(AvatarPawn);
	
	const FVector SourceLoc = AvatarPawn->GetActorLocation();
	return SourceLoc;
}

void ULinkGameplayAbility_RangedWeapon::TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData,
	TArray<FHitResult>& OutHits)
{
	ULinkRangedWeaponInstance* WeaponData = InputData.WeaponData;
	check(WeaponData);

	const FVector BulletDir = InputData.AimDir;
	const FVector EndTrace = InputData.StartTrace + (BulletDir * WeaponData->MaxDamageRange);

	FVector HitLocation = EndTrace;

	TArray<FHitResult> AllImpacts;
	FHitResult Impact = DoSingleBulletTrace(InputData.StartTrace, EndTrace, WeaponData->BulletTraceWeaponRadius, false, AllImpacts);

	const AActor* HitActor = Impact.GetActor();
	if (HitActor)
	{
		if (AllImpacts.Num() > 0)
		{
			OutHits.Append(AllImpacts);
		}

		HitLocation = Impact.ImpactPoint;
	}

	if (OutHits.Num() == 0)
	{
		if (!Impact.bBlockingHit)
		{
			Impact.Location = EndTrace;
			Impact.ImpactPoint = EndTrace;
		}

		OutHits.Add(Impact);
	}
}

int32 FindFirstPawnHitResult(const TArray<FHitResult>& HitResults)
{
	for (int32 Idx = 0; Idx < HitResults.Num(); ++Idx)
	{
		const FHitResult& CurHitResult = HitResults[Idx];
		if (CurHitResult.HitObjectHandle.DoesRepresentClass(APawn::StaticClass()))
		{
			return Idx;
		}
		else
		{
			AActor* HitActor = CurHitResult.HitObjectHandle.FetchActor();
			
			if ((HitActor != nullptr) && (HitActor->GetAttachParentActor() != nullptr) && (Cast<APawn>(HitActor->GetAttachParentActor()) != nullptr))
			{
				return Idx;
			}
		}
	}
	return INDEX_NONE;
}

FHitResult ULinkGameplayAbility_RangedWeapon::DoSingleBulletTrace(const FVector& StartTrace, const FVector& EndTrace,
	float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHits) const
{	FHitResult Impact;

	if (FindFirstPawnHitResult(OutHits) == INDEX_NONE)
	{
		Impact = WeaponTrace(StartTrace, EndTrace, 0.0f, bIsSimulated, OutHits);
	}

	if (FindFirstPawnHitResult(OutHits) == INDEX_NONE)
	{
		if (SweepRadius > 0.0f)
		{
			TArray<FHitResult> SweepHits;
			Impact = WeaponTrace(StartTrace, EndTrace, SweepRadius, bIsSimulated, SweepHits);

			const int32 FirstPawnIdx = FindFirstPawnHitResult(SweepHits);
			if (SweepHits.IsValidIndex(FirstPawnIdx))
			{
				bool bUseSweepHits = true;
				for (int32 Idx = 0; Idx < FirstPawnIdx; ++Idx)
				{
					const FHitResult& CurHitResult = SweepHits[Idx];

					auto Pred = [&CurHitResult](const FHitResult& Other)
					{
						return Other.HitObjectHandle == CurHitResult.HitObjectHandle;
					};

					if (CurHitResult.bBlockingHit && OutHits.ContainsByPredicate(Pred))
					{
						bUseSweepHits = false;
						break;
					}
				}

				// SweepHits
				if (bUseSweepHits)
				{
					OutHits = SweepHits;
				}
			}
		}
	}

	return Impact;
}

FHitResult ULinkGameplayAbility_RangedWeapon::WeaponTrace(const FVector& StartTrace, const FVector& EndTrace,
	float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHitResults) const
{
	TArray<FHitResult> HitResults;

	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), true, GetAvatarActorFromActorInfo());
	TraceParams.bReturnPhysicalMaterial = true;

	AddAdditionalTraceIgnoreActors(TraceParams);

	const ECollisionChannel TraceChannel = DetermineTraceChannel(TraceParams, bIsSimulated);
	if (SweepRadius > 0.0f)
	{
		GetWorld()->SweepMultiByChannel(HitResults, StartTrace, EndTrace, FQuat::Identity, TraceChannel, FCollisionShape::MakeSphere(SweepRadius), TraceParams);
	}
	else
	{
		GetWorld()->LineTraceMultiByChannel(HitResults, StartTrace, EndTrace, TraceChannel, TraceParams);
	}

	FHitResult Hit(ForceInit);
	if (HitResults.Num() > 0)
	{
		for (FHitResult& CurHitResult : HitResults)
		{
			auto Pred = [&CurHitResult](const FHitResult& Other)
			{
				return Other.HitObjectHandle == CurHitResult.HitObjectHandle;
			};

			if (!OutHitResults.ContainsByPredicate(Pred))
			{
				OutHitResults.Add(CurHitResult);
			}
		}

		Hit = OutHitResults.Last();
	}
	else
	{
		Hit.TraceStart = StartTrace;
		Hit.TraceEnd = EndTrace;
	}

	return Hit;
}

ECollisionChannel ULinkGameplayAbility_RangedWeapon::DetermineTraceChannel(FCollisionQueryParams& TraceParams,
	bool bIsSimulated) const
{
	return Link_TraceChannel_Weapon;
}

void ULinkGameplayAbility_RangedWeapon::AddAdditionalTraceIgnoreActors(FCollisionQueryParams& TraceParams) const
{
	if (AActor* Avatar = GetAvatarActorFromActorInfo())
	{
		TArray<AActor*> AttachedActors;

		Avatar->GetAttachedActors(AttachedActors);

		TraceParams.AddIgnoredActors(AttachedActors);
	}
}

void ULinkGameplayAbility_RangedWeapon::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData,
	FGameplayTag ApplicationTag)
{
	UAbilitySystemComponent* LinkAbilitySystemComponent = CurrentActorInfo->AbilitySystemComponent.Get();
	check(LinkAbilitySystemComponent);

	if (const FGameplayAbilitySpec* AbilitySpec = LinkAbilitySystemComponent->FindAbilitySpecFromHandle(CurrentSpecHandle))
	{
		FGameplayAbilityTargetDataHandle LocalTargetDataHandle(MoveTemp(const_cast<FGameplayAbilityTargetDataHandle&>(InData)));

		if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
		{
			OnRangeWeaponTargetDataReady(LocalTargetDataHandle);
		}
		else
		{
			K2_EndAbility();
		}
	}
}

ULinkRangedWeaponInstance* ULinkGameplayAbility_RangedWeapon::GetWeaponInstance()
{
	return Cast<ULinkRangedWeaponInstance>(GetAssociatedEquipment());
}
