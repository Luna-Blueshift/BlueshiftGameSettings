// Copyright (c) Blueshift Interactive Ltd (2025)

#pragma once

/// Blueshift includes
#include "GameSettings/BlueshiftGameSetting.h"

#include "BlueshiftGameSetting_Bool.generated.h"

UCLASS(Blueprintable, BlueprintType, Abstract, meta = (DisplayName = "Blueshift Game Setting (Bool)"))
class BLUESHIFTGAMESETTINGS_API UBlueshiftGameSetting_Bool : public UBlueshiftGameSetting
{
	GENERATED_BODY()

public:
	// Begin UBlueshiftGameSetting interface
	virtual FText GetDisplayValue_Implementation() const override;
	// End UBlueshiftGameSetting interface
	
	/** Override this to set the real value, i.e. in the Game User Settings. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Settings")
	void SetValue(const bool Value);
	virtual void SetValue_Implementation(const bool Value);

	/** Override this to get the value, i.e. from the Game User Settings. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Settings")
	bool GetValue() const;
	virtual bool GetValue_Implementation() const { return false; }
};