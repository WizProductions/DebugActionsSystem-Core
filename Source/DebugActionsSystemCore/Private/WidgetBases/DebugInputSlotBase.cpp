// Copyright Wiz Corporation. All Rights Reserved.

#include "WidgetBases/DebugInputSlotBase.h"
#include "DebugActionsSystemCoreDefines.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/NamedSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBoxSlot.h"
#include "SubSystems/DebugSubsystem.h"
#include "WidgetBases/DebugToolsWidgetBase.h"

#define LOCTEXT_NAMESPACE "UMG"

void UDebugInputSlotBase::NativeConstruct() {
	Super::NativeConstruct();

	bIsUsed = false;
	
	//==== INIT ====\\.
	if (!((MySlot = Cast<UCanvasPanelSlot>(Slot)))) {
		WIZ_RET_LOG( , "CanvasSlot is not valid. Are you sure Debug Input Slot is a child of Canvas?", Error, LogDebugActionsSystem);
	}

	InputWidgetSlot = Cast<UCanvasPanelSlot>(InputNamedSlot->Slot);

	//First outer -> Widget tree -> outer -> Debug Tools
	UObject* Owner = GetOuter()->GetOuter();
	UDebugToolsWidgetBase* DebugToolsWidget = UDebugSubsystem::Get(GetWorld())->GetDebugToolsWidget();

	//This widget is a child of existing Debug Tools
	if (Owner != DebugToolsWidget) {
		UE_LOG(LogDebugActionsSystem, Error, TEXT("DebugInputSlot::NativeConstruct: DebugInputSlot is not a child of UDebugToolsWidget"));
		return;
	}

	MySlot->SetAutoSize(true); //Size to content: true
	this->SetVisibility(ESlateVisibility::Collapsed);
	
	DebugToolsWidget->RegisterDebugInputSlot(this);
}

#if WITH_EDITOR
const FText UDebugInputSlotBase::GetPaletteCategory() {
	return LOCTEXT("Debug Actions System", "Debug Actions System");
}
#endif


UWidget* UDebugInputSlotBase::GetInputWidget() const { return InputNamedSlot->GetContent(); }

void UDebugInputSlotBase::SetInputWidget(UDebugInput* InDebugInput) {

	//Clear Content
	if (InDebugInput == nullptr) {
		InputNamedSlot->ClearChildren();
		return;
	}

	//eg: floatSlider wanted? => send to namedSlot
	InputNamedSlot->SetContent(InDebugInput->GetMyWidget());
	
	bIsUsed = true;
	this->SetVisibility(ESlateVisibility::Visible);
	InDebugInput->OnAddedToSlot(this);

}

void UDebugInputSlotBase::RemoveInputWidget() {
	SetInputWidget(nullptr);
	bIsUsed = false;
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UDebugInputSlotBase::SetTitle(const FText& InTitle) { InputTitle->SetText(InTitle); }
void UDebugInputSlotBase::SetInputSlotWidget(UNamedSlot* InNamedSlot) { InputNamedSlot = InNamedSlot; }
void UDebugInputSlotBase::SetInputTitleWidget(UTextBlock* InTextBlock) { InputTitle = InTextBlock; }
