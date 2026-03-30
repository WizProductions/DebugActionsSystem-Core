// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "DebugSubsystem.h"
#include "WidgetBases/DebugPanelWidgetBase.h"

template <typename DI> requires std::is_base_of_v<UDebugInputBase, DI>
DI* UDebugSubsystem::RequestDebugInput(const FGameplayTag& SharedKeyTag) {
	
	UDebugInputBase* NewDI = NULL;
	if (RequestDebugInput(DI::StaticClass(), NewDI, SharedKeyTag) == false)
		return NULL;
	
	return Cast<DI>(NewDI);
}

template <typename WidgetT> requires std::is_base_of_v<UWidget, WidgetT>
WidgetT* UDebugSubsystem::GetNewWidgetInDebugPanel() {
	
	UWidget* NewWidget = NULL;
	GetNewWidgetInDebugPanel(WidgetT::StaticClass(), NewWidget);
	
	return Cast<WidgetT>(NewWidget);
}
