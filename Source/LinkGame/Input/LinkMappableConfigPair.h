#pragma once

#include "LinkMappableConfigPair.generated.h"

/** forward declaration */
class UPlayerMappableInputConfig;

USTRUCT()
struct FLinkMappableConfigPair
{
	GENERATED_BODY()
public:
	FLinkMappableConfigPair() = default;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UPlayerMappableInputConfig> Config;

	UPROPERTY(EditAnywhere)
	bool bShouldActivateAutomatically = true;
};