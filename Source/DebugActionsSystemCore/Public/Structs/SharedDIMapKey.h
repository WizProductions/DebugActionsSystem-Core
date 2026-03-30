// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SharedDIMapKey.generated.h"

//#############################################################################
//##-------------------------------- STRUCTURE ------------------------------##
//#############################################################################

/**
* A simple structure used as key, mix GameplayTag and TSubclassOf<UDebugInput> to make unique key.
*/
USTRUCT()
struct FSharedDIMapKey {
	GENERATED_BODY()
	
//#############################################################################
//##---------------------------------- FIELDS -------------------------------##
//#############################################################################

	UPROPERTY()
	FGameplayTag KeyTag;
	
	UPROPERTY()
	TSubclassOf<class UDebugInputBase> InputClass;
	
//#############################################################################
//##--------------------------------- METHODS -------------------------------##
//#############################################################################

	bool operator==(const FSharedDIMapKey& Rhs) const;
};

FORCEINLINE uint32 GetTypeHash(const FSharedDIMapKey& Key) {
	return HashCombine(GetTypeHash(Key.KeyTag), GetTypeHash(Key.InputClass));
}