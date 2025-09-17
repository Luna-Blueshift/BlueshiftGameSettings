// Copyright (c) Blueshift Interactive Ltd (2025)

#include "GameSettings/BlueshiftGameSetting_Int.h"

/// Blueshift includes
#include "BlueshiftGameSettingsLog.h"

// Begin UBlueshiftGameSetting interface
FText UBlueshiftGameSetting_Int::GetDisplayValue_Implementation() const
{
	return GetDisplayValueFromInt(GetValue());
}
// End UBlueshiftGameSetting interface

void UBlueshiftGameSetting_Int::SetValue_Implementation(const int32 Value)
{
	UE_LOG(LogBlueshiftGameSettings, Log, TEXT("%s Set Value to %i"), *GetName(), Value);
}

FText UBlueshiftGameSetting_Int::GetDisplayValueFromInt_Implementation(const int32 Value) const
{
	return FText::FromString(FString::FromInt(Value));
}

FText UBlueshiftGameSetting_Int::GetOption(const int32 Index) const
{
	const TMap<int32, FText> Options = GetOptions();
	if (const FText* Option = Options.Find(Index))
	{
		FText Result = FText::FromString(Option->ToString());
		return Result;
	}

	return FText::GetEmpty();
}

FText UBlueshiftGameSetting_Int::GetCurrentOption() const
{
	return GetOption(GetValue());
}

int32 UBlueshiftGameSetting_Int::GetValueByPredicate(const TFunction<bool(int32, int32)>& SortFunc) const
{
	if (SortFunc == nullptr)
	{
		return 0;
	}
	
	TArray<int32> Keys;
	GetOptions().GetKeys(OUT Keys);
	if (Keys.IsEmpty())
	{
		return 0;
	}
	
	Keys.Sort(SortFunc);
	return Keys.Last();
}

int32 UBlueshiftGameSetting_Int::GetMinValue() const
{
	return GetValueByPredicate([](const int32 A, const int32 B)
	{
		return A > B;
	});
}

int32 UBlueshiftGameSetting_Int::GetMaxValue() const
{
	return GetValueByPredicate([](const int32 A, const int32 B)
	{
		return A < B;
	});
}