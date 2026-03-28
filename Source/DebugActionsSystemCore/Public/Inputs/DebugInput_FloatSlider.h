// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DebugInput.h"
#include "DebugInput_FloatSlider.generated.h"

//#############################################################################
//##---------------------------------- CLASS --------------------------------##
//#############################################################################

/**
* An input that allows you to select a float value between ranges. \n
* Setup is optional, it allows you to customize slider.
*/
UCLASS()
class DEBUGACTIONSSYSTEMCORE_API UDebugInput_FloatSlider : public UDebugInput {
	GENERATED_BODY()

//#############################################################################
//##--------------------------------- FIELDS --------------------------------##
//#############################################################################
	
	//==== References ====\\.
public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class USpinBox> MySpinBox = NULL;

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################

public:
	virtual void ConfigureDebugInput_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "DebugActionsSystem|Setup")
	void Setup(const FText& InDebugInputTitle, FVector2f BothMinMaxValue, float DefaultValue = 0.f);
	UFUNCTION(BlueprintPure, Category = "DebugActionsSystem")
	float GetValue() const;
};