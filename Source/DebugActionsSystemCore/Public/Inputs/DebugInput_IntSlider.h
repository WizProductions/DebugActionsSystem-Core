// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DebugInputBase.h"
#include "DebugInput_IntSlider.generated.h"

//#############################################################################
//##---------------------------------- CLASS --------------------------------##
//#############################################################################

/**
* An input that allows you to select an int value between ranges. \n
* Setup is optional, it allows you to customize slider.
*/
UCLASS()
class DEBUGACTIONSSYSTEMCORE_API UDebugInput_IntSlider : public UDebugInputBase {
	GENERATED_BODY()

//#############################################################################
//##--------------------------------- FIELDS --------------------------------##
//#############################################################################

	//==== References ====\\.
public:
	UPROPERTY(BlueprintReadOnly, Category = "References")
	TObjectPtr<class USpinBox> MySpinBox = NULL;

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################

public:
	virtual void ConfigureDebugInput_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "DebugActionsSystem|Setup")
	void Setup(const FString& InDebugInputTitle, const FIntVector2& BothMinMaxValue, int DefaultValue = 0);
	UFUNCTION(BlueprintPure, Category = "DebugActionsSystem")
	int GetValue() const;
};