// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DebugActionBase.generated.h"

enum class EDebugActionResult : uint8;

//#############################################################################
//##---------------------------------- CLASS --------------------------------##
//#############################################################################

/**
 * Base of all debug actions, cannot be instanced. \n
 * You can create a custom DebugActions by inheriting from this base and overload ExecuteDebugAction() method
 * You can hide variables with "Hidden" category.
 */
UCLASS(Blueprintable, EditInlineNew, Abstract, HideDropdown, HideCategories=("Hidden"))
class DEBUGACTIONSSYSTEMCORE_API UDebugActionBase : public UObject {
	GENERATED_BODY()

//#############################################################################
//##--------------------------------- FIELDS --------------------------------##
//#############################################################################

protected:
	//Used for customize title in array (only on editor)
	UPROPERTY(VisibleAnywhere, Transient, meta = (HideInDetailPanel, EditCondition = "false", EditConditionHides), Category = "Hidden")
	FString Private_DataAssetActionTitle;
	
protected:
	//==== Properties ====\\.
	UPROPERTY(BlueprintGetter="GetMyDebugActionWidget", BlueprintSetter="SetMyDebugActionWidget", Category = "References")
	TObjectPtr<class UDebugActionWidgetBase> MyDebugActionWidget;
	UPROPERTY(BlueprintReadOnly, Category = "References")
	TObjectPtr<class UDebugSubsystem> MyDebugSubsystem = NULL;
	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	int32 DepthLevel = -1;
	
	//==== Flags ====\\.
	UPROPERTY(BlueprintReadOnly, Category = "Properties|Flags")
	bool bDebugActionState = false;

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################

public:
#if WITH_EDITOR
	virtual void PostInitProperties() override;
	virtual void UpdateEditorDataAssetTitle();
	UFUNCTION(BlueprintCallable, Category = "DataAsset|Refresh")
	void RefreshDebugDataAssetView();
#endif
	
	/** @param OutDebugActionsHierarchy: Reserved for UDebugActionFolder subclass */
	virtual EDebugActionResult InitializeDebugAction(TArray<TObjectPtr<UDebugActionBase>>& OutDebugActionsHierarchy, class UDebugSubsystem* Subsystem);
	virtual void SetDebugActionWidgetVisibility(bool bNewIsCollapsed, int DepthRecursivity);
	virtual void SetDebugActionWidgetVisibility(bool bNewIsCollapsed);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "DebugActionsSystem")
	void OnPostInitProperties();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DebugActionsSystem")
	void OnParentFolderIsDeveloped(class UDebugActionFolder* ParentFolder);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DebugActionsSystem")
	void OnParentFolderIsCollapsed(class UDebugActionFolder* ParentFolder);
	
	FORCEINLINE void SetDepthLevel(const int InDepthLevel) { DepthLevel = InDepthLevel; }
	
	UFUNCTION(BlueprintCallable, Category = "Properties")
	FORCEINLINE int32 GetDepthLevel() const { return DepthLevel; }
	UFUNCTION(BlueprintCallable, Category = "Properties")
	FORCEINLINE UDebugActionWidgetBase* GetMyDebugActionWidget() const { return MyDebugActionWidget; }
	UFUNCTION(BlueprintCallable, Category = "Properties")
	FORCEINLINE void SetMyDebugActionWidget(UDebugActionWidgetBase* InDebugActionWidget) { MyDebugActionWidget = InDebugActionWidget; }
	UFUNCTION(BlueprintCallable, Category = "Properties")
	FORCEINLINE bool GetDebugActionState() const { return bDebugActionState; }
	UFUNCTION(BlueprintCallable, Category = "Properties")
	FORCEINLINE void SetDebugActionState(const bool bInDebugActionState) { bDebugActionState = bInDebugActionState; }
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DebugActionsSystem|Settings")
	FText GetDebugActionTitle() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DebugActionsSystem")
	EDebugActionResult ExecuteDebugAction();
};
