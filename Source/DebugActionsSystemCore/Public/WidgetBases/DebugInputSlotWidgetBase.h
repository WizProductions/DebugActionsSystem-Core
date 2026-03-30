// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DebugInputSlotWidgetBase.generated.h"

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/**
 * The widget base of the debug input, you can inherit from this base to customize how inputs are painted in the debug panel. \n
 * <b>Need to be added in the panel manually to create slots for debug inputs</b>.
 */
UCLASS(Abstract)
class DEBUGACTIONSSYSTEMCORE_API UDebugInputSlotWidgetBase : public UUserWidget {
	GENERATED_BODY()

//#############################################################################
//##---------------------------------- FIELDS -------------------------------##
//#############################################################################

protected:
	//==== Widgets Binding ====\\.
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "UserInterface|Interaction")
	TObjectPtr<class UNamedSlot> NS_InputSlot;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "UserInterface|Painting")
	TObjectPtr<class UTextBlock> TB_InputTitle;
	
	//==== Properties ====\\.
protected:
	/** Slot in the debug panel canvas */
	UPROPERTY(BlueprintReadOnly, Category = "UserInterface|Interaction|Slot")
	TObjectPtr<class UCanvasPanelSlot> MySlot;
	/** Canvas Slot of Named Slot contains InputSlot */
	UPROPERTY(BlueprintReadOnly, Category = "UserInterface|Interaction|Slot")
	TObjectPtr<class UCanvasPanelSlot> NamedSlotWidgetSlot;
	UPROPERTY(BlueprintReadOnly, Category = "References")
	TObjectPtr<class UDebugInputBase> MyDebugInput;
	
	//==== Flags ====\\.
	bool bIsUsed : 1 = false;
	
//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################

public:
	virtual void NativeConstruct() override;

#if UE_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

public:
	UFUNCTION(BlueprintCallable, Category = "Properties|Flags")
	FORCEINLINE bool IsUsed() const { return bIsUsed; }

	UFUNCTION(BlueprintNativeEvent, Category = "DebugActionsSystem|References")
	UCanvasPanelSlot* GetInputWidgetSlot() const;
	UFUNCTION(BlueprintNativeEvent, Category = "DebugActionsSystem|References")
	UWidget* GetInputWidget() const;
	UFUNCTION(BlueprintNativeEvent, Category = "DebugActionsSystem")
	void SetInputWidget(class UDebugInputBase* InDebugInput);
	UFUNCTION(BlueprintNativeEvent, Category = "DebugActionsSystem")
	void RemoveInputWidget();
	UFUNCTION(BlueprintNativeEvent, Category = "DebugActionsSystem")
	void SetTitle(const FText& InTitle);	
};