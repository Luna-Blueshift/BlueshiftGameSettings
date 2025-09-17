// Copyright (c) Blueshift Interactive Ltd (2025)

#pragma once

/// Blueshift includes
#include "GameSettings/BlueshiftGameSetting_Int.h"

#include "BlueshiftGameSetting_QualityLevel.generated.h"

UCLASS(Blueprintable, BlueprintType, Abstract, meta = (DisplayName = "Blueshift Game Setting (Quality Level)"))
class BLUESHIFTGAMESETTINGS_API UBlueshiftGameSetting_QualityLevel : public UBlueshiftGameSetting_Int
{
	GENERATED_BODY()

public:
	// Begin UBlueshiftGameSetting interface
	virtual FText GetDisplayValue_Implementation() const override;
	// End UBlueshiftGameSetting interface

	// Begin UBlueshiftGameSetting_Int interface
	virtual TMap<int32, FText> GetOptions_Implementation() const override;
	// End UBlueshiftGameSetting_Int interface
};