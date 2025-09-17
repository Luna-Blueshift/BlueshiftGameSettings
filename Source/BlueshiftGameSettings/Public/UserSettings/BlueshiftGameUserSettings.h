// Copyright (c) Blueshift Interactive Ltd (2025)

#pragma once

/// UE includes
#include "GameFramework/GameUserSettings.h"

#include "BlueshiftGameUserSettings.generated.h"

class UBlueshiftGameUserSettingsConfig;
enum class EColorVisionDeficiency : uint8;

/*
 *	Subclass of UGameUserSettings with added functionality.
 *	Supports Blueprints by using UBlueshiftBPGameUserSettings and setting the class in UBlueshiftGameDeveloperSettings.
 */
UCLASS(BlueprintType, NotBlueprintable, meta = (DisplayName = "Game User Settings (Blueshift)"))
class BLUESHIFTGAMESETTINGS_API UBlueshiftGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	static UBlueshiftGameUserSettings* Get();
	
	// Begin UObject interface
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
	// End UObject interface
	
	// Begin UGameUserSettings interface
	virtual void SetToDefaults() override;
	virtual void SaveSettings() override;
	virtual void LoadSettings(bool bForceReload = false) override;
	virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;
	virtual void ApplyNonResolutionSettings() override;
	// End UGameUserSettings interface
	
	//-----------------------------------------------------------------------------------------------------------------
	//	HELPERS
	//-----------------------------------------------------------------------------------------------------------------
protected:
	void RunOnGameThread(const TFunction<void()>& Func);
	
public:
	UFUNCTION(BlueprintPure, Category = "Settings", meta = (AutoCreateRefTerm = "Class", DeterminesOutputType = "Class"))
	static UGameUserSettings* GetGameUserSettings_Typed(TSubclassOf<UGameUserSettings> Class);
	
	UFUNCTION(BlueprintPure, Category = "Settings", meta = (AutoCreateRefTerm = "Class", DeterminesOutputType = "Class", BlueprintThreadSafe))
	static const UGameUserSettings* GetGameUserSettings_Typed_Immutable(TSubclassOf<UGameUserSettings> Class);

	//-----------------------------------------------------------------------------------------------------------------
	//	DELEGATES
	//-----------------------------------------------------------------------------------------------------------------
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnApplySettings, UGameUserSettings*, GameUserSettings, const bool, bNonResolution);
	
	UPROPERTY(BlueprintAssignable, Category = "Settings")
	FOnApplySettings OnApplySettings;

	//-----------------------------------------------------------------------------------------------------------------
	//	BLUEPRINT
	//-----------------------------------------------------------------------------------------------------------------
private:
	/** Instance of the Config object. */
	UPROPERTY(Transient)
	mutable TObjectPtr<UBlueshiftBPGameUserSettings> Blueprint = nullptr;

public:
	static bool SupportsBlueprint();
	UBlueshiftBPGameUserSettings* GetBlueprint() const;
	
	template <typename TType>
	TType* GetBlueprint() const
	{
		return Cast<TType>(Blueprint);
	}

	UFUNCTION(BlueprintPure, Category = "Blueprint", meta = (DisplayName = "Get Blueprint", NotBlueprintThreadSafe, AutoCreateRefTerm = "Class", DeterminesOutputType = "Class"))
	UBlueshiftBPGameUserSettings* BP_GetBlueprint(TSubclassOf<UBlueshiftBPGameUserSettings> Class) const;

	UFUNCTION(BlueprintPure, Category = "Blueprint", meta = (DisplayName = "Get Blueprint (Thread-Safe)", BlueprintThreadSafe, AutoCreateRefTerm = "Class", DeterminesOutputType = "Class"))
	const UBlueshiftBPGameUserSettings* BP_GetBlueprint_ThreadSafe(TSubclassOf<UBlueshiftBPGameUserSettings> Class) const;

	static UBlueshiftBPGameUserSettings* GetGameUserSettingsBlueprint();
	
	UFUNCTION(BlueprintPure, Category = "Blueprint", meta = (DisplayName = "Get Game User Settings Blueprint", NotBlueprintThreadSafe, AutoCreateRefTerm = "Class", DeterminesOutputType = "Class"))
	static UBlueshiftBPGameUserSettings* BP_GetGameUserSettingsBlueprint(TSubclassOf<UBlueshiftBPGameUserSettings> Class);

	UFUNCTION(BlueprintPure, Category = "Blueprint", meta = (DisplayName = "Get Game User Settings Blueprint (Thread-Safe)", BlueprintThreadSafe, AutoCreateRefTerm = "Class", DeterminesOutputType = "Class"))
	static const UBlueshiftBPGameUserSettings* BP_GetGetGameUserSettingsBlueprint_ThreadSafe(TSubclassOf<UBlueshiftBPGameUserSettings> Class);
	
	//-----------------------------------------------------------------------------------------------------------------
	//	ACCESSIBILITY OPTIONS
	//-----------------------------------------------------------------------------------------------------------------
private:
	UPROPERTY(Config)
	EColorVisionDeficiency ColorVisionDeficiencyMode = EColorVisionDeficiency::NormalVision;
	
	UPROPERTY(Config)
	float ColorVisionSeverity = 1.0f;

public:
	UFUNCTION(BlueprintCallable, Category = "Accessibility|Color Blind Options")
	void UpdateColorVision() const;

	UFUNCTION(BlueprintPure, Category = "Accessibility|Color Blind Options")
	FORCEINLINE EColorVisionDeficiency GetColorVisionDeficiencyMode() const { return ColorVisionDeficiencyMode; }
	
	UFUNCTION(BlueprintPure, Category = "Accessibility|Color Blind Options")
	FORCEINLINE float GetColorVisionSeverity() const { return ColorVisionSeverity; }
	
	UFUNCTION(BlueprintCallable, Category = "Accessibility|Color Blind Options")
	void SetColorVisionDeficiencyMode(const EColorVisionDeficiency InColorVisionDeficiencyMode, const bool bUpdateColorVision = true);
	
	UFUNCTION(BlueprintCallable, Category = "Accessibility|Color Blind Options")
	void SetColorVisionSeverity(const float InColorVisionSeverity, const bool bUpdateColorVision = true);
};