// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DebugActionBase.generated.h"

enum class EDebugActionResult : UINT8;

//#############################################################################
//##---------------------------------- CLASS --------------------------------##
//#############################################################################

/*
 *
*/
UCLASS(Blueprintable, Abstract)
class DEBUGACTIONSSYSTEMCORE_API UDebugActionBase : public UObject {
	GENERATED_BODY()

//#############################################################################
//##--------------------------------- FIELDS --------------------------------##
//#############################################################################

protected:
	//==== Properties ====\\.
	UPROPERTY(BlueprintGetter="GetMyDebugActionWidget", BlueprintSetter="SetMyDebugActionWidget")
	TObjectPtr<class UDebugActionWidgetBase> MyDebugActionWidget;
	
	int DepthLevel = -1;
	bool bDebugActionState = false;
	
	UPROPERTY()
	class UDebugSubsystem* MyDebugSubsystem = NULL;

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################

public:
	/** @param OutDebugActionsHierarchy: Reserved for UDebugActionFolder subclass */
	virtual EDebugActionResult InitializeDebugAction(TArray<TObjectPtr<UDebugActionBase>>& OutDebugActionsHierarchy, class UDebugSubsystem* Subsystem);
	virtual void SetDebugActionWidgetVisibility(bool bNewIsCollapsed, int DepthRecursivity);
	virtual void SetDebugActionWidgetVisibility(bool bNewIsCollapsed);
	virtual void OnParentFolderIsDeveloped(class UDebugActionFolder* ParentFolder);
	virtual void OnParentFolderIsCollapsed(class UDebugActionFolder* ParentFolder);
	void SetDepthLevel(const int InDepthLevel) { DepthLevel = InDepthLevel; }
	int GetDepthLevel() const { return DepthLevel; }
	
	UFUNCTION(BlueprintCallable, Category = "Debug Action")
	virtual FText GetDebugActionTitle() const { return FText::FromString("DefaultAction"); }
	UFUNCTION(BlueprintCallable, Category = "Debug Action")
	UDebugActionWidgetBase* GetMyDebugActionWidget() const { return MyDebugActionWidget; }
	UFUNCTION(BlueprintCallable, Category = "Debug Action")
	void SetMyDebugActionWidget(UDebugActionWidgetBase* InDebugActionWidget) { MyDebugActionWidget = InDebugActionWidget; }
	UFUNCTION(BlueprintCallable, Category = "Debug Action")
	bool GetDebugActionState() const { return bDebugActionState; }
	UFUNCTION(BlueprintCallable, Category = "Debug Action")
	void SetDebugActionState(const bool bInDebugActionState) { bDebugActionState = bInDebugActionState; }
	UFUNCTION(BlueprintCallable, Category = "Debug Action")
	virtual EDebugActionResult ExecuteDebugAction();
};
