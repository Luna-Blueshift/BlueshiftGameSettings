// Copyright (c) Blueshift Interactive Ltd (2025)

#include "GameSettings/BlueshiftGameSetting_QualityLevel.h"

/// Blueshift includes
#include "BlueshiftGameDeveloperSettings.h"
#include "BlueshiftGameSettingsTypes.h"

// Begin UBlueshiftGameSetting interface
FText UBlueshiftGameSetting_QualityLevel::GetDisplayValue_Implementation() const
{
	const EBlueshiftSettingQualityLevel QualityLevel = static_cast<EBlueshiftSettingQualityLevel>(GetValue());
	return UBlueshiftGameDeveloperSettings::Get()->GetUserFacingQualityLevelText(QualityLevel);
}
// End UBlueshiftGameSetting interface

// Begin UBlueshiftGameSetting_Int interface
TMap<int32, FText> UBlueshiftGameSetting_QualityLevel::GetOptions_Implementation() const
{
	TMap<int32, FText> Result;

	// Get the Developer Settings - this is where we keep our Quality Level -> FText mapping
	const UBlueshiftGameDeveloperSettings* const GameDeveloperSettings = UBlueshiftGameDeveloperSettings::Get();
	
	for (EBlueshiftSettingQualityLevel QualityLevel : TEnumRange<EBlueshiftSettingQualityLevel>())
	{
		const int32 ValueAsInt = static_cast<int32>(QualityLevel);
		const FText UserFacingText = GameDeveloperSettings->GetUserFacingQualityLevelText(QualityLevel);
		Result.Emplace(ValueAsInt, UserFacingText);
	}

	return  Result;
}
// End UBlueshiftGameSetting_Int interface