// Copyright Wiz Corporation. All Rights Reserved.

#include "WidgetBases/DebugInputSlotBase.h"
#include "DebugActionsSystemCoreDefines.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/NamedSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBoxSlot.h"
#include "SubSystems/DebugSubsystem.h"
#include "WidgetBases/DebugPanelWidgetBase.h"

#define LOCTEXT_NAMESPACE "UMG"

void UDebugInputSlotBase::NativeConstruct() {
	Super::NativeConstruct();

	bIsUsed = false;
	
	//==== INIT ====\\.
	if (!((MySlot = Cast<UCanvasPanelSlot>(Slot)))) {
		WIZ_RET_LOG( , "CanvasSlot not valid. Are you sure Debug Input Slot is a child of Canvas?", Error, LogDebugActionsSystem);
	}

	InputWidgetSlot = Cast<UCanvasPanelSlot>(NS_InputSlot->Slot);

	//First outer -> Widget tree -> outer -> Debug Panel
	UObject* Owner = GetOuter()->GetOuter();
	UDebugPanelWidgetBase* DebugPanelWidget = UDebugSubsystem::Get(GetWorld())->GetDebugPanelWidget();

	//This widget is a child of existing Debug Tools
	if (Owner != DebugPanelWidget) {
		WIZ_RET_LOG( , "DebugInputSlot is not a child of UDebugPanelWidget", Error, LogDebugActionsSystem);
	}

	MySlot->SetAutoSize(true); //Size to content: true
	this->SetVisibility(ESlateVisibility::Collapsed);
	
	DebugPanelWidget->RegisterDebugInputSlot(this);
}

#if WITH_EDITOR
const FText UDebugInputSlotBase::GetPaletteCategory() {
	return LOCTEXT("Debug Actions System", "Debug Actions System");
}
#endif


UWidget* UDebugInputSlotBase::GetInputWidget() const { return NS_InputSlot ? NS_InputSlot->GetContent() : NULL; }

void UDebugInputSlotBase::SetInputWidget(UDebugInput* InDebugInput) {

	//Clear Content
	if (InDebugInput == nullptr) {
		NS_InputSlot->ClearChildren();
		return;
	}

	//eg: floatSlider wanted? => send to namedSlot
	NS_InputSlot->SetContent(InDebugInput->GetMyWidget());
	
	bIsUsed = true;
	this->SetVisibility(ESlateVisibility::Visible);
	InDebugInput->OnAddedToSlot(this);

}

void UDebugInputSlotBase::RemoveInputWidget() {
	SetInputWidget(nullptr);
	bIsUsed = false;
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UDebugInputSlotBase::SetTitle(const FText& InTitle) { TB_InputTitle->SetText(InTitle); }