// Copyright (c) Blueshift Interactive Ltd (2025)

#include "GameSettings/BlueshiftGameSetting_Resolution.h"

/// UE includes
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"

/// Blueshift includes
#include "BlueshiftGameSettingsLog.h"

FText GetResolutionDisplayValue(const FIntPoint& Resolution)
{
	FFormatNamedArguments FormatNamedArguments;
	FormatNamedArguments.Emplace(TEXT("ResolutionX"), Resolution.X);
	FormatNamedArguments.Emplace(TEXT("ResolutionY"), Resolution.Y);
	return FText::Format(INVTEXT("{ResolutionX} x {ResolutionY}"), FormatNamedArguments);
}

TArray<FIntPoint> GetAvailableResolutions()
{
	// Resolve Window Mode
	EWindowMode::Type WindowMode = EWindowMode::Type::Windowed;
	if (GEngine != nullptr
		&& GEngine->GetGameUserSettings() != nullptr)
	{
		WindowMode = GEngine->GetGameUserSettings()->GetFullscreenMode();
	}

	// Resolve resolutions
	TArray<FIntPoint> AvailableResolutions;
	switch (WindowMode)
	{
		case EWindowMode::Type::Fullscreen:
		{
			UKismetSystemLibrary::GetSupportedFullscreenResolutions(OUT AvailableResolutions);
			break;
		}

		default:
		case EWindowMode::Type::WindowedFullscreen:
		case EWindowMode::Type::Windowed:
		{
			UKismetSystemLibrary::GetConvenientWindowedResolutions(OUT AvailableResolutions);
			break;
		}
	}
	
	return AvailableResolutions;
}

// Begin UBlueshiftGameSetting interface
FText UBlueshiftGameSetting_Resolution::GetDisplayValue_Implementation() const
{
	return GetResolutionDisplayValue(GetValue());
}

void UBlueshiftGameSetting_Resolution::SetValue_Implementation(const int32 Value)
{
	Super::SetValue_Implementation(Value);

	const FIntPoint Resolution = GetScreenResolutionFromIndex(Value);
	if (0 < Resolution.X
		&& 0 < Resolution.Y)
	{
		if (GEngine != nullptr
			&& GEngine->GetGameUserSettings() != nullptr)
		{
			GEngine->GetGameUserSettings()->SetScreenResolution(bCheckForCommandLineOverrides);
			UE_LOG(LogBlueshiftGameSettings, Log, TEXT("%s Set Resolution to %s"), *GetName(), *GetResolutionDisplayValue(Resolution).ToString());
		}
	}
}

int32 UBlueshiftGameSetting_Resolution::GetValue_Implementation() const
{
	const TArray<FIntPoint> AvailableResolutions = GetAvailableResolutions();
	if (GEngine != nullptr
		&& GEngine->GetGameUserSettings() != nullptr)
	{
		const FIntPoint ScreenResolution = GEngine->GetGameUserSettings()->GetScreenResolution();
		return AvailableResolutions.Find(ScreenResolution);
	}

	// Fallback
	return Super::GetValue_Implementation();
}

TMap<int32, FText> UBlueshiftGameSetting_Resolution::GetOptions_Implementation() const
{
	TMap<int32, FText> Result;
	
	const TArray<FIntPoint> AvailableResolutions = GetAvailableResolutions();
	for (int32 Index = 0; Index < AvailableResolutions.Num(); Index++)
	{
		const FIntPoint& Resolution = AvailableResolutions[Index];
		const FText UserFacingText = GetResolutionDisplayValue(Resolution);
		Result.Emplace(Index, UserFacingText);
	}
	
	return Result;
}
// End UBlueshiftGameSetting interface

FIntPoint UBlueshiftGameSetting_Resolution::GetScreenResolutionFromIndex(const int32 Index) const
{
	const TArray<FIntPoint> AvailableResolutions = GetAvailableResolutions();
	if (AvailableResolutions.IsValidIndex(Index))
	{
		return AvailableResolutions[Index];
	}

	return FIntPoint(INDEX_NONE, INDEX_NONE);
}