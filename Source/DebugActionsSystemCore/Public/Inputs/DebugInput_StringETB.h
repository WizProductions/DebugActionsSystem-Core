// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DebugInputBase.h"
#include "DebugInput_StringETB.generated.h"

//#############################################################################
//##---------------------------------- CLASS --------------------------------##
//#############################################################################

/**
* An input that allows you to enter a string. \n
* Setup is optional.
* THIS INPUT CLASS IS NOT READY FOR PRODUCTION.
*/
UCLASS()
class DEBUGACTIONSSYSTEMCORE_API UDebugInput_StringETB : public UDebugInputBase {
	GENERATED_BODY()

//#############################################################################
//##--------------------------------- FIELDS --------------------------------##
//#############################################################################

	//==== References ====\\.
public:
	UPROPERTY(BlueprintReadOnly, Category = "References")
	TObjectPtr<class UEditableTextBox> MyEditableTextBox = NULL;

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################

public:
	UWidget* OnConfigureDebugInput_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "DebugActionsSystem|Setup")
	void Setup(const FText& InDebugInputTitle, const FText& HintText = INVTEXT(""), bool IsPassword = false);
	UFUNCTION(BlueprintPure, Category = "DebugActionsSystem")
	FString GetValue() const;
};