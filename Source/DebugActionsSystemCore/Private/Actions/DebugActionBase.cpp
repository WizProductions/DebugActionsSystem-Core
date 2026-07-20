// Copyright Wiz Corporation. All Rights Reserved.

#include "Actions/DebugActionBase.h"
#include "Components/SlateWrapperTypes.h"
#include "Enumerations/EDebugActionResult.h"
#include "WidgetBases/DebugActionWidgetBase.h"

#if WITH_EDITOR
void UDebugActionBase::PostEditChangeProperty( FPropertyChangedEvent& PropertyChangedEvent )
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	//Here - Called by the system when the array is updated or following a manual call during the array update
	
	//The folder doesn't need to be updated
	if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayAdd)
		return;
	
	//Update folder title in DataAsset by new value from user
	UpdateEditorDataAssetTitle();
}

void UDebugActionBase::RecursiveRequestDataAssetTitleUpdate()
{
	UpdateEditorDataAssetTitle();
}

void UDebugActionBase::UpdateEditorDataAssetTitle()
{
	const FText Title = GetDebugActionTitle();
	if (Title.IsEmpty() == false)
		Private_DataAssetActionTitle = Title.ToString();
}
#endif

void UDebugActionBase::PostInitProperties()
{
	Super::PostInitProperties();
	
	OnPostInitProperties();
}

EDebugActionResult UDebugActionBase::InitializeDebugAction( TArray<TObjectPtr<UDebugActionBase>>& OutDebugActionsHierarchy, UDebugSubsystem* Subsystem )
{
	bDebugActionState = false;
	MyDebugSubsystem  = Subsystem;

	return EDebugActionResult::Success;
}

void UDebugActionBase::SetDebugActionWidgetVisibility( ESlateVisibility NewVisibility, int DepthRecursivity )
{
	SetDebugActionWidgetVisibility(NewVisibility);
}

void UDebugActionBase::SetDebugActionWidgetVisibility( ESlateVisibility NewVisibility )
{
	MyDebugActionWidget->SetVisibility(NewVisibility);
}

EDebugActionResult UDebugActionBase::ExecuteDebugAction()
{
	bDebugActionState = !bDebugActionState;
	return OnExecuteDebugAction();
}

EDebugActionResult UDebugActionBase::OnExecuteDebugAction_Implementation()
{
	//For derived implementation.
	return EDebugActionResult::NotImplemented;
}

void UDebugActionBase::OnParentFolderIsCollapsed_Implementation( class UDebugActionFolder* ParentFolder )
{
	//For derived implementation.
}
void UDebugActionBase::OnParentFolderIsDeveloped_Implementation( class UDebugActionFolder* ParentFolder )
{
	//For derived implementation.
}

FText UDebugActionBase::GetDebugActionTitle_Implementation() const
{
	return FText::FromString("DefaultAction");
}
