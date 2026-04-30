// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "BlueprintFunctionsLibraries/DASFunctionLibrary.h"
#include "Engine/Engine.h"

#define DH_DEFAULT_DURATION 1.f
#define DH_SEPARATOR_BOTTOM "-----------------------------------------------"

template <typename T>
void UDASFunctionLibrary::PrintTArray(const TCHAR* Label, const TArray<void*>& Array, const FColor& Color, int32 Key) {
	if (!GEngine)
		return;

	FString OutLog;

	OutLog += FString::Printf(TEXT("-----[ Array<%s>: %s (%d) ] -------\n"), *GetTemplateTypeName<T>(), Label, Array.Num());

	for (int32 i = 0; i < Array.Num(); ++i) {
		const void* Address = Array[i];
		OutLog += FString::Printf(TEXT("(%d): %p\n"), i, Address);
	}

	OutLog += DH_SEPARATOR_BOTTOM;
	GEngine->AddOnScreenDebugMessage(Key, DH_DEFAULT_DURATION, Color, OutLog);
}

template <typename T>
void UDASFunctionLibrary::PrintTSet(const TCHAR* Label, const TSet<T>& Set, const FColor& Color, int32 Key) {
	if (!GEngine)
		return;

	FString OutLog;

	OutLog += FString::Printf(TEXT("-----[ TSet<%s>: %s (%d) ] -------\n"), *GetTemplateTypeName<T>(), Label, Set.Num());

	int32 i = 0;
	for (auto It = Set.CreateConstIterator(); It; ++It) {
		const void* Address = static_cast<const void*>(&(*It));
		OutLog += FString::Printf(TEXT("Elem%d : %p\n"), i++, Address);
	}

	OutLog += DH_SEPARATOR_BOTTOM;
	GEngine->AddOnScreenDebugMessage(Key, DH_DEFAULT_DURATION, Color, OutLog);
}

template <typename K, typename V>
void UDASFunctionLibrary::PrintTMap(const TCHAR* Label, const TMap<K, V>& Map, const FColor& Color, int32 Key) {
	if (!GEngine)
		return;

	FString OutLog;
	OutLog += FString::Printf(TEXT("-----[ TMap<%s, %s>: %s (%d) ] -------\n"), *GetTemplateTypeName<K>(), *GetTemplateTypeName<V>(), Label, Map.Num());

	for (const TPair<K, V>& Pair : Map) {
		const void* Address = static_cast<const void*>(Pair.Value);
		OutLog += FString::Printf(TEXT("Key: %s - Value: %p\n"), *LexToString(Pair.Key), Address);
	}

	OutLog += DH_SEPARATOR_BOTTOM;
	GEngine->AddOnScreenDebugMessage(Key, DH_DEFAULT_DURATION, Color, OutLog);
}

template <typename K, typename V>
void UDASFunctionLibrary::PrintTMapOfArrays(const TCHAR* Label, const TMap<K, V>& Map, const FColor& Color, int32 Key) {
	if (!GEngine)
		return;

	FString OutLog;
	OutLog += FString::Printf(TEXT("-----[ MapOfArrays<%s, %s>: %s (%d) ] -------\n"), *GetTemplateTypeName<K>(), *GetTemplateTypeName<V>(), Label, Map.Num());

	int32 arrayIndex = 0;

	for (const TPair<K, V>& Pair : Map) {
		FString KeyName = TEXT("UnknownKey");
		if constexpr (std::is_pointer_v<K>) {
			KeyName = (Pair.Key != nullptr) ? Pair.Key->GetName() : TEXT("NullPtr");
		}

		OutLog += FString::Printf(TEXT("Key[%d]: %s | ArrayCount: %d\n"), arrayIndex, *KeyName, Pair.Value.Num());

		for (int32 j = 0; j < Pair.Value.Num(); j++) {
			const void* Addr = static_cast<const void*>(&Pair.Value[j]);
			OutLog += FString::Printf(TEXT("   [%d | Elem%d] : %p\n"), arrayIndex, j, Addr);
		}

		arrayIndex++;
	}

	OutLog += DH_SEPARATOR_BOTTOM;
	GEngine->AddOnScreenDebugMessage(Key, DH_DEFAULT_DURATION, Color, OutLog);
}

template <typename K, typename V>
void UDASFunctionLibrary::PrintTMapOfSets(const TCHAR* Label, const TMap<K, V>& Map, const FColor& Color, int32 Key) {
	if (!GEngine)
		return;

	FString OutLog;
	OutLog += FString::Printf(TEXT("-----[ Map<%s, %s>: %s (%d) ] -------\n"), *GetTemplateTypeName<K>(), *GetTemplateTypeName<V>(), Label, Map.Num());

	int32 setIndex = 0;

	for (const TPair<K, V>& Pair : Map) {
		FString KeyName = TEXT("UnknownKey");

		if constexpr (std::is_pointer_v<K>) {
			KeyName = (Pair.Key != nullptr) ? Pair.Key->GetName() : TEXT("NullPtr");
		}
		if constexpr (TIsDerivedFrom<K, UObject>::IsDerived) {
			KeyName = Pair.Key->GetName();
		}
		if constexpr (TIsTSubclassOf<K>::Value)
		{
			KeyName = Pair.Key->GetName();
		}
		
		OutLog += FString::Printf(TEXT("Key: %s - (%d elems)\n"), *KeyName, Pair.Value.Num());

		int32 elemIndex = 0;
		for (auto Elem : Pair.Value) {
			OutLog += FString::Printf(TEXT("    - [%d]=%p\n"), elemIndex, Elem);

			elemIndex++;
		}

		setIndex++;
	}

	OutLog += DH_SEPARATOR_BOTTOM;
	GEngine->AddOnScreenDebugMessage(Key, DH_DEFAULT_DURATION, Color, OutLog);
}

template<typename T>
constexpr FString UDASFunctionLibrary::GetTemplateTypeName() {
#if defined(__clang__) || defined(__GNUC__)
	FString FuncSig = TEXT(__PRETTY_FUNCTION__);
	int32 Start = FuncSig.Find(TEXT("T = ")) + 4;
	int32 End = FuncSig.Find(TEXT("]"), ESearchCase::IgnoreCase, ESearchDir::FromStart, Start);
#elif defined(_MSC_VER)
	FString FuncSig = TEXT(__FUNCSIG__);
	int32 Start = FuncSig.Find(TEXT("GetTemplateTypeName<"));
	if (Start != INDEX_NONE) Start += FCString::Strlen(TEXT("GetTemplateTypeName<"));
	int32 End = FuncSig.Find(TEXT(">(void)"));
#endif

	if (Start != INDEX_NONE && End != INDEX_NONE && End > Start)
	{
		FString TypeName = FuncSig.Mid(Start, End - Start);
		
		TypeName.ReplaceInline(TEXT("class "), TEXT(""));
		TypeName.ReplaceInline(TEXT("struct "), TEXT(""));
		TypeName.ReplaceInline(TEXT("enum "), TEXT(""));
		TypeName.ReplaceInline(TEXT("FDefaultSetAllocator"), TEXT(""));
		TypeName.ReplaceInline(TEXT("DefaultKeyFuncs"), TEXT(""));
		TypeName.ReplaceInline(TEXT(">"), TEXT(""));
		TypeName.ReplaceInline(TEXT("0"), TEXT(""));
		TypeName = TypeName.TrimStartAndEnd();
		
		return TypeName;
	}

	return TEXT("UnknownType");
}