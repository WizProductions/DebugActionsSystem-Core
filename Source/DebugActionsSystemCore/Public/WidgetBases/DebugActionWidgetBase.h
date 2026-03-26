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
	UPROPERTY(BlueprintGetter="GetMyDebugAction")
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
	UFUNCTION(Category = "Debug Action", BlueprintPure)
	UDebugActionBase* GetMyDebugAction() const { return MyDebugAction; }
	
	UFUNCTION(Category = "Debug Action", BlueprintCallable, BlueprintNativeEvent)
	bool InitDebugActionWidget(UDebugActionBase* LinkedDebugAction);
	
	UFUNCTION(Category = "Debug Action", BlueprintCallable)
	EDebugActionResult ExecuteMyDebugAction();
};