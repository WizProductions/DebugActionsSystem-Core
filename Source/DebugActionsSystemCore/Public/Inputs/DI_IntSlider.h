// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DebugInput.h"
#include "DI_IntSlider.generated.h"

//#############################################################################
//##---------------------------------- CLASS --------------------------------##
//#############################################################################

/**
* Default.
*/
UCLASS()
class DEBUGACTIONSSYSTEMCORE_API UDI_IntSlider : public UDebugInput {
	GENERATED_BODY()

//#############################################################################
//##--------------------------------- FIELDS --------------------------------##
//#############################################################################

	UPROPERTY()
	TObjectPtr<class USpinBox> MySpinBox;

//#############################################################################
//##------------------------------- FUNCTIONS -------------------------------##
//#############################################################################

public:
	virtual void PostInitProperties() override;
	
	void Setup(FString InDebugInputTitle, FIntVector2 BothMinMaxValue, int DefaultValue = 0);
	int GetValue() const;
};