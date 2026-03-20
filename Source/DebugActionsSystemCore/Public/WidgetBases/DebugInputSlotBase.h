// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DebugInputSlotBase.generated.h"

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/**
 * The Debug Input Slot, it needs to be added in debug tools widget.
 */
UCLASS()
class DEBUGACTIONSSYSTEMCORE_API UDebugInputSlotBase : public UUserWidget {
	GENERATED_BODY()

//##############################################################################
//##------------------------------- ATTRIBUTES -------------------------------##
//##############################################################################

protected:
	//==== Properties ====\\.
	UPROPERTY(BlueprintSetter = "SetInputSlotWidget")
	TObjectPtr<class UNamedSlot> InputNamedSlot;
	UPROPERTY(BlueprintSetter = "SetInputTitleWidget")
	TObjectPtr<class UTextBlock> InputTitle;

private:
	/** Debug tools canvas */
	UPROPERTY()
	TObjectPtr<class UCanvasPanelSlot> MySlot;
	/**
	 * Canvas Slot of Named Slot contains InputSlot
	 */
	UPROPERTY()
	TObjectPtr<class UCanvasPanelSlot> InputWidgetSlot;
	
	//==== Flags ====\\.
	bool bIsUsed;
	
//#############################################################################
//##------------------------------- FUNCTIONS -------------------------------##
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

protected:
	UFUNCTION(BlueprintCallable)
	void SetInputSlotWidget(UNamedSlot* InNamedSlot);
	UFUNCTION(BlueprintCallable)
	void SetInputTitleWidget(UTextBlock* InTextBlock);
};