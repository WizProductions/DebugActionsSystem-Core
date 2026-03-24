// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DebugInputSlotBase.generated.h"

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/**
 * The widget base of the debug input, you can inherit from this base to customize how inputs are painted in the debug panel. \n
 * <b>Need to be added in the panel manually to create slots for debug inputs</b>.
 */
UCLASS(Abstract)
class DEBUGACTIONSSYSTEMCORE_API UDebugInputSlotBase : public UUserWidget {
	GENERATED_BODY()

//#############################################################################
//##---------------------------------- FIELDS -------------------------------##
//#############################################################################

protected:
	//==== Widgets Binding ====\\.
	//==== Properties ====\\.
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "UserInterface|Interaction")
	TObjectPtr<class UNamedSlot> NS_InputSlot;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "UserInterface|Painting")
	TObjectPtr<class UTextBlock> TB_InputTitle;

private:
	/** Slot in the debug panel canvas */
	UPROPERTY()
	TObjectPtr<class UCanvasPanelSlot> MySlot;
	/** Canvas Slot of Named Slot contains InputSlot */
	UPROPERTY()
	TObjectPtr<class UCanvasPanelSlot> InputWidgetSlot;
	
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
	bool IsUsed() { return bIsUsed; }

	UWidget* GetInputWidget() const;
	UCanvasPanelSlot* GetInputWidgetSlot() const { return InputWidgetSlot; }
	void SetInputWidget(class UDebugInput* InDebugInput);
	void RemoveInputWidget();

	void SetTitle(const FText& InTitle);	
};