// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DebugActionWidgetBase.generated.h"

enum class EDebugActionResult : UINT8;

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/**
 * The widget base of the debug action, you can inherit from this base to customize how actions are painted in the debug panel.
 */
UCLASS(Abstract)
class DEBUGACTIONSSYSTEMCORE_API UDebugActionWidgetBase : public UUserWidget {
	GENERATED_BODY()

//#############################################################################
//##--------------------------------- FIELDS --------------------------------##
//#############################################################################

protected:
	UPROPERTY(BlueprintReadOnly, Category = "References")
	TObjectPtr<class UDebugActionBase> MyDebugAction = NULL;

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################	

public:
#if UE_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DebugActionsSystem")
	bool InitDebugActionWidget(UDebugActionBase* LinkedDebugAction);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DebugActionsSystem")
	EDebugActionResult ExecuteAction();
};