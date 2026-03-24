// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DebugPanelWidgetBase.generated.h"

class UDebugActionsSystemDataAsset;
class UDebugActionBase;
class UDebugInputSlotBase;

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/**
 * The widget panel of Debug Actions System, you can inherit from this base to customize its appearance. \n
 */
UCLASS(Abstract)
class DEBUGACTIONSSYSTEMCORE_API UDebugPanelWidgetBase : public UUserWidget {
	GENERATED_BODY()
	
//#############################################################################
//##--------------------------------- FIELDS --------------------------------##
//#############################################################################

protected:
	//==== Widgets Binding ====\\.
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "UserInterface|Interaction")
	TObjectPtr<class UCanvasPanel> CP_Root = nullptr;
	
	//==== Settings ====\\.
	UPROPERTY()
	TArray<TObjectPtr<UDebugActionBase>> DebugActionsDepthsArray;

	//==== Properties ====\\.
	int MaxDepthLevel = -1;
	UPROPERTY()
	TArray<TObjectPtr<UDebugInputSlotBase>> DebugInputsSlotRegistered = {};

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################
	
#if UE_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

private:
	void Internal_UpdateDebugActionsDepthLevelsArray(UDebugActionBase* InDebugActionFolder);
protected:
	void Internal_FindAndInitChildDebugActions(class UDebugSubsystem* Subsystem, int ParentDebugActionIndex, int DepthLevel, TArray<TObjectPtr<UDebugActionBase>>& ChildDebugActions, TObjectPtr<UDebugActionBase> ParentDebugAction);
	UFUNCTION(Category="Debug Action", BlueprintNativeEvent)
	bool AddDebugActionParentWidget(int DebugActionIndex, int DepthLevel, UDebugActionBase* DebugAction);
	UFUNCTION(Category="Debug Action", BlueprintNativeEvent)
	bool AddDebugActionChildWidget(int ChildDebugActionIndex, int DepthLevel, UDebugActionBase* ChildDebugAction, UDebugActionBase* ParentDebugAction);
public:
	void GenerateDebugMenu(TArray<TObjectPtr<UDebugActionBase>>& DebugActions);
	void OnFolderStateChange(bool bIsDeveloped, bool bIsNewFolderClicked, UDebugActionBase* InDebugActionFolder);
	UDebugActionBase* GetDebugActionByDepth(int Depth) const;
private:
	template <typename T> requires std::is_base_of_v<UWidget, T>
	T* Internal_NewWidget();
public:
	void RegisterDebugInputSlot(class UDebugInputSlotBase* InDebugInputSlot);
	/** Request a slot assignment to DI @return True if a slot is free and DI is assigned to his, otherwise False */
	bool AssignSlotToDebugInput(class UDebugInput* InDebugInput);
	/** Unassign slot to DI @return True is a slot is found and unassigned, otherwise False */
	bool UnassignSlotToDebugInput(class UDebugInput* InDebugInput);
	void ClearSlotsAssigment();
	/** Set a new visibility to all registered and used debug input slots */
	void SetDebugInputSlotsRegisteredVisibility(ESlateVisibility InVisibility);

	
	friend class UDebugSubsystem;
};

#if CPP
#include "DebugPanelWidgetBase.inl"
#endif
