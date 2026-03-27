// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DebugInput.h"
#include "DI_FloatSlider.generated.h"

//#############################################################################
//##---------------------------------- CLASS --------------------------------##
//#############################################################################

/**
* An input that allows you to select a float value between ranges. \n
* <b>Setup is recommended but optional</b>
*/
UCLASS()
class DEBUGACTIONSSYSTEMCORE_API UDI_FloatSlider : public UDebugInput {
	GENERATED_BODY()

//#############################################################################
//##--------------------------------- FIELDS --------------------------------##
//#############################################################################
	
	//==== Exposed Properties ====\\.
public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class USpinBox> MySpinBox = NULL;

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################

public:
	virtual void PostInitProperties() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DebugActionsSystem|Setup")
	void Setup(const FText& InDebugInputTitle, FVector2f BothMinMaxValue, float DefaultValue = 0.f);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DebugActionsSystem")
	float GetValue() const;
};