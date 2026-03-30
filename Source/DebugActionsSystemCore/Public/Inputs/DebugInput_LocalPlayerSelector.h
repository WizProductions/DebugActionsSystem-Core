// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DebugInputBase.h"
#include "DebugInput_LocalPlayerSelector.generated.h"

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/**
* An input that allows you to select a local player connected in local (Alone or Splitscreen). \n
*/
UCLASS()
class DEBUGACTIONSSYSTEMCORE_API UDebugInput_LocalPlayerSelector : public UDebugInputBase {
	GENERATED_BODY()

//##############################################################################
//##--------------------------------- FIELDS ---------------------------------##
//##############################################################################

public:
	//==== References ====\\.
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UComboBoxString> MyComboBox = NULL;

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################

public:
	virtual void ConfigureDebugInput_Implementation() override;
	
	UFUNCTION(BlueprintCallable, Category = "DebugActionsSystem")
	void RefreshDebugInputConfiguration();

	UFUNCTION(BlueprintPure, Category = "DebugActionsSystem")
	ULocalPlayer* GetValue() const;
};