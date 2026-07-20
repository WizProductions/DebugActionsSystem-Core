// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Structs/DISlotProperties.h"
#include "DebugInputBase.generated.h"

class UDebugSubsystem;

//#############################################################################
//##---------------------------------- CLASS --------------------------------##
//#############################################################################

/**
* Debug input class is used to allow user to select a value sent to action.
* You need to add manually GetValue method, and Setup if needed.
*/
UCLASS(Blueprintable, Abstract, HideDropdown)
class DEBUGACTIONSSYSTEMCORE_API UDebugInputBase : public UObject {
	GENERATED_BODY()

//#############################################################################
//##--------------------------------- FIELDS --------------------------------##
//#############################################################################
	
	//==== Settings ====\\.
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	FDISlotProperties MySlotProperties;
	
	//==== References ====\\.
protected:
	UPROPERTY(BlueprintReadOnly, Category = References)
	TObjectPtr<class UDebugInputSlotWidgetBase> MyDebugInputSlotWidget = NULL;
private:
	UPROPERTY()
	TObjectPtr<class UWidget> MyInputDataWidget = NULL;

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################
	
public:
	/** Only called by the Subsystem. */
	void ConfigureDebugInput();
	void AddedToSlot(class UDebugInputSlotWidgetBase* InInputSlot);
	void RemoveFromSlot(class UDebugInputSlotWidgetBase* InInputSlot);
	
	UFUNCTION(BlueprintPure, Category = References)
	UDebugSubsystem* GetDebugSubsystem() const;
	UFUNCTION(BlueprintPure, Category = References)
	UWidget* GetInputDataWidget() const { return MyInputDataWidget; }
	UFUNCTION(BlueprintPure, Category = References)
	UDebugInputSlotWidgetBase* GetDebugInputSlotWidget() const { return MyDebugInputSlotWidget; }
	
	/** When the widget become visible and set to a slot. */
	UFUNCTION(BlueprintNativeEvent, Category = DebugActionsSystem)
	void OnAddedToSlot(class UDebugInputSlotWidgetBase* InInputSlot);
	
	/** When the widget become hidden and unset from a slot. */
	UFUNCTION(BlueprintNativeEvent, Category = DebugActionsSystem)
	void OnRemovedFromSlot(class UDebugInputSlotWidgetBase* InInputSlot);
	
	/** Returns the widget created and configured; Uses GetNewWidgetInDebugPanel() to get a registered widget. */
	UFUNCTION(BlueprintNativeEvent, Category = DebugActionsSystem)
	UPARAM(DisplayName = "Configured Widget") UWidget* OnConfigureDebugInput();
};