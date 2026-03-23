// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DebugInput.generated.h"

//#############################################################################
//##---------------------------------- CLASS --------------------------------##
//#############################################################################


/**
* Debug input class is used to allow user to select a value sent to action.
*/
UCLASS(Blueprintable, Abstract)
class DEBUGACTIONSSYSTEMCORE_API UDebugInput : public UObject {
	GENERATED_BODY()

//#############################################################################
//##--------------------------------- FIELDS --------------------------------##
//#############################################################################

	//==== Settings ====\\.
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Settings")
	FText DebugInputTitle = FText::FromString("Default_Title");
	UPROPERTY(BlueprintReadOnly, Category = "Settings")
	FVector2D DebugInputSize = FVector2D(120, 65);
	
protected:
	//==== Properties ====\\.
	UPROPERTY(BlueprintReadOnly, Category = "References")
	TObjectPtr<class UWidget> MyWidget = NULL;
	UPROPERTY(BlueprintReadOnly, Category = "References")
	class UDebugInputSlotBase* MyDebugInputSlot = NULL;

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################
	
public:
	UFUNCTION(BlueprintCallable, Category = "References")
	UWidget* GetMyWidget() { return MyWidget; }
	
	virtual void OnAddedToSlot(class UDebugInputSlotBase* InSlot);
	virtual void OnRemovedFromSlot(class UDebugInputSlotBase* InSlot);
	virtual void SetWidgetVisibility(ESlateVisibility InVisibility);
	virtual void RefreshValues();
};