// Copyright (c) Blueshift Interactive Ltd (2025)

#include "UserSettings/BlueshiftGameUserSettings.h"

/// UE includes
#include "BlueshiftGameDeveloperSettings.h"
#include "BlueshiftGameSettingsLog.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UserSettings/BlueshiftBPGameUserSettings.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#define LOCTEXT_NAMESPACE "BlueshiftGameSettings"

UBlueshiftGameUserSettings* UBlueshiftGameUserSettings::Get()
{
	return Cast<UBlueshiftGameUserSettings>(UGameUserSettings::GetGameUserSettings());	
}

// Begin UObject interface
#if WITH_EDITOR
EDataValidationResult UBlueshiftGameUserSettings::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	const UClass* const ThisClass = GetClass();
	check(ThisClass != nullptr);

	// Iterate all Blueprint properties and check if they have the Config flag on them
	// We need to warn the user if this happens because the Blueprint VM won't be initialized in the correct order
	// when initializing a UGameUserSettings class which has Config properties
	for (TFieldIterator<FProperty> PropertyIt(ThisClass, EFieldIterationFlags::IncludeSuper); PropertyIt; ++PropertyIt)
	{
		// Resolve Property
		const FProperty* Property = *PropertyIt;
		if (Property == nullptr)
		{
			continue;
		}

		// Skip native properties defined in C++ class
		if (Property->GetOwnerClass()->HasAnyClassFlags(CLASS_Native))
		{
			continue;
		}

		// Property contains config and thus could cause a crash on startup
		if (Property->HasAnyPropertyFlags(CPF_Config))
		{
			FFormatNamedArguments FormatNamedArguments;
			FormatNamedArguments.Emplace(TEXT("PropertyName"), FText::FromString(*Property->GetName()));
			FormatNamedArguments.Emplace(TEXT("BlueprintName"), FText::FromString(*ThisClass->GetName()));
			
			Context.AddWarning(FText::Format(LOCTEXT("ConfigPropertyWarning", "Property '{PropertyName}' in Blueprint subclass '{BlueprintName}' is marked as Config, which may cause runtime crashes with GameUserSettings. Please use the config object or move the property to C++."), FormatNamedArguments));
			// NOTE: Do not block cook by returning invalid here, just warn
		}
	}
	
	return Result;
}
#endif
// End UObject interface

// Begin UGameUserSettings interface
void UBlueshiftGameUserSettings::SetToDefaults()
{
	Super::SetToDefaults();
	
	// Log
	UE_LOG(LogBlueshiftGameSettings, Log, TEXT("%hs: %s"), __FUNCTION__, *GetFullName());

	// Don't run when constructing the CDO as Blueprint variables marked with Config may do a Console Variable lookup
	// Which isn't thread-safe, and will be tricky to avoid
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		RunOnGameThread([this]()
		{
			// Blueprints don't know which thread we're in, so just ensure it's always the game thread
			if (GetBlueprint())
			{
				GetBlueprint()->BP_OnSetToDefaults();
			}
		});
	}
	
	// Accessibility Options
	ColorVisionDeficiencyMode = EColorVisionDeficiency::NormalVision;
	ColorVisionSeverity = 1.0f;
}

void UBlueshiftGameUserSettings::SaveSettings()
{
	QUICK_SCOPE_CYCLE_COUNTER(BlueshiftGameUserSettings_SaveSettings);

	// Log
	UE_LOG(LogBlueshiftGameSettings, Log, TEXT("%hs: %s"), __FUNCTION__, *GetFullName());
	
	// Save Blueprint Config
	if (SupportsBlueprint())
	{
		if (UBlueshiftBPGameUserSettings* const BPGameUserSettings = GetBlueprint())
		{
			// Mirrors UGameUserSettings::SaveSettings in order to save to the same ini file
			// and ensure scalability persists
			// NOTE: We must also call this before the Super call to ensure our config is also saved with the cloud data
			Scalability::SaveState(GIsEditor ? GEditorSettingsIni : GGameUserSettingsIni);
			BPGameUserSettings->SaveConfig(CPF_Config, *GGameUserSettingsIni);
		}
	}
	
	Super::SaveSettings();

	// Blueprint event
	if (SupportsBlueprint())
	{
		RunOnGameThread([this]()
	    {
			UE_LOG(LogBlueshiftGameSettings, Log, TEXT("%hs: %s: BP_OnSaveSettings"), __FUNCTION__, *GetFullName());
			if (UBlueshiftBPGameUserSettings* const BPGameUserSettings = GetBlueprint())
			{
				BPGameUserSettings->BP_OnSaveSettings();
			}
	    });
	}
}

