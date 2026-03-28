// Copyright Wiz Corporation. All Rights Reserved.


#include "Inputs/DebugInput_ClassSelector.h"
#include "BlueprintFunctionsLibraries/DASFunctionLibrary.h"
#include "Components/ComboBoxString.h"
#include "SubSystems/DebugSubsystem.h"
#include "WidgetBases/DebugInputSlotWidgetBase.h"

void UDebugInput_ClassSelector::ConfigureDebugInput_Implementation() {
	Super::ConfigureDebugInput_Implementation();
	
	DebugInputTitle = FText::FromString("Default CF title");
	DebugInputSize = FVector2D(120, 28);

	//==== Combo Box ====\\.
	MyClassFilterComboBox = UDebugSubsystem::Get(GetWorld())->GetNewWidgetInDebugPanel<UComboBoxString>();
	MyInputDataWidget = MyClassFilterComboBox;
}

TSubclassOf<class UObject> UDebugInput_ClassSelector::GetValue() const {

	if (MyClassFilterComboBox == NULL)
		WIZ_RET_LOG(NULL, "My ComboBox is invalid", Error, LogDebugActionsSystem);
	
	if (AllDerivedClasses.IsValidIndex(MyClassFilterComboBox->GetSelectedIndex())) {
		return AllDerivedClasses[MyClassFilterComboBox->GetSelectedIndex()];
	}

	return NULL;
}

void UDebugInput_ClassSelector::Setup(const FText& InDebugInputTitle, TSubclassOf<UObject> InClassFilter, bool bIncludeParentClass, bool bIncludeAbstractClass, bool bAlphabeticSort) {
	
	if (ClassFilter == InClassFilter)
		return;
	
	if (MyClassFilterComboBox == NULL)
		WIZ_RET_LOG( , "My combo box is invalid", Error, LogDebugActionsSystem);
	
	ClassFilter = InClassFilter;
	
	//Fill comboBox with derived classes.
	UDASFunctionLibrary::GetAllDerivedClasses(ClassFilter, AllDerivedClasses, bIncludeParentClass, bIncludeAbstractClass);

	if (bAlphabeticSort) {
		AllDerivedClasses.Sort([](const UClass& A, const UClass& B)
			{
				return A.GetName().ToUpper() < B.GetName().ToUpper();
			});
	}
	
	
	if (MyClassFilterComboBox->GetOptionCount() != 0)
		MyClassFilterComboBox->ClearOptions();
	
	for (auto Class : AllDerivedClasses) {
		MyClassFilterComboBox->AddOption(Class->GetName());
	}
	
	ClassFilter = InClassFilter;
	DebugInputTitle = InDebugInputTitle;
	
	MyDebugInputSlotWidget->SetTitle(DebugInputTitle);
}