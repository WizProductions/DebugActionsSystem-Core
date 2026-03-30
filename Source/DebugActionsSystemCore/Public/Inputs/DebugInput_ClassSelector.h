// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DebugInputBase.h"
#include "GameFramework/Actor.h"
#include "DebugInput_ClassSelector.generated.h"

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/**
* An input that retrieves all Class currently loaded by engine allows you to select one to apply an action in all object of this class type. \n
* NEEDS A SETUP.
*/
UCLASS()
class DEBUGACTIONSSYSTEMCORE_API UDebugInput_ClassSelector : public UDebugInputBase {
	GENERATED_BODY()

//##############################################################################
//##--------------------------------- FIELDS ---------------------------------##
//##############################################################################

	//==== References ====\\.
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

public:
	virtual void ConfigureDebugInput_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "DebugActionsSystem|Setup")
	void Setup(const FText& InDebugInputTitle, TSubclassOf<UObject> InClassFilter, bool bIncludeParentClass, bool bIncludeAbstractClass, bool bAlphabeticSort);
	UFUNCTION(BlueprintPure, Category = "DebugActionsSystem")
	TSubclassOf<class UObject> GetValue() const;
};