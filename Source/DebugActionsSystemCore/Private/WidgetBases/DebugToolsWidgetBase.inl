// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "DebugActionsSystemCoreDefines.h"
#include "DebugToolsWidgetBase.h"
#include "Blueprint/WidgetTree.h"

template <typename T> requires std::is_base_of_v<UWidget, T>
T* UDebugToolsWidgetBase::Internal_NewWidget() {

	if (WidgetTree) {
		T* NewWidget = WidgetTree->ConstructWidget<T>();
		return NewWidget;
	}

	WIZ_RET_LOG(NULL, "WidgetTree is not properly created!", Error, LogDebugActionsSystem);
}
