// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DebugInput.h"
#include "DI_LocalPlayerSelector.generated.h"

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/**
* An input that allows you to select a local player connected in local (Alone or Splitscreen). \n
*/
UCLASS()
class DEBUGACTIONSSYSTEMCORE_API UDI_LocalPlayerSelector : public UDebugInput {
	GENERATED_BODY()

//##############################################################################
//##--------------------------------- FIELDS ---------------------------------##
//##############################################################################

public:
	//==== Exposed Properties ====\\.
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UComboBoxString> MyComboBox = NULL;

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################

protected:
	virtual void PostInitProperties() override;

public:
	ULocalPlayer* GetValue() const;
	virtual void RefreshValues() override;
};