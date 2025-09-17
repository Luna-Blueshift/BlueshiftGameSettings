// Copyright (c) Blueshift Interactive Ltd (2025)

#include "GameSettings/BlueshiftGameSetting_Bool.h"

/// Blueshift includes
#include "BlueshiftGameSettingsLog.h"

// Begin UBlueshiftGameSetting interface
FText UBlueshiftGameSetting_Bool::GetDisplayValue_Implementation() const
{
	return FText::FromString(FString( GetValue() ? TEXT("True") : TEXT("False") ));
}
// End UBlueshiftGameSetting interface

void UBlueshiftGameSetting_Bool::SetValue_Implementation(const bool Value)
{
	UE_LOG(LogBlueshiftGameSettings, Log, TEXT("%s Set Value to %s"), *GetName(), Value ? TEXT("True") : TEXT("False"));
}