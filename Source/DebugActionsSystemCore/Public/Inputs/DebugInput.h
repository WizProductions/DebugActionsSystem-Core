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
UCLASS(Blueprintable, Abstract, HideDropdown)
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
	TObjectPtr<class UWidget> MyInputDataWidget = NULL;
	UPROPERTY(BlueprintReadOnly, Category = "References")
	class UDebugInputSlotWidgetBase* MyDebugInputSlotWidget = NULL;

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################
	
public:
	UFUNCTION(BlueprintCallable, Category = "References")
	UWidget* GetMyInputDataWidget() { return MyInputDataWidget; }
	UFUNCTION(BlueprintCallable, Category = "References")
	UDebugInputSlotWidgetBase* GetMyDebugInputSlotWidget() const { return MyDebugInputSlotWidget; }
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DebugActionsSystem")
	void OnAddedToSlot(class UDebugInputSlotWidgetBase* InSlot);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DebugActionsSystem")
	void OnRemovedFromSlot(class UDebugInputSlotWidgetBase* InSlot);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DebugActionsSystem")
	void SetWidgetVisibility(ESlateVisibility InVisibility);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DebugActionsSystem")
	void RefreshValues();
};