// Copyright (c) Blueshift Interactive Ltd (2025)

#pragma once

#include "BlueshiftGameSettingsTypes.generated.h"

UENUM(BlueprintType, meta = (DisplayName = "Settings Quality Level (Blueshift)"))
enum class EBlueshiftSettingQualityLevel : uint8
{
	Low,
	Medium,
	High,
	Epic,
	Cinematic,
	MAX
};
ENUM_RANGE_BY_COUNT(EBlueshiftSettingQualityLevel, EBlueshiftSettingQualityLevel::MAX);