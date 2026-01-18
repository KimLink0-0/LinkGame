// Fill out your copyright notice in the Description page of Project Settings.

#include "LinkCharacter.h"
#include "LinkPawnExtensionComponent.h"
#include "Camera/LinkCameraComponent.h"

// Sets default values
ALinkCharacter::ALinkCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	// PawnExtensionComponent 생성
	PawnExtComponent = CreateDefaultSubobject<ULinkPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
	
	// CameraComponent 생성
	CameraComponent = CreateDefaultSubobject<ULinkCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));
	
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

