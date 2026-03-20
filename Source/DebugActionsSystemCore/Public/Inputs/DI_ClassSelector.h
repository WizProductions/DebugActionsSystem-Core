// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DebugInput.h"
#include "GameFramework/Actor.h"
#include "DI_ClassSelector.generated.h"

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/**
* Default.
*/
UCLASS()
class DEBUGACTIONSSYSTEMCORE_API UDI_ClassSelector : public UDebugInput {
	GENERATED_BODY()

//##############################################################################
//##--------------------------------- FIELDS ---------------------------------##
//##############################################################################

	//==== Properties ====\\.
protected:
	UPROPERTY()
	TSubclassOf<UObject> ClassFilter = UObject::StaticClass();
	UPROPERTY()
	TObjectPtr<class UComboBoxString> MyClassFilterComboBox = nullptr;
	UPROPERTY()
	TArray<UClass*> AllDerivedClasses;

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################

protected:
	virtual void PostInitProperties() override;
	
public:
	void Setup(FText InDebugInputTitle, TSubclassOf<UObject> InClassFilter);
	TSubclassOf<class UObject> GetValue() const;
};