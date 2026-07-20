// Copyright Wiz Corporation. All Rights Reserved.


#include "Inputs/DebugInput_ObjectSelector.h"
#include "BlueprintFunctionsLibraries/DASFunctionLibrary.h"
#include "Components/ComboBoxString.h"
#include "SubSystems/DebugSubsystem.h"
#include "WidgetBases/DebugInputSlotWidgetBase.h"

UWidget* UDebugInput_ObjectSelector::OnConfigureDebugInput_Implementation()
{
	MySlotProperties = FDISlotProperties("Default AIS Title", FVector2D(120, 28));

	//==== Combo Box (Class Filter) ====\\.
	MyComboBox = UDebugSubsystem::Get(this)->GetNewWidgetInDebugPanel<UComboBoxString>();
	if (MyComboBox == NULL)
		WIZ_RET_LOG(NULL, "Widget is invalid", Error, LogDebugActionsSystem);

	return MyComboBox;
}

void UDebugInput_ObjectSelector::Setup( TSubclassOf<UObject> ObjectClass, const FText& InDebugInputTitle, TArray<FName>& SearchPathFilters )
{
	ObjectsCache.Empty();

	UWorld* World = GetWorld();
	if (World == NULL) {
		WIZ_RET_LOG(, "World is invalid.", Error, LogDebugActionsSystem);
	}

	if (MyComboBox == NULL)
		WIZ_RET_LOG(, "My ComboBox is invalid", Error, LogDebugActionsSystem);

	if (MyComboBox.Get()->GetOptionCount() != 0) {
		MyComboBox->ClearOptions();
	}

	CacheObjectClass = ObjectClass;

	TArray<FAssetData> Assets;
	UDASFunctionLibrary::GetAssetsOfClass(ObjectClass, Assets, SearchPathFilters);

	for (auto Asset : Assets) {

		if (Asset.IsValid() == false)
			continue;

		//Convert AssetData to SoftObjectPtr
		TSoftObjectPtr<> SoftAssetPtr(Asset.ToSoftObjectPath());

		ObjectsCache.Add(SoftAssetPtr);

		FString OptionText = FString::Printf(TEXT("%s - %s"), *SoftAssetPtr.GetAssetName(), *SoftAssetPtr.ToSoftObjectPath().GetLongPackageName());
		MyComboBox->AddOption(OptionText);
	}

	MyComboBox.Get()->SetSelectedIndex(0);
	
	MySlotProperties.DisplayName = InDebugInputTitle;
	MyDebugInputSlotWidget->SetSlotProperties(MySlotProperties);
}

// ReSharper disable once CppUE4BlueprintCallableFunctionMayBeConst
void UDebugInput_ObjectSelector::GetValue( TSubclassOf<UObject> ObjectClass, TSoftObjectPtr<UObject>& OutObject )
{
	OutObject = GetValue<TSoftObjectPtr<UObject>>();
}
