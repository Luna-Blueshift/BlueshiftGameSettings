// Copyright (c) Blueshift Interactive Ltd (2025)

#include "BlueshiftGameDeveloperSettings.h"

const UBlueshiftGameDeveloperSettings* UBlueshiftGameDeveloperSettings::Get()
{
	return GetDefault<UBlueshiftGameDeveloperSettings>();
}

FText UBlueshiftGameDeveloperSettings::GetUserFacingQualityLevelText(const EBlueshiftSettingQualityLevel QualityLevel) const
{
	const int32 Index = static_cast<int32>(QualityLevel);
	return UserFacingQualityLevelText[Index];
}