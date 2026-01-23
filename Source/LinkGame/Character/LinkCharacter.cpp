// Fill out your copyright notice in the Description page of Project Settings.

#include "LinkCharacter.h"
#include "LinkPawnExtensionComponent.h"
#include "AbilitySystem/LinkAbilitySystemComponent.h"
#include "LinkGame/Camera/LinkCameraComponent.h"

// Sets default values
ALinkCharacter::ALinkCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// PawnExtComponent 
	PawnExtComponent = CreateDefaultSubobject<ULinkPawnExtensionComponent>(TEXT("PawnExtensionComponent"));

	// CameraComponent 
	{
		CameraComponent = CreateDefaultSubobject<ULinkCameraComponent>(TEXT("CameraComponent"));
		CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));
	}
}

UAbilitySystemComponent* ALinkCharacter::GetAbilitySystemComponent() const
{
	return PawnExtComponent->GetLinkAbilitySystemComponent();
}

// Called when the game starts or when spawned
void ALinkCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALinkCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALinkCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnExtComponent->SetupPlayerInputComponent();
}