void UBlueshiftGameUserSettings::LoadSettings(bool bForceReload)
{
	QUICK_SCOPE_CYCLE_COUNTER(BlueshiftGameUserSettings_LoadSettings);
	
	Super::LoadSettings(bForceReload);

	// Only instantiate/load Config for runtime objects
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		// Log
		UE_LOG(LogBlueshiftGameSettings, Log, TEXT("%hs: %s"), __FUNCTION__, *GetFullName());

		if (SupportsBlueprint())
		{
			// Load Blueprint Config
			if (UBlueshiftBPGameUserSettings* const BPGameUserSettings = GetBlueprint())
			{
				BPGameUserSettings->LoadConfig(nullptr, *GGameUserSettingsIni);
			}
			
			// Blueprint event
			RunOnGameThread([this]()
			{
				UE_LOG(LogBlueshiftGameSettings, Log, TEXT("%hs: %s: BP_OnLoadSettings"), __FUNCTION__, *GetFullName());
				if (UBlueshiftBPGameUserSettings* const BPGameUserSettings = GetBlueprint())
				{
					BPGameUserSettings->BP_OnLoadSettings();
				}
			});
		}
	}
}

void UBlueshiftGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	QUICK_SCOPE_CYCLE_COUNTER(BlueshiftGameUserSettings_ApplySettings);
	
	Super::ApplySettings(bCheckForCommandLineOverrides);

	// Log
	UE_LOG(LogBlueshiftGameSettings, Log, TEXT("%hs: %s"), __FUNCTION__, *GetFullName());

	if (SupportsBlueprint() || OnApplySettings.IsBound())
	{
		// Make sure we're running on the GT
		RunOnGameThread([this]()
		{
			if (UBlueshiftBPGameUserSettings* const BPGameUserSettings = GetBlueprint())
			{
				BPGameUserSettings->BP_OnApplySettings(false);

				// Broadcast event
				if (BPGameUserSettings->OnApplySettings.IsBound())
				{
					BPGameUserSettings->OnApplySettings.Broadcast(this, BPGameUserSettings, false);
				}
			}
			
			if (OnApplySettings.IsBound())
			{
				OnApplySettings.Broadcast(this, false);
			}
		});
	}
}

void UBlueshiftGameUserSettings::ApplyNonResolutionSettings()
{
	QUICK_SCOPE_CYCLE_COUNTER(BlueshiftGameUserSettings_ApplyNonResolutionSettings);
	
	Super::ApplyNonResolutionSettings();
	
	// Log
	UE_LOG(LogBlueshiftGameSettings, Log, TEXT("%hs: %s"), __FUNCTION__, *GetFullName());

	if (SupportsBlueprint() || OnApplySettings.IsBound())
	{
		RunOnGameThread([this]()
		{
			if (UBlueshiftBPGameUserSettings* const BPGameUserSettings = GetBlueprint())
			{
				BPGameUserSettings->BP_OnApplySettings(true);

				// Broadcast event
				if (BPGameUserSettings->OnApplySettings.IsBound())
				{
					BPGameUserSettings->OnApplySettings.Broadcast(this, BPGameUserSettings, true);
				}
			}

			if (OnApplySettings.IsBound())
			{
				OnApplySettings.Broadcast(this, true);
			}
		});
	}
}
// End UGameUserSettings interface

//-----------------------------------------------------------------------------------------------------------------
//	HELPERS
//-----------------------------------------------------------------------------------------------------------------

void UBlueshiftGameUserSettings::RunOnGameThread(const TFunction<void()>& Func)
{
	if (Func == nullptr)
	{
		return;
	}

	if (IsInGameThread())
	{
		Func();
	}
	else
	{
		AsyncTask(ENamedThreads::GameThread, [F = Func]()
		{
			if (F != nullptr)
			{
				F();
			}
		});
	}
}

