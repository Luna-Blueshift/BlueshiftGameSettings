// Copyright (c) Blueshift Interactive Ltd (2025)

#include "UserSettings/BlueshiftBPGameUserSettings.h"

/// UE includes
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

/// Blueshift includes
#include "UserSettings/BlueshiftGameUserSettings.h"

#define LOCTEXT_NAMESPACE "BlueshiftGameSettingsBlueprint"

// Begin UObject interface
#if WITH_EDITOR
EDataValidationResult UBlueshiftBPGameUserSettings::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

#if WITH_EDITORONLY_DATA
	if (bWarnConfigVariableUsage)
	{
		const UClass* const ThisClass = GetClass();
		check(ThisClass != nullptr);

		// Iterate all Blueprint properties and check if they have the Config flag on them
		// Ideally every property should contain the config flag here
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

			// Property doesn't contain Config, thus won't be saved
			if (Property->HasAnyPropertyFlags(CPF_Config) == false)
			{
				FFormatNamedArguments FormatNamedArguments;
				FormatNamedArguments.Emplace(TEXT("PropertyName"), FText::FromString(*Property->GetName()));
				FormatNamedArguments.Emplace(TEXT("BlueprintName"), FText::FromString(*ThisClass->GetName()));
			
				Context.AddWarning(FText::Format(LOCTEXT("ConfigPropertyWarning", "Property '{PropertyName}' in Blueprint subclass '{BlueprintName}' is not marked as Config, so it won't be saved with GameUserSettings. Please mark as Config."), FormatNamedArguments));
				// NOTE: Do not block cook by returning invalid here, just warn
			}
		}
	}
#endif
	
	return Result;
}
#endif
// End UObject interface

UBlueshiftGameUserSettings* UBlueshiftBPGameUserSettings::GetGameUserSettings() const
{
	return Cast<UBlueshiftGameUserSettings>(GetOuter());
}

#undef LOCTEXT_NAMESPACE