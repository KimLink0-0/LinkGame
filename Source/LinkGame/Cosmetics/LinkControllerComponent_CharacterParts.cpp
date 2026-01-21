// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkControllerComponent_CharacterParts.h"

#include "LinkPawnComponent_CharacterParts.h"

ULinkControllerComponent_CharacterParts::ULinkControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer) : Super (ObjectInitializer)
{
}

void ULinkControllerComponent_CharacterParts::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		if (AController* OwningController = GetController<AController>())
		{
			OwningController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChanged);
		}
	}
}

void ULinkControllerComponent_CharacterParts::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveAllCharacterParts();
	
	Super::EndPlay(EndPlayReason);
}

ULinkPawnComponent_CharacterParts* ULinkControllerComponent_CharacterParts::GetPawnCustomizer() const
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		return ControlledPawn->FindComponentByClass<ULinkPawnComponent_CharacterParts>();
	}
	return nullptr;
}

void ULinkControllerComponent_CharacterParts::AddCharacterPart(const FLinkCharacterPart& NewPart)
{
	AddCharacterPartInternal(NewPart);
}

void ULinkControllerComponent_CharacterParts::AddCharacterPartInternal(const FLinkCharacterPart& NewPart)
{
	// AddDefaulted_GetRef 는 유용한 함수니 나중에 찾아보기, 최적화 함수 중 하나
	FLinkControllerCharacterPartEntry& NewEntry = CharacterParts.AddDefaulted_GetRef();
	NewEntry.Part = NewPart;
	
	// 라이라는 해당 함수가 분리 (컨트롤러는 Spawn 하는 역할이 아니기 때문)
	if (ULinkPawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		NewEntry.Handle = PawnCustomizer->AddCharacterPart(NewPart);
	}
}

void ULinkControllerComponent_CharacterParts::RemoveAllCharacterParts()
{
	if (ULinkPawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		for (FLinkControllerCharacterPartEntry& Entry : CharacterParts)
		{
			PawnCustomizer->RemoveCharacterPart(Entry.Handle);
		}
	}
}

void ULinkControllerComponent_CharacterParts::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	if (ULinkPawnComponent_CharacterParts* OldCustomizer = OldPawn ? OldPawn->FindComponentByClass<ULinkPawnComponent_CharacterParts>() : nullptr)
	{
		for (FLinkControllerCharacterPartEntry& Entry : CharacterParts)
		{
			OldCustomizer->RemoveCharacterPart(Entry.Handle);
			Entry.Handle.Reset();
		}
	}
	
	if (ULinkPawnComponent_CharacterParts* NewCustomizer = NewPawn ? NewPawn->FindComponentByClass<ULinkPawnComponent_CharacterParts>() : nullptr)
	{
		for (FLinkControllerCharacterPartEntry& Entry : CharacterParts)
		{
			check(!Entry.Handle.IsValid());
			Entry.Handle = NewCustomizer->AddCharacterPart(Entry.Part);
		}
	}
}
