// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "DebugActionBase.h"
#include "DebugAction_CommandBase.generated.h"

class UDebugInput_LocalPlayerSelector;

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################


/**
 * A base of Action use a commands.
 */
UCLASS(BlueprintType, EditInlineNew, Abstract, HideDropdown)
class DEBUGACTIONSSYSTEMCORE_API UDebugAction_CommandBase : public UDebugActionBase {
	GENERATED_BODY()

//#############################################################################
//##--------------------------------- FIELDS --------------------------------##
//#############################################################################
	
protected:
	//==== Properties ====\\.
	UPROPERTY(BlueprintReadOnly, Category = "References")
	TObjectPtr<UDebugInput_LocalPlayerSelector> MyPlayerSelector = NULL;
	
	UPROPERTY(BlueprintReadWrite, Category = "Hidden|Properties")
	bool bColorFeedbackOnSuccess = true;
	
//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################
	
public:
	void OnParentFolderIsDeveloped_Implementation(UDebugActionFolder* ParentFolder) override;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Command")
	bool CanExecuteCommand(UWorld* ContextWorld) const;
	UFUNCTION(BlueprintNativeEvent, Category = "Command")
	void GetCommand(FString& Cmd);
	UFUNCTION(BlueprintNativeEvent, Category = "Command")
	void GetMessage(ULocalPlayer* Executer, FString& Message);
	UFUNCTION(BlueprintNativeEvent, Category = "Command")
	void PreCommandExecute(const FString& Cmd);
	
	EDebugActionResult OnExecuteDebugAction_Implementation() override;
};