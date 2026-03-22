// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DASFunctionLibrary.generated.h"

UENUM(BlueprintType, meta=(Bitflags))
enum class EWidgetStyleMode : uint8 {
	None     = 0 UMETA(Hidden),
	Normal   = 1 << 0, // 0001 = 1
	Hovered  = 1 << 1, // 0010 = 2
	Pressed  = 1 << 2, // 0100 = 4
	Disabled = 1 << 3  // 1000 = 8
};

ENUM_CLASS_FLAGS(EWidgetStyleMode) // Macro to add bitwise operations

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/*

*/
UCLASS()
class DEBUGACTIONSSYSTEMCORE_API UDASFunctionLibrary : public UBlueprintFunctionLibrary {
	GENERATED_BODY()
	
//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################

public:
	/** Returns all derived classes of ParentClass loaded in the game */
	UFUNCTION(BlueprintCallable)
	static void GetAllDerivedClasses(TSubclassOf<UObject> ParentClass, TArray<UClass*>& OutSubClasses, bool bIncludeAbstract = false);

	UFUNCTION(BlueprintCallable)
	static void SetImageOfButtonStyles(
		class UButton* ButtonWidget,
		UPARAM(meta=(
			DisplayThumbnail="true",
			DisplayName="Image",
			AllowedClasses= "/Script/Engine.Texture,/Script/Engine.MaterialInterface,/Script/Engine.SlateTextureAtlasInterface",
			DisallowedClasses = "/Script/MediaAssets.MediaTexture"
		)) class UObject* Texture,
		UPARAM(meta=(Bitmask, BitmaskEnum="/Script/DebugActionsSystem.EWidgetStyleMode")) int32 WidgetStyleModesMask = 15
	);

	UFUNCTION(BlueprintCallable)
	static void SetDrawAsOfButtonStyles(
		class UButton* ButtonWidget,
		ESlateBrushDrawType::Type DrawAsType,
		UPARAM(meta=(Bitmask, BitmaskEnum="/Script/DebugActionsSystem.EWidgetStyleMode")) int32 WidgetStyleModesMask = 15
	);
	
	UFUNCTION(BlueprintPure, Category = "Debug", meta = (DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static FString GetBlueprintPMethodPrefix(const UObject* ContextObject, FString MethodName);
	
	static void K2_WizLog(
		const UObject* ContextObject, 
		FString MethodName,
		const FString& Message            = "",
		bool bAddLogMessage				  = false,
		FColor OnScreenMessageColor       = FColor::White,
		float OnScreenMessageDuration     = 7.f,
		uint64 OnScreenMessageKey         = INDEX_NONE
	);
	
	template <typename T>
	static void PrintTArray(const TCHAR* Label, const TArray<void*>& Array, const FColor& Color, int32 Key);

	template <typename T>
	static void PrintTSet(const TCHAR* Label, const TSet<T>& Set, const FColor& Color, int32 Key);

	template <typename K, typename V>
	static void PrintTMap(const TCHAR* Label, const TMap<K, V>& Map, const FColor& Color, int32 Key);

	template <typename K, typename V>
	static void PrintTMapOfArrays(const TCHAR* Label, const TMap<K, V>& Map, const FColor& Color, int32 Key);

	template <typename K, typename V>
	static void PrintTMapOfSets(const TCHAR* Label, const TMap<K, V>& Map, const FColor& Color, int32 Key);

	template <typename T>
	static constexpr FString GetTemplateTypeName();
};

#if CPP
#include "BlueprintFunctionsLibraries/DASFunctionLibrary.inl"
#endif