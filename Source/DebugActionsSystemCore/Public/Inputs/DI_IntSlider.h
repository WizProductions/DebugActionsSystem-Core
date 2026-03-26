// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DebugInput.h"
#include "DI_IntSlider.generated.h"

//#############################################################################
//##---------------------------------- CLASS --------------------------------##
//#############################################################################

/**
* An input that allows you to select an int value between ranges. \n
* <b>Setup is recommended but optional</b>
*/
UCLASS()
class DEBUGACTIONSSYSTEMCORE_API UDI_IntSlider : public UDebugInput {
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
	
	void Setup(FString InDebugInputTitle, FIntVector2 BothMinMaxValue, int DefaultValue = 0);
	int GetValue() const;
};