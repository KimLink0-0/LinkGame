// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LinkCharacterPartTypes.h"
#include "Components/ControllerComponent.h"
#include "LinkControllerComponent_CharacterParts.generated.h"

class ULinkPawnComponent_CharacterParts;

USTRUCT()
struct FLinkControllerCharacterPartEntry
{
	GENERATED_BODY()
	
	// 메타 데이터, CharacterPart 에 대한 정의
	UPROPERTY(EditAnywhere)
	FLinkCharacterPart Part;
	
	// Pawn 에서 생성한 인스턴스, CharacterPart 에 대한 핸들 값
	FLinkCharacterPartHandle Handle;
};

UCLASS(meta = (BlueprintSpawnableComponent))
class ULinkControllerComponent_CharacterParts : public UControllerComponent
{
	GENERATED_BODY()
public:
	ULinkControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	// UControllerComponent's Interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	ULinkPawnComponent_CharacterParts* GetPawnCustomizer() const;
	
	// 라이라는 데디케이트와 블루프린트에서 관리하는 것이 많아서 함수를 분리
	UFUNCTION(BlueprintCallable, Category = Cosmetic)
	void AddCharacterPart(const FLinkCharacterPart& NewPart);
	
	void AddCharacterPartInternal(const FLinkCharacterPart& NewPart);
	
	void RemoveAllCharacterParts();
	
	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);
	
	// CharacterPart 에 대한 메타 데이터, 그것들을 가지고 있음
	UPROPERTY(EditAnywhere, Category = Cosmetics)
	TArray<FLinkControllerCharacterPartEntry> CharacterParts;
};