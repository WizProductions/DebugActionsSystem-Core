// Copyright Wiz Corporation. All Rights Reserved.

#include "WidgetBases/DebugInputSlotWidgetBase.h"
#include "DebugActionsSystemCoreDefines.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/NamedSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBoxSlot.h"
#include "Inputs/DebugInputBase.h"
#include "SubSystems/DebugSubsystem.h"
#include "WidgetBases/DebugPanelWidgetBase.h"

#define LOCTEXT_NAMESPACE "UMG"

void UDebugInputSlotWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	bIsUsed = false;

	//==== INIT ====\\.
	if (!((MySlot = Cast<UCanvasPanelSlot>(Slot)))) {
		WIZ_RET_LOG(, "CanvasSlot not valid. Are you sure Debug Input Slot is a child of Canvas?", Error, LogDebugActionsSystem);
	}

	NamedSlotWidgetSlot                     = Cast<UCanvasPanelSlot>(NS_InputSlot->Slot);
	UDebugPanelWidgetBase* DebugPanelWidget = UDebugSubsystem::Get(this)->GetDebugPanelWidget();

	//First outer -> Widget tree -> outer -> Debug Panel
	UObject* Outer = GetOuter();
	if (IsValid(Outer) == false) {
		WIZ_RET_LOG(, "Outer is invalid.", Error);
	}
	while (IsValid(Outer->GetOuter()) && Outer != DebugPanelWidget) {
		Outer = Outer->GetOuter();
	}

	//This widget is a child of existing Debug Tools
	if (Outer != DebugPanelWidget) {
		WIZ_RET_LOG(, "DebugInputSlot is not a child of UDebugPanelWidget", Error, LogDebugActionsSystem);
	}

	MySlot->SetAutoSize(true); //Size to content: true
	this->SetVisibility(ESlateVisibility::Collapsed);

	DebugPanelWidget->RegisterDebugInputSlot(this);
}

#if WITH_EDITOR
const FText UDebugInputSlotWidgetBase::GetPaletteCategory()
{
	return LOCTEXT("Debug Actions System", "Debug Actions System");
}

#endif

UCanvasPanelSlot* UDebugInputSlotWidgetBase::GetInputWidgetSlot_Implementation() const
{
	return NamedSlotWidgetSlot;
}

UWidget* UDebugInputSlotWidgetBase::GetInputWidget_Implementation() const
{
	return NS_InputSlot
		       ? NS_InputSlot->GetContent()
		       : NULL;
}

void UDebugInputSlotWidgetBase::SetInputWidget_Implementation( UDebugInputBase* InDebugInput )
{
	if (IsValid(InDebugInput) == false) {
		RemoveInputWidget();
		return;
	}

	MyDebugInput = InDebugInput;

	UWidget* DIInputDataWidget = MyDebugInput.Get()->GetInputDataWidget();
	if (IsValid(DIInputDataWidget) == false)
		WIZ_RET_LOG(, "Input data widget found on Debug Input is invalid.", Error);

	NS_InputSlot->SetContent(DIInputDataWidget);

	bIsUsed = true;
	this->SetVisibility(ESlateVisibility::Visible);

	MyDebugInput->AddedToSlot(this);
}

void UDebugInputSlotWidgetBase::RemoveInputWidget_Implementation()
{
	MyDebugInput->RemoveFromSlot(this);

	NS_InputSlot->ClearChildren();
	MyDebugInput = NULL;
	bIsUsed      = false;

	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UDebugInputSlotWidgetBase::SetSlotProperties_Implementation( const FDISlotProperties& InProperties )
{
	if (InProperties.DisplayName.IsEmpty() == false) {

		if (IsValid(TB_InputTitle) == false)
			WIZ_RET_LOG(, "Textbox is invalid.", Error);

		TB_InputTitle->SetText(InProperties.DisplayName);
	}

	if (InProperties.Size.IsZero() == false) {

		if (IsValid(NS_InputSlot) == false)
			WIZ_RET_LOG(, "Slot is invalid.", Error);

		if (NamedSlotWidgetSlot.Get()->GetSize() != InProperties.Size) {
			NamedSlotWidgetSlot.Get()->SetSize(InProperties.Size);
		}
	}
}
