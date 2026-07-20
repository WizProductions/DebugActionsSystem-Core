// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "InteractiveToolBuilder.h"
#include "BaseTools/ClickDragTool.h"
#include "DebugActionsSystemCoreInteractiveTool.generated.h"


/**
 * Builder for UDebugActionsSystemInteractiveTool
 */
UCLASS()
class DEBUGACTIONSSYSTEMCORE_API UDebugActionsSystemInteractiveToolBuilder : public UInteractiveToolBuilder
{
	GENERATED_BODY()

public:
	bool CanBuildTool(const FToolBuilderState& SceneState) const override { return false; }
	UInteractiveTool* BuildTool(const FToolBuilderState& SceneState) const override;
};


/**
 * Property set for the UDebugActionsSystemInteractiveTool
 */
UCLASS(Transient)
class DEBUGACTIONSSYSTEMCORE_API UDebugActionsSystemInteractiveToolProperties : public UInteractiveToolPropertySet
{
	GENERATED_BODY()

public:
	UDebugActionsSystemInteractiveToolProperties();

	/** First point of measurement */
	UPROPERTY(EditAnywhere, Category = Options)
	FVector StartPoint;

	/** Second point of measurement */
	UPROPERTY(EditAnywhere, Category = Options)
	FVector EndPoint;
	
	/** Current distance measurement */
	UPROPERTY(EditAnywhere, Category = Options)
	double Distance;
};



/**
 * UDebugActionsSystemInteractiveTool is an example Tool that allows the user to measure the 
 * distance between two points. The first point is set by click-dragging the mouse, and
 * the second point is set by shift-click-dragging the mouse.
 */
UCLASS()
class DEBUGACTIONSSYSTEMCORE_API UDebugActionsSystemInteractiveTool : public UInteractiveTool, public IClickDragBehaviorTarget
{
	GENERATED_BODY()

public:
	virtual void SetWorld(UWorld* World);

	/** UInteractiveTool overrides */
	void Setup() override;
	void Render(IToolsContextRenderAPI* RenderAPI) override;
	void OnPropertyModified(UObject* PropertySet, FProperty* Property) override;

	/** IClickDragBehaviorTarget implementation */
	FInputRayHit CanBeginClickDragSequence(const FInputDeviceRay& PressPos) override;
	void OnClickPress(const FInputDeviceRay& PressPos) override;
	void OnClickDrag(const FInputDeviceRay& DragPos) override;
	// these are not used in this Tool
	void OnClickRelease(const FInputDeviceRay& ReleasePos) override {}
	void OnTerminateDragSequence() override {}

	/** IModifierToggleBehaviorTarget implementation (inherited via IClickDragBehaviorTarget) */
	void OnUpdateModifierState(int ModifierID, bool bIsOn) override;


protected:
	/** Properties of the tool are stored here */
	UPROPERTY()
	TObjectPtr<UDebugActionsSystemInteractiveToolProperties> Properties;


protected:
	UPROPERTY()
	TObjectPtr<UWorld> TargetWorld = NULL;		// target World we will raycast into

	static constexpr int MoveSecondPointModifierID = 1;		// identifier we associate with the shift key
	bool bSecondPointModifierDown = false;				// flag we use to keep track of modifier state
	bool bMoveSecondPoint = false;						// flag we use to keep track of which point we are moving during a press-drag

	FInputRayHit FindRayHit(const FRay& WorldRay, FVector& HitPos);		// raycasts into World
	void UpdatePosition(const FRay& WorldRay);					// updates first or second point based on raycast
	void UpdateDistance();										// updates distance
};
