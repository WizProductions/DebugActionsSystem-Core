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
* An input that retrieves all <b>Class</b> currently loaded by engine allows you to select one to apply an action in all object of this class type. \n
* <b>Needs a Setup</b>.
*/
UCLASS()
class DEBUGACTIONSSYSTEMCORE_API UDI_ClassSelector : public UDebugInput {
	GENERATED_BODY()

//##############################################################################
//##--------------------------------- FIELDS ---------------------------------##
//##############################################################################

	//==== Exposed Properties ====\\.
public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UComboBoxString> MyClassFilterComboBox = NULL;
	
	//==== Properties ====\\.
protected:
	UPROPERTY()
	TSubclassOf<UObject> ClassFilter = UObject::StaticClass();
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