UGameUserSettings* UBlueshiftGameUserSettings::GetGameUserSettings_Typed(TSubclassOf<UGameUserSettings> Class)
{
	return UGameUserSettings::GetGameUserSettings();
}

const UGameUserSettings* UBlueshiftGameUserSettings::GetGameUserSettings_Typed_Immutable(TSubclassOf<UGameUserSettings> Class)
{
	return UGameUserSettings::GetGameUserSettings();
}

//-----------------------------------------------------------------------------------------------------------------
//	BLUEPRINT
//-----------------------------------------------------------------------------------------------------------------

bool UBlueshiftGameUserSettings::SupportsBlueprint()
{
	return UBlueshiftGameDeveloperSettings::Get()->BlueprintClass.IsNull() == false;
}

UBlueshiftBPGameUserSettings* UBlueshiftGameUserSettings::GetBlueprint() const
{
	// Ensure Blueprint exists
	if (Blueprint == nullptr)
	{
		if (UBlueshiftGameDeveloperSettings::Get()->BlueprintClass.IsNull() == false)
		{
			Blueprint = NewObject<UBlueshiftBPGameUserSettings>(const_cast<UBlueshiftGameUserSettings*>(this), UBlueshiftGameDeveloperSettings::Get()->BlueprintClass.LoadSynchronous());
		}
	}

	return Blueprint;
}

UBlueshiftBPGameUserSettings* UBlueshiftGameUserSettings::BP_GetBlueprint(TSubclassOf<UBlueshiftBPGameUserSettings> Class) const
{
	return GetBlueprint();
}

const UBlueshiftBPGameUserSettings* UBlueshiftGameUserSettings::BP_GetBlueprint_ThreadSafe(TSubclassOf<UBlueshiftBPGameUserSettings> Class) const
{
	return GetBlueprint();
}

UBlueshiftBPGameUserSettings* UBlueshiftGameUserSettings::GetGameUserSettingsBlueprint()
{
	// Resolve Game User Settings
	UBlueshiftGameUserSettings* const GameUserSettings = UBlueshiftGameUserSettings::Get();
	if (GameUserSettings == nullptr)
	{
		UE_LOG(LogBlueshiftGameSettings, Error, TEXT("%hs: Game User Settings must use BlueshiftGameUserSettings or a subclass to support Blueprints"), __FUNCTION__);
		return nullptr;
	}

	return GameUserSettings->GetBlueprint();
}

UBlueshiftBPGameUserSettings* UBlueshiftGameUserSettings::BP_GetGameUserSettingsBlueprint(TSubclassOf<UBlueshiftBPGameUserSettings> Class)
{
	return GetGameUserSettingsBlueprint();
}

const UBlueshiftBPGameUserSettings* UBlueshiftGameUserSettings::BP_GetGetGameUserSettingsBlueprint_ThreadSafe(TSubclassOf<UBlueshiftBPGameUserSettings> Class)
{
	return GetGameUserSettingsBlueprint();
}

//-----------------------------------------------------------------------------------------------------------------
//	ACCESSIBILITY OPTIONS
//-----------------------------------------------------------------------------------------------------------------

void UBlueshiftGameUserSettings::UpdateColorVision() const
{
	UWidgetBlueprintLibrary::SetColorVisionDeficiencyType(ColorVisionDeficiencyMode, ColorVisionSeverity, true, false);
}

void UBlueshiftGameUserSettings::SetColorVisionDeficiencyMode(const EColorVisionDeficiency InColorVisionDeficiencyMode, const bool bUpdateColorVision)
{
	if (ColorVisionDeficiencyMode != InColorVisionDeficiencyMode)
	{
		ColorVisionDeficiencyMode = InColorVisionDeficiencyMode;

		if (bUpdateColorVision)
		{
			UpdateColorVision();
		}
	}
}

void UBlueshiftGameUserSettings::SetColorVisionSeverity(const float InColorVisionSeverity, const bool bUpdateColorVision)
{
	if (FMath::IsNearlyEqual(ColorVisionSeverity, InColorVisionSeverity) == false)
	{
		ColorVisionSeverity = FMath::Clamp(InColorVisionSeverity, 0.0f, 1.0f);
	
		if (bUpdateColorVision)
		{
			UpdateColorVision();
		}
	}
}

#undef LOCTEXT_NAMESPACE