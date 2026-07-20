// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "DebugAction_CommandBase.h"
#include "DebugAction_OpenLevel.generated.h"

class UDebugInput_ObjectSelector;
class UDebugInput_StringETB;

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/**
 * A simple debug action that seamless travel with a simple click.
 */
UCLASS(BlueprintType, EditInlineNew, meta=(ToolTip="Fire a open level to selected level."))
class DEBUGACTIONSSYSTEMCORE_API UDebugAction_OpenLevel : public UDebugAction_CommandBase {
	GENERATED_BODY()
	
protected:
	UDebugAction_OpenLevel();

//#############################################################################
//##--------------------------------- FIELDS --------------------------------##
//#############################################################################

protected:
	//==== Settings ====\\.
	/** 
	 * Search path filter, can be left blank to not filter the search.
	 * eg: "/Game", "/Engine"...
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	TArray<FName> SearchPathFilters; 
	
	//==== Properties ====\\.
	UPROPERTY(BlueprintReadOnly, Category = "References")
	TObjectPtr<UDebugInput_ObjectSelector> MyLevelSelector = NULL; 
	
	UPROPERTY(BlueprintReadOnly, Category = "References")
	TObjectPtr<UDebugInput_StringETB> MyStringInput = NULL; 
	
//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################
	
public:
	FText GetDebugActionTitle_Implementation() const override { return FText::FromString("Open Level"); }
	void OnParentFolderIsDeveloped_Implementation(UDebugActionFolder* ParentFolder) override;

	void GetCommand_Implementation(FString& Cmd) override;
	void GetMessage_Implementation(ULocalPlayer* Executer, FString& Message) override;
};