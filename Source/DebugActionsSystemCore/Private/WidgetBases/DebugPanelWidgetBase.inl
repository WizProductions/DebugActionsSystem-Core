// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "DebugActionsSystemCoreDefines.h"
#include "DebugPanelWidgetBase.h"
#include "Blueprint/WidgetTree.h"

template <typename WidgetT> requires std::is_base_of_v<UWidget, WidgetT>
WidgetT* UDebugPanelWidgetBase::Internal_NewWidget(TSubclassOf<UWidget> WidgetClass) {

	if (WidgetTree == NULL)
		WIZ_RET_LOG(NULL, "WidgetTree is not properly created!", Error, LogDebugActionsSystem);
	
	return WidgetTree->ConstructWidget<WidgetT>(WidgetClass);
}
