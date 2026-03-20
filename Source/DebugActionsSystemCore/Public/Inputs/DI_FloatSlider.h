// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DebugInput.h"
#include "DI_FloatSlider.generated.h"

//#############################################################################
//##---------------------------------- CLASS --------------------------------##
//#############################################################################

/**
* Default.
*/
UCLASS()
class DEBUGACTIONSSYSTEMCORE_API UDI_FloatSlider : public UDebugInput {
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
	
	void Setup(FText InDebugInputTitle, FVector2f BothMinMaxValue, float DefaultValue = 0.f);
	float GetValue() const;
};