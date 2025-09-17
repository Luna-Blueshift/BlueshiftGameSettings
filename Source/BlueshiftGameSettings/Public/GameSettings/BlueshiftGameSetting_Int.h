// Copyright (c) Blueshift Interactive Ltd (2025)

#pragma once

/// Blueshift includes
#include "GameSettings/BlueshiftGameSetting.h"

#include "BlueshiftGameSetting_Int.generated.h"

UCLASS(Blueprintable, BlueprintType, Abstract, meta = (DisplayName = "Blueshift Game Setting (Int)"))
class BLUESHIFTGAMESETTINGS_API UBlueshiftGameSetting_Int : public UBlueshiftGameSetting
{
	GENERATED_BODY()

public:
	// Begin UBlueshiftGameSetting interface
	virtual FText GetDisplayValue_Implementation() const override;
	// End UBlueshiftGameSetting interface

	/** Override this to set the real value, i.e. in the Game User Settings. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Settings")
	void SetValue(const int32 Value);
	virtual void SetValue_Implementation(const int32 Value);

	/** Override this to get the value, i.e. from the Game User Settings. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Settings")
	int32 GetValue() const;
	virtual int32 GetValue_Implementation() const { return INDEX_NONE; }

	/** Get the options available to the user. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Settings")
	TMap<int32, FText> GetOptions() const;
	virtual TMap<int32, FText> GetOptions_Implementation() const { return { }; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Settings")
	FText GetDisplayValueFromInt(const int32 Value) const;
	virtual FText GetDisplayValueFromInt_Implementation(const int32 Value) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = "false", Category = "Settings")
	FText GetOption(const int32 Index) const;
	
	/** Get the value (text) for the option we currently have selected, if we have implemented this. */
	UFUNCTION(BlueprintCallable, BlueprintPure = "false", Category = "Settings")
	FText GetCurrentOption() const;

protected:
	int32 GetValueByPredicate(const TFunction<bool(int32, int32)>& SortFunc) const;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure = "false", Category = "Settings")
	virtual int32 GetMinValue() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure = "false", Category = "Settings")
	virtual int32 GetMaxValue() const;
};