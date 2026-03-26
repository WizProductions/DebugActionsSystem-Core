// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DebugPanelWidgetBase.generated.h"

class UDebugActionsSystemDataAsset;
class UDebugActionBase;
class UDebugInputSlotWidgetBase;

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
	TObjectPtr<class UCanvasPanel> CP_Root = NULL;
	
	//==== Settings ====\\.
	/** Used to store one folder per depth to store the current full path */
	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	TArray<TObjectPtr<UDebugActionBase>> DebugActionsDepthsArray;

	//==== Properties ====\\.
	int MaxDepthLevel = -1;
	UPROPERTY(BlueprintGetter = "GetDebugSubSystemChecked", Category = "References")
	TObjectPtr<class UDebugSubsystem> MyDebugSubsystem = NULL;
	UPROPERTY(BlueprintReadOnly, Category = "References")
	TArray<TObjectPtr<UDebugInputSlotWidgetBase>> DebugInputsSlotRegistered = {};

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################
	
#if UE_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

public:
	UFUNCTION(BlueprintPure)
	UDebugSubsystem* GetDebugSubSystemChecked();
private:
	void Internal_UpdateDebugActionsDepthLevelsArray(UDebugActionBase* InDebugActionFolder);
protected:
	void Internal_FindAndInitChildDebugActions(class UDebugSubsystem* Subsystem, int ParentDebugActionIndex, int DepthLevel, TArray<TObjectPtr<UDebugActionBase>>& ChildDebugActions, TObjectPtr<UDebugActionBase> ParentDebugAction);
	UFUNCTION(BlueprintNativeEvent, Category="DebugAction")
	bool AddDebugActionParentWidget(int DebugActionIndex, int DepthLevel, UDebugActionBase* DebugAction);
	UFUNCTION(BlueprintNativeEvent, Category="DebugAction")
	bool AddDebugActionChildWidget(int ChildDebugActionIndex, int DepthLevel, UDebugActionBase* ChildDebugAction, UDebugActionBase* ParentDebugAction);
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DebugAction")
	bool CreateDebugActionWidget(UDebugActionBase* LinkedDebugAction, class UDebugActionWidgetBase*& NewWidget, class UCanvasPanelSlot*& NewWidgetCanvasSlot);
	void GenerateDebugMenu(TArray<TObjectPtr<UDebugActionBase>>& DebugActions);
	void OnFolderStateChange(bool bIsDeveloped, bool bIsNewFolderClicked, UDebugActionBase* InDebugActionFolder);
	UDebugActionBase* GetDebugActionByDepth(int Depth) const;
private:
	template <typename WidgetT = UWidget> requires std::is_base_of_v<UWidget, WidgetT>
	WidgetT* Internal_NewWidget(TSubclassOf<UWidget> WidgetClass = WidgetT::StaticClass());
public:
	void RegisterDebugInputSlot(class UDebugInputSlotWidgetBase* InDebugInputSlot);
	/** Request a slot assignment to DI @return True if a slot is free and DI is assigned to his, otherwise False */
	bool AssignSlotToDebugInput(class UDebugInput* InDebugInput);
	/** Unassign slot to DI @return True is a slot is found and unassigned, otherwise False */
	bool UnassignSlotToDebugInput(class UDebugInput* InDebugInput);
	void ClearSlotsAssigment();
	/** Set a new visibility to all registered and used debug input slots */
	void SetActiveDebugInputSlotsVisibility(ESlateVisibility InVisibility);

	
	friend class UDebugSubsystem;
};

#if CPP
#include "DebugPanelWidgetBase.inl"
#endif
