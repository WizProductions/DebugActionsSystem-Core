// Copyright Wiz Corporation. All Rights Reserved.

#include "BlueprintFunctionsLibraries/DASFunctionLibrary.h"
#include "Components/Button.h"

void UDASFunctionLibrary::GetAllDerivedClasses(TSubclassOf<UObject> ParentClass, TArray<UClass*>& OutSubClasses, bool bIncludeAbstract) {

	OutSubClasses.Empty();
	if (!ParentClass) return;

	for (TObjectIterator<UClass> It; It; ++It) {
		UClass* Cls = *It;
		if (!Cls) continue;
		if (!bIncludeAbstract && Cls->HasAnyClassFlags(CLASS_Abstract)) continue;
		if (Cls == ParentClass) continue;

		// Remove hot-reload artifacts
		FString Name = Cls->GetName();
		if (Name.StartsWith(TEXT("SKEL_")) || Name.StartsWith(TEXT("REINST_")) || Name.StartsWith(TEXT("HOTRELOAD_"))) {
			continue;
		}

		if (Cls->IsChildOf(ParentClass)) {
			OutSubClasses.AddUnique(Cls);
		}
	}
}

void UDASFunctionLibrary::SetImageOfButtonStyles(class UButton* ButtonWidget, class UObject* Texture, int32 WidgetStyleModesMask) {

	if (ButtonWidget == NULL) {
		ensureAlwaysMsgf(false, TEXT("The button is invalid."));
		return;
	}

	//Use const cast to avoid a copy (why doesn't Epic want to return a non-const reference of the fbStyle?)
	FButtonStyle& NewStyle = const_cast<FButtonStyle&>(ButtonWidget->GetStyle());
	
	if (WidgetStyleModesMask & static_cast<int32>(EWidgetStyleMode::Normal))
		NewStyle.Normal.SetResourceObject(Texture);
	
	if (WidgetStyleModesMask & static_cast<int32>(EWidgetStyleMode::Hovered))
		NewStyle.Hovered.SetResourceObject(Texture);
	
	if (WidgetStyleModesMask & static_cast<int32>(EWidgetStyleMode::Pressed))
		NewStyle.Pressed.SetResourceObject(Texture);
	
	if (WidgetStyleModesMask & static_cast<int32>(EWidgetStyleMode::Disabled))
		NewStyle.Disabled.SetResourceObject(Texture);

	ButtonWidget->SetStyle(NewStyle);
}

void UDASFunctionLibrary::SetDrawAsOfButtonStyles(UButton* ButtonWidget, ESlateBrushDrawType::Type DrawAsType, int32 WidgetStyleModesMask) {
	
	if (ButtonWidget == NULL) {
		ensureAlwaysMsgf(false, TEXT("The button is invalid."));
		return;
	}

	FButtonStyle& NewStyle = const_cast<FButtonStyle&>(ButtonWidget->GetStyle());

	if (WidgetStyleModesMask & static_cast<int32>(EWidgetStyleMode::Normal))
		NewStyle.Normal.DrawAs = DrawAsType;
	if (WidgetStyleModesMask & static_cast<int32>(EWidgetStyleMode::Hovered))
		NewStyle.Hovered.DrawAs = DrawAsType;
	if (WidgetStyleModesMask & static_cast<int32>(EWidgetStyleMode::Pressed))
		NewStyle.Pressed.DrawAs = DrawAsType;
	if (WidgetStyleModesMask & static_cast<int32>(EWidgetStyleMode::Disabled))
		NewStyle.Disabled.DrawAs = DrawAsType;

	ButtonWidget->SetStyle(NewStyle);
}

FString UDASFunctionLibrary::GetBlueprintPMethodPrefix(const UObject* ContextObject, FString MethodName) {
	
	FString BPName = TEXT("Unknown");
	if (ContextObject)
	{
		BPName = ContextObject->GetClass()->GetName();
		BPName.RemoveFromEnd(TEXT("_C"));
	}

	return FString::Printf(TEXT("%s:%s"), *BPName, *MethodName);
}

void UDASFunctionLibrary::K2_WizLog(
	const UObject* ContextObject,
	FString MethodName,
	const FString& Message,
	bool bAddLogMessage,
	FColor OnScreenMessageColor,
	float OnScreenMessageDuration,
	int64 OnScreenMessageKey
) {
	//@TODO: Make definition
}
