// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DebugInputBase.h"
#include "Components/ComboBoxString.h"
#include <type_traits>
#include "DebugActionsSystemCoreDefines.h"
#include "DebugInput_ObjectSelector.generated.h"

//Keep default type
template <typename T>
struct TUnwrapSoftPtr {
	using Type = T;
};

//Extract the type in soft object ptr
template <typename T>
struct TUnwrapSoftPtr<TSoftObjectPtr<T>> {
	using Type = T;
};

template <typename O>
concept ObjectType = std::is_base_of_v<UObject, typename TUnwrapSoftPtr<O>::Type>;

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/**
* An input that retrieves a specific object in the editor and allows you to select one. \n
* NEEDS A SETUP.
*/
UCLASS()
class DEBUGACTIONSSYSTEMCORE_API UDebugInput_ObjectSelector : public UDebugInputBase {
	GENERATED_BODY()

//##############################################################################
//##--------------------------------- FIELDS ---------------------------------##
//##############################################################################

	//==== References ====\\.
public:
	UPROPERTY(BlueprintReadOnly, Category = "References")
	TObjectPtr<UComboBoxString> MyComboBox = NULL;

	//==== Properties ====\\.
protected:
	UPROPERTY()
	TSubclassOf<UObject> ClassFilter = UObject::StaticClass();
	UPROPERTY()
	TArray<TSoftObjectPtr<UObject>> ObjectsCache;
	UPROPERTY()
	TObjectPtr<UClass> CacheObjectClass = NULL;

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################

public:
	UWidget* OnConfigureDebugInput_Implementation() override;

	template <ObjectType O>
	void Setup( const FText& InDebugInputTitle, TArray<FName>& SearchPathFilters );
	template <ObjectType O>
	O GetValue() const;

	/** Setup this debug input with lot a settings, RequiredRootFolder example: "/Game/Levels", only objects in this folder will be found. */
	UFUNCTION(BlueprintCallable, Category = "DebugActionsSystem|Setup", meta = (AutoCreateRefTerm = "SearchPathFilters"))
	void Setup( TSubclassOf<UObject> ObjectClass, const FText& InDebugInputTitle, TArray<FName>& SearchPathFilters );
	UFUNCTION(BlueprintPure, Category = "DebugActionsSystem", meta = (DeterminesOutputType = "ObjectClass", DynamicOutputParam = "OutObject"))
	void GetValue( TSubclassOf<UObject> ObjectClass, TSoftObjectPtr<UObject>& OutObject );
};

//#############################################################################
//##-------------------------- INLINE TEMPLATES -----------------------------##
//#############################################################################

template <ObjectType O>
void UDebugInput_ObjectSelector::Setup( const FText& InDebugInputTitle, TArray<FName>& SearchPathFilters )
{
	this->Setup(O::StaticClass(), InDebugInputTitle, SearchPathFilters);
}

template <ObjectType O>
O UDebugInput_ObjectSelector::GetValue() const
{
	if (MyComboBox == NULL)
		WIZ_RET_LOG(NULL, "My ComboBox is invalid", Error, LogDebugActionsSystem);

	if (CacheObjectClass == NULL)
		WIZ_RET_LOG(NULL, "Component selector not set up, do you call SetupSelector() after request debug input?", Error, LogDebugActionsSystem);

	auto Index = MyComboBox.Get()->GetSelectedIndex();
	if (ObjectsCache.IsValidIndex(Index))
	{
		return TSoftObjectPtr<typename TUnwrapSoftPtr<O>::Type>(ObjectsCache[Index].ToSoftObjectPath());
	}

	WIZ_RET_LOG(NULL, "Index of component invalid, probably delete between Setup() and ExecuteDebugAction()", Warning, LogDebugActionsSystem);
}