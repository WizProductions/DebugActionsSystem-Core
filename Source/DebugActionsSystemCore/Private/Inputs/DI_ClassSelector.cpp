// Copyright Wiz Corporation. All Rights Reserved.


#include "Inputs/DI_ClassSelector.h"
#include "BlueprintFunctionsLibraries/DASFunctionLibrary.h"
#include "Components/ComboBoxString.h"
#include "SubSystems/DebugSubsystem.h"
#include "WidgetBases/DebugInputSlotBase.h"

void UDI_ClassSelector::PostInitProperties() {
	Super::PostInitProperties();

	//In game only
	if (!GetWorld())
		return;

	DebugInputTitle = FText::FromString("Default CF title");
	DebugInputSize = FVector2D(120, 28);

	//==== Combo Box ====\\.
	MyClassFilterComboBox = UDebugSubsystem::Get(GetWorld())->GetNewWidgetInDebugPanel<UComboBoxString>();
	MyWidget = MyClassFilterComboBox;
}

TSubclassOf<class UObject> UDI_ClassSelector::GetValue() const {

	if (MyClassFilterComboBox) {
		if (AllDerivedClasses.IsValidIndex(MyClassFilterComboBox->GetSelectedIndex())) {
			return AllDerivedClasses[MyClassFilterComboBox->GetSelectedIndex()];
		}
	}

	return nullptr;
}

void UDI_ClassSelector::Setup(FText InDebugInputTitle, TSubclassOf<UObject> InClassFilter) {
	
	if (ClassFilter == InClassFilter)
		return;
	
	ClassFilter = InClassFilter;
	
	//Fill comboBox with derived classes.
	UDASFunctionLibrary::GetAllDerivedClasses(ClassFilter, AllDerivedClasses);

	if (MyClassFilterComboBox->GetOptionCount() != 0)
		MyClassFilterComboBox->ClearOptions();
	
	for (auto Class : AllDerivedClasses) {
		MyClassFilterComboBox->AddOption(Class->GetName());
	}
	
	ClassFilter = InClassFilter;
	DebugInputTitle = InDebugInputTitle;
	
	MyDebugInputSlot->SetTitle(DebugInputTitle);
}