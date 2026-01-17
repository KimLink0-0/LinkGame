// Fill out your copyright notice in the Description page of Project Settings.

#include "LinkCharacter.h"
#include "LinkPawnExtensionComponent.h"

// Sets default values
ALinkCharacter::ALinkCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	PawnExtComponent = CreateDefaultSubobject<ULinkPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
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
	
	// Pawn 이 Possess 되고 나서, Controller 와  PlayerState 정보 접근이 가능한 상태가 된 시점
	// Super 함수 참고
	PawnExtComponent->SetupPlayerInputComponent();
}

