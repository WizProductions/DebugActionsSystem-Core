// Copyright Wiz Corporation. All Rights Reserved.


#include "Inputs/DebugInput_StringETB.h"
#include "Components/EditableTextBox.h"
#include "SubSystems/DebugSubsystem.h"
#include "WidgetBases/DebugInputSlotWidgetBase.h"

UWidget* UDebugInput_StringETB::OnConfigureDebugInput_Implementation()
{
	MySlotProperties = FDISlotProperties("Value", FVector2D(80, 28));

	MyEditableTextBox = UDebugSubsystem::Get(this)->GetNewWidgetInDebugPanel<UEditableTextBox>();

	if (MyEditableTextBox == NULL)
		WIZ_RET_LOG(NULL, "My SpinBox is invalid", Error, LogDebugActionsSystem);

	MyEditableTextBox.Get()->SetForegroundColor(FLinearColor(0, 0, 0, 1));
	MyEditableTextBox.Get()->WidgetStyle.BackgroundColor = FLinearColor(0.75, 0.75, 0.75, 1);
	MyEditableTextBox.Get()->WidgetStyle.TextStyle.Font.Size = 12.f;

	return MyEditableTextBox;
}

void UDebugInput_StringETB::Setup( const FText& InDebugInputTitle, const FText& HintText, bool IsPassword )
{
	if (MyEditableTextBox == NULL)
		WIZ_RET_LOG(, "My SpinBox is invalid", Error, LogDebugActionsSystem);

	if (InDebugInputTitle.IsEmpty())
		WIZ_LOG("Title is empty", Warning, LogDebugActionsSystem);

	MyEditableTextBox.Get()->SetHintText(HintText);
	MyEditableTextBox.Get()->SetIsPassword(IsPassword);

	MySlotProperties.DisplayName = InDebugInputTitle;
	MyDebugInputSlotWidget->SetSlotProperties(MySlotProperties);
}

FString UDebugInput_StringETB::GetValue() const
{
	if (MyEditableTextBox == NULL)
		WIZ_RET_LOG("", "My SpinBox is invalid", Error, LogDebugActionsSystem);

	return MyEditableTextBox.Get()->GetText().ToString();
}
