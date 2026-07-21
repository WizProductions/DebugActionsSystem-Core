// Copyright Wiz Corporation. All Rights Reserved.

#include "Inputs/DebugInputBase.h"
#include "DebugActionsSystemCoreDefines.h"
#include "SubSystems/DebugSubsystem.h"
#include "WidgetBases/DebugInputSlotWidgetBase.h"

void UDebugInputBase::ConfigureDebugInput()
{
	//Already configured
	if (IsValid(MyInputDataWidget))
		WIZ_RET_LOG(, "The input data widget is already configured, under normal circumstances, you don't need to call this method again.", Warning, LogDebugActionsSystem);

	MyInputDataWidget = OnConfigureDebugInput();

	if (IsValid(MyInputDataWidget) == false)
		WIZ_RET_LOG(, "The input data widget is invalid. Did you configure it in the OnConfigureDebugInput method?", Fatal, LogDebugActionsSystem);
}

void UDebugInputBase::AddedToSlot( class UDebugInputSlotWidgetBase* InInputSlot )
{
	MyDebugInputSlotWidget = InInputSlot;

	if (IsValid(InInputSlot) == false)
		WIZ_RET_LOG(, "New textblock is invalid.", Error, LogDebugActionsSystem);

	InInputSlot->SetSlotProperties(MySlotProperties);

	OnAddedToSlot(InInputSlot);
}

void UDebugInputBase::RemoveFromSlot( class UDebugInputSlotWidgetBase* InInputSlot )
{
	MyDebugInputSlotWidget = NULL;
	
	OnRemovedFromSlot(InInputSlot);
}

UDebugSubsystem* UDebugInputBase::GetDebugSubsystem() const
{
	return UDebugSubsystem::Get(this);
}

void UDebugInputBase::OnAddedToSlot_Implementation( UDebugInputSlotWidgetBase* InInputSlot )
{
	//For derived implementation.
}

void UDebugInputBase::OnRemovedFromSlot_Implementation( class UDebugInputSlotWidgetBase* InInputSlot )
{
	//For derived implementation.
}

UWidget* UDebugInputBase::OnConfigureDebugInput_Implementation()
{
	//For derived implementation.
	return NULL;
}