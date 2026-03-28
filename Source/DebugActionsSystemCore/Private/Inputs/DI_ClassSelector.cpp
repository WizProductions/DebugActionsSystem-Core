// Copyright Wiz Corporation. All Rights Reserved.


#include "Inputs/DI_ClassSelector.h"
#include "BlueprintFunctionsLibraries/DASFunctionLibrary.h"
#include "Components/ComboBoxString.h"
#include "SubSystems/DebugSubsystem.h"
#include "WidgetBases/DebugInputSlotWidgetBase.h"

void UDI_ClassSelector::ConfigureDebugInput_Implementation() {
	Super::ConfigureDebugInput_Implementation();
	
	DebugInputTitle = FText::FromString("Default CF title");
	DebugInputSize = FVector2D(120, 28);

	//==== Combo Box ====\\.
	MyClassFilterComboBox = UDebugSubsystem::Get(GetWorld())->GetNewWidgetInDebugPanel<UComboBoxString>();
	MyInputDataWidget = MyClassFilterComboBox;
}

TSubclassOf<class UObject> UDI_ClassSelector::GetValue() const {

	if (MyClassFilterComboBox == NULL)
		WIZ_RET_LOG(NULL, "My ComboBox is invalid", Error, LogDebugActionsSystem);
	
	if (AllDerivedClasses.IsValidIndex(MyClassFilterComboBox->GetSelectedIndex())) {
		return AllDerivedClasses[MyClassFilterComboBox->GetSelectedIndex()];
	}

	return NULL;
}

void UDI_ClassSelector::Setup(const FText& InDebugInputTitle, TSubclassOf<UObject> InClassFilter) {
	
	if (ClassFilter == InClassFilter)
		return;
	
	if (MyClassFilterComboBox == NULL)
		WIZ_RET_LOG( , "My combo box is invalid", Error, LogDebugActionsSystem);
	
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
	
	MyDebugInputSlotWidget->SetTitle(DebugInputTitle);
}