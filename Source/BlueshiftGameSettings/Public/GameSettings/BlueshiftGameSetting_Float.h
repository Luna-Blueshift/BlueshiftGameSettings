// Copyright (c) Blueshift Interactive Ltd (2025)

#pragma once

/// Blueshift includes
#include "GameSettings/BlueshiftGameSetting.h"

#include "BlueshiftGameSetting_Float.generated.h"

UCLASS(Blueprintable, BlueprintType, Abstract, meta = (DisplayName = "Blueshift Game Setting (Float)"))
class BLUESHIFTGAMESETTINGS_API UBlueshiftGameSetting_Float : public UBlueshiftGameSetting
{
	GENERATED_BODY()

public:
	// Begin UBlueshiftGameSetting interface
	virtual FText GetDisplayValue_Implementation() const override;
	// End UBlueshiftGameSetting interface

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Display")
	TEnumAsByte<ERoundingMode> RoundingMode = HalfToEven;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Display")
	bool bAlwaysSign = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Display")
	bool bUseGrouping = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Display", meta = (UIMin = "0", ClampMin = "0"))
	int32 MinimumIntegralDigits = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Display", meta = (UIMin = "0", ClampMin = "0"))
	int32 MaximumIntegralDigits = 324;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Display", meta = (UIMin = "0", ClampMin = "0"))
	int32 MinimumFractionalDigits = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Display", meta = (UIMin = "0", ClampMin = "0"))
	int32 MaximumFractionalDigits = 2;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Input", meta = (UIMin = "0.0", ClampMin = "0.0"))
	float StepSize = 0.01f;
	
	/** Override this to set the real value, i.e. in the Game User Settings. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Settings")
	void SetValue(const float Value);
	virtual void SetValue_Implementation(const float Value);

	/** Override this to get the value, i.e. from the Game User Settings. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Settings")
	float GetValue() const;
	virtual float GetValue_Implementation() const { return -1.0f; }

	/** Define the range we can be set within. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Settings")
	void GetValueRange(float& Min, float& Max) const;
	virtual void GetValueRange_Implementation(float& Min, float& Max) const { Min = 0.0f; Max = 1.0f; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Settings")
	FText GetDisplayValueFromFloat(const float Value) const;
	virtual FText GetDisplayValueFromFloat_Implementation(const float Value) const;

	UFUNCTION(BlueprintPure, Category = "Settings")
	float GetValueAlpha() const;
};