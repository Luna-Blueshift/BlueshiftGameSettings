// Copyright (c) Blueshift Interactive Ltd (2025)

#pragma once

/// UE includes
#include "Engine/DeveloperSettings.h"

/// Blueshift includes
#include "BlueshiftGameSettingsTypes.h"

#include "BlueshiftGameDeveloperSettings.generated.h"

class UBlueshiftGameUserSettingsConfig;
class UBlueshiftBPGameUserSettings;

UCLASS(Config = "Engine", DefaultConfig, meta = (DisplayName = "Game Developer Settings"))
class BLUESHIFTGAMESETTINGS_API UBlueshiftGameDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Settings", meta = (DisplayName = "Get Game Developer Settings (Blueshift)"))
	static const UBlueshiftGameDeveloperSettings* Get();

	// Begin UDeveloperSettings interface
	virtual FName GetCategoryName() const override { return TEXT("Blueshift"); }
	// End UDeveloperSettings interface
	
	/** Class to use for Blueprint-safe wrapper. */
	UPROPERTY(EditAnywhere, Config, Category = "GameUserSettings")
	TSoftClassPtr<UBlueshiftBPGameUserSettings> BlueprintClass = nullptr;
	
	UPROPERTY(EditAnywhere, Config, Category = "Settings")
	FText UserFacingQualityLevelText[static_cast<uint8>(EBlueshiftSettingQualityLevel::MAX)];

	/** Get the text to display to the user associated with the given quality level. */
	UFUNCTION(BlueprintPure, Category = "Settings")
	FText GetUserFacingQualityLevelText(const EBlueshiftSettingQualityLevel QualityLevel) const;
};