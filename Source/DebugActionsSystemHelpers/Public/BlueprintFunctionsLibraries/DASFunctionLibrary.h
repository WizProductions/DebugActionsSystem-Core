// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Styling/SlateBrush.h"
#include "Templates/SubclassOf.h"
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

struct FAssetData;

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/*

*/
UCLASS()
class DEBUGACTIONSSYSTEMHELPERS_API UDASFunctionLibrary : public UBlueprintFunctionLibrary {
	GENERATED_BODY()
	
//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################

public:
	/** Returns all derived classes of ParentClass loaded in the game. */
	UFUNCTION(BlueprintCallable, Category = "Utils")
	static void GetAllDerivedClasses(TSubclassOf<UObject> ParentClass, TArray<UClass*>& OutSubClasses, bool bIncludeParentClass = true, bool bIncludeAbstract = false);
	
	/** Returns all object of Class in the engine, SearchPathFilters can be null and method returns all object found. */
	UFUNCTION(BlueprintCallable, Category = "Search", meta = (AutoCreateRefTerm = "SearchPathFilters"))
	static void GetAssetsOfClass(const UClass* TargetClass, TArray<FAssetData>& OutAssets, const TArray<FName>& SearchPathFilters, bool bSearchSubclasses = true, bool bRecursivePaths = true);
	
	UFUNCTION(BlueprintPure, Category = "Debug", meta = (DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static FString GetBlueprintPMethodPrefix(const UObject* ContextObject, FString MethodName);
	
	//@TODO: Remake with attribute, CustomThunk? 1.2?
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (BlueprintInternalUseOnly = "true"))
	static void K2_WizLog(
		const UObject* ContextObject, 
		FString MethodName				  = "UnknownMethod",
		const FString& Message            = "DefaultMessage",
		bool bAddLogMessage				  = false,
		FColor OnScreenMessageColor       = FColor::White,
		float OnScreenMessageDuration     = 7.f,
		int64 OnScreenMessageKey          = -1
	);
	
	UFUNCTION(BlueprintCallable, Category = "Utils|UMG")
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

	UFUNCTION(BlueprintCallable, Category = "Utils|UMG")
	static void SetDrawAsOfButtonStyles(
		class UButton* ButtonWidget,
		ESlateBrushDrawType::Type DrawAsType,
		UPARAM(meta=(Bitmask, BitmaskEnum="/Script/DebugActionsSystem.EWidgetStyleMode")) int32 WidgetStyleModesMask = 15
	);
	
	UFUNCTION(BlueprintPure, Category = "Debug")
	static int64 GetObjectUniqueID(UObject* Object);
};

#if CPP
#include "DASFunctionLibrary.inl"
#endif