// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"
#include "Helpers/DASHelpers.h"
#include "DebugActionsSystemHelpersDefine.h"

//==== DAS - Log Macros ====\\.
#define DAS_LOG_YELLOW(Message, Verbosity, ...) WIZ_LOG(Message, Verbosity, LogDebugActionsSystem, true, FColor::Yellow, 10.f, ##__VA_ARGS__)
#define DAS_LOG_RED(Message, Verbosity, ...) WIZ_LOG(Message, Verbosity, LogDebugActionsSystem, true, FColor::Red, 10.f, ##__VA_ARGS__)
#define DAS_RET_LOG_RED(ReturnValue, Message, Verbosity, ...) WIZ_RET_LOG(ReturnValue, Message, Verbosity, LogDebugActionsSystem, true, FColor::Red, 10.f, ##__VA_ARGS__)