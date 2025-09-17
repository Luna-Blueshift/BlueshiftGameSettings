// Copyright (c) Blueshift Interactive Ltd (2025)

#include "GameSettings/BlueshiftGameSetting_Float.h"

/// Blueshift includes
#include "BlueshiftGameSettingsLog.h"

// Begin UBlueshiftGameSetting interface
FText UBlueshiftGameSetting_Float::GetDisplayValue_Implementation() const
{
	return GetDisplayValueFromFloat(GetValue());
}
// End UBlueshiftGameSetting interface

void UBlueshiftGameSetting_Float::SetValue_Implementation(const float Value)
{
	UE_LOG(LogBlueshiftGameSettings, Log, TEXT("%s Set Value to %f"), *GetName(), Value);
}

FText UBlueshiftGameSetting_Float::GetDisplayValueFromFloat_Implementation(const float Value) const
{
	FNumberFormattingOptions NumberFormatOptions;
	NumberFormatOptions.AlwaysSign = bAlwaysSign;
	NumberFormatOptions.UseGrouping = bUseGrouping;
	NumberFormatOptions.RoundingMode = RoundingMode;
	NumberFormatOptions.MinimumIntegralDigits = MinimumIntegralDigits;
	NumberFormatOptions.MaximumIntegralDigits = MaximumIntegralDigits;
	NumberFormatOptions.MinimumFractionalDigits = MinimumFractionalDigits;
	NumberFormatOptions.MaximumFractionalDigits = MaximumFractionalDigits;
	
	return FText::AsNumber(Value, &NumberFormatOptions);
}

float UBlueshiftGameSetting_Float::GetValueAlpha() const
{
	const float Value = GetValue();
	
	float Min, Max;
	GetValueRange(OUT Min, OUT Max);
	const float Range = Max - Min;
	
	const float Result = (Value - Min) / FMath::Max(Range, UE_SMALL_NUMBER);
	return Result;
}
