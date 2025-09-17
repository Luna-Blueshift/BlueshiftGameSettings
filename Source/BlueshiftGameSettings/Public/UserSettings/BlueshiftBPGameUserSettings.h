// Copyright (c) Blueshift Interactive Ltd (2025)

#pragma once

/// UE includes
#include "UObject/Object.h"

#include "BlueshiftBPGameUserSettings.generated.h"

class UGameUserSettings;

/**
 *	Config class to hold variables for Game User Settings.
 *	Override this to add your own config variables, which is safe for Blueprints.
 */
UCLASS(Config = "GameUserSettings", ConfigDoNotCheckDefaults, Blueprintable, Abstract, BlueprintType, meta = (DisplayName = "Blueprint Game User Settings (Blueshift)"))
class BLUESHIFTGAMESETTINGS_API UBlueshiftBPGameUserSettings : public UObject
{
	GENERATED_BODY()

	friend class UBlueshiftGameUserSettings;

public:
	// Begin UObject interface
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
	// End UObject interface

	UBlueshiftGameUserSettings* GetGameUserSettings() const;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	bool bWarnConfigVariableUsage = false;
#endif

	//-----------------------------------------------------------------------------------------------------------------
	//	EVENTS
	//-----------------------------------------------------------------------------------------------------------------
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Settings", meta = (DisplayName = "On Set to Defaults"))
	void BP_OnSetToDefaults();

	UFUNCTION(BlueprintImplementableEvent, Category = "Settings", meta = (DisplayName = "On Save Settings"))
	void BP_OnSaveSettings();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Settings", meta = (DisplayName = "On Load Settings"))
	void BP_OnLoadSettings();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Settings", meta = (DisplayName = "On Apply Settings"))
	void BP_OnApplySettings(const bool bNonResolution);

	//-----------------------------------------------------------------------------------------------------------------
	//	DELEGATES
	//-----------------------------------------------------------------------------------------------------------------
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnApplySettings, UGameUserSettings*, GameUserSettings, UBlueshiftBPGameUserSettings*, BPGameUserSettings, const bool, bNonResolution);
	
	UPROPERTY(BlueprintAssignable, Category = "Settings")
	FOnApplySettings OnApplySettings;
};