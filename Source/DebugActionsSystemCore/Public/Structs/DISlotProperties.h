// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DISlotProperties.generated.h"

//#############################################################################
//##-------------------------------- STRUCTURE ------------------------------##
//#############################################################################

/**
 * Debug input properties container
 */
USTRUCT(BlueprintType)
struct FDISlotProperties {
	GENERATED_BODY()
	
	FDISlotProperties() {
		DisplayName = FText::FromString("Default");
		Size = FVector2D(120, 65);
	}
	
	FDISlotProperties(const FText& InDisplayName, const FVector2D& InSize) {
		DisplayName = InDisplayName;
		Size = InSize;
	}
	
	FDISlotProperties(const FString& InDisplayName, const FVector2D& InSize) {
		DisplayName = FText::FromString(InDisplayName);
		Size = InSize;
	}
	
//#############################################################################
//##---------------------------------- FIELDS -------------------------------##
//#############################################################################

//==== Properties ====\\.
public:
	UPROPERTY(BlueprintReadWrite, Category = "Settings")
	FText DisplayName;
	UPROPERTY(BlueprintReadWrite, Category = "Settings")
	FVector2D Size;
};
