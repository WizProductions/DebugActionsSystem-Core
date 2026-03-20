// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DebugActionWidgetBase.generated.h"

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

enum class EDebugActionResult : UINT8;
/*
 *
*/
UCLASS(Abstract)
class DEBUGACTIONSSYSTEMCORE_API UDebugActionWidgetBase : public UUserWidget {
	GENERATED_BODY()

//##############################################################################
//##------------------------------- ATTRIBUTES -------------------------------##
//##############################################################################

private:
	UPROPERTY(BlueprintGetter="GetMyDebugAction")
	TObjectPtr<class UDebugActionBase> MyDebugAction = nullptr;

//#############################################################################
//##------------------------------- FUNCTIONS -------------------------------##
//#############################################################################	

public:
#if UE_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
	/** return the linked debug action */
	UFUNCTION(Category = "Debug Action", BlueprintPure)
	UDebugActionBase* GetMyDebugAction() const { return MyDebugAction; }
	
	UFUNCTION(Category = "Debug Action", BlueprintCallable, BlueprintNativeEvent)
	bool InitDebugActionWidget(UDebugActionBase* LinkedDebugAction);
	
	UFUNCTION(Category = "Debug Action", BlueprintCallable)
	EDebugActionResult ExecuteMyDebugAction();
};