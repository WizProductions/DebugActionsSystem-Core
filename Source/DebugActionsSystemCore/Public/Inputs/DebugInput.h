// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DebugInput.generated.h"

//#############################################################################
//##---------------------------------- CLASS --------------------------------##
//#############################################################################


/**
* Debug input class is used to allow user to select a value sent to action.
* You need to add manually GetValue method, and Setup if needed.
*/
UCLASS(Blueprintable, Abstract, HideDropdown)
class DEBUGACTIONSSYSTEMCORE_API UDebugInput : public UObject {
	GENERATED_BODY()

//#############################################################################
//##--------------------------------- FIELDS --------------------------------##
//#############################################################################


	//==== Settings ====\\.
protected:
	UPROPERTY(BlueprintReadWrite, Category = "Settings")
	FText DebugInputTitle = FText::FromString("Default_Title");
	UPROPERTY(BlueprintReadWrite, Category = "Settings")
	FVector2D DebugInputSize = FVector2D(120, 65);
	
protected:
	//==== References ====\\.
	UPROPERTY(BlueprintReadOnly, Category = "References")
	class UDebugSubsystem* MyDebugSubsystem = NULL;
	UPROPERTY(BlueprintReadWrite, Category = "References")
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
	/** When the widget become visible and set to a slot. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DebugActionsSystem")
	void OnAddedToSlot(class UDebugInputSlotWidgetBase* InSlot);
	/** When the widget become hidden and unset from a slot. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DebugActionsSystem")
	void OnRemovedFromSlot(class UDebugInputSlotWidgetBase* InSlot);
	/** Get the widget as you want and customize it. \n
	 *  DON'T FORGOT to set MyInputDataWidget with your new widget.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DebugActionsSystem")
	void ConfigureDebugInput();
	
	friend class UDebugSubsystem;
};