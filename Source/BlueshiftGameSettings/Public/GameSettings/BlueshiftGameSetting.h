// Copyright (c) Blueshift Interactive Ltd (2025)

#pragma once

/// UE includes
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"

#include "BlueshiftGameSetting.generated.h"

UENUM(BlueprintType, meta = (DisplayName = "Game Setting Visibility (Blueshift)"))
enum class EBlueshiftGameSettingVisibility : uint8
{
	Enabled,
	Disabled,
	Hidden,
};

UCLASS(NotBlueprintable, BlueprintType, Abstract, meta = (DisplayName = "Blueshift Game Setting (Base)"))
class BLUESHIFTGAMESETTINGS_API UBlueshiftGameSetting
	: public UObject
	, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Settings")
	static UBlueshiftGameSetting* GetGameSettingCDO(const TSubclassOf<UBlueshiftGameSetting> Class)
	{
		return GetMutableDefault<UBlueshiftGameSetting>(Class);
	}

	// Begin IGameplayTagAssetInterface
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override
	{
		TagContainer.AppendTags(Tags);
	}
	// End IGameplayTagAssetInterface

protected:
	/** Name of this setting to display to the user. */
	UPROPERTY(EditAnywhere, Category = "Settings")
	FText DisplayName;

	/** Description to display in the details UI. */
	UPROPERTY(EditAnywhere, Category = "Settings")
	FText Description;

	/** Settings Tags. */
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (Categories = "Settings"))
	FGameplayTagContainer Tags;
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Settings")
	FText GetDisplayName() const;
	virtual FText GetDisplayName_Implementation() const { return DisplayName; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Settings")
	FText GetDescription() const;
	virtual FText GetDescription_Implementation() const { return Description; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Settings")
	FText GetDisplayValue() const;
	virtual FText GetDisplayValue_Implementation() const { return FText::GetEmpty(); }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Settings")
	EBlueshiftGameSettingVisibility GetVisibility() const;
	virtual EBlueshiftGameSettingVisibility GetVisibility_Implementation() const { return EBlueshiftGameSettingVisibility::Enabled; }
};
