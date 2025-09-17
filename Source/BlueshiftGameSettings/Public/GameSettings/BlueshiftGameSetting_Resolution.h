// Copyright (c) Blueshift Interactive Ltd (2025)

#pragma once

/// Blueshift includes
#include "GameSettings/BlueshiftGameSetting_Int.h"

#include "BlueshiftGameSetting_Resolution.generated.h"

UCLASS(Blueprintable, BlueprintType, Abstract, meta = (DisplayName = "Blueshift Game Setting (Resolution)"))
class BLUESHIFTGAMESETTINGS_API UBlueshiftGameSetting_Resolution : public UBlueshiftGameSetting_Int
{
	GENERATED_BODY()
	
public:
	// Begin UBlueshiftGameSetting interface
	virtual FText GetDisplayValue_Implementation() const override;
	virtual void SetValue_Implementation(const int32 Value) override;
	virtual int32 GetValue_Implementation() const override;
	virtual TMap<int32, FText> GetOptions_Implementation() const override;
	// End UBlueshiftGameSetting interface

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	bool bCheckForCommandLineOverrides = false;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure = "false", Category = "Settings|Resolution")
	FIntPoint GetScreenResolutionFromIndex(const int32 Index) const;
};