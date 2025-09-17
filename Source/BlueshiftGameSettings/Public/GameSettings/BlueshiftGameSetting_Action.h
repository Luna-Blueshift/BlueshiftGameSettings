// Copyright (c) Blueshift Interactive Ltd (2025)

#pragma once

/// Blueshift includes
#include "GameSettings/BlueshiftGameSetting.h"

#include "BlueshiftGameSetting_Action.generated.h"

/**
 *	Game Setting which triggers an action - will be bound to a button.
 */
UCLASS(Blueprintable, BlueprintType, Abstract, meta = (DisplayName = "Blueshift Game Setting (Action)"))
class BLUESHIFTGAMESETTINGS_API UBlueshiftGameSetting_Action : public UBlueshiftGameSetting
{
	GENERATED_BODY()

public:
	/** Override this method to implement your action. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Settings")
	void ExecuteAction() const;
	virtual void ExecuteAction_Implementation() const { }
};