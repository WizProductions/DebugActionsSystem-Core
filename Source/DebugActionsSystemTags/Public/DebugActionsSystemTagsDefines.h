// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"

//Version with "DAS_SharedDIKey_" prefix (eg: DAS_SharedDIKey_{$TagName})
#define DAS_DECLARE_SHARED_DI_GAMEPLAY_TAG_EXTERN(TagName) \
	DEBUGACTIONSSYSTEMTAGS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(DAS_SharedDIKey_ ## TagName)
//Version with "DAS_SharedDIKey_" prefix (eg: DAS_SharedDIKey_{$TagName})
#define DAS_DEFINE_SHARED_DI_GAMEPLAY_TAG_COMMENT(TagName, Tag, Comment) \
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(DAS_SharedDIKey_ ## TagName, "DAS.SharedDIKey." #Tag, Comment);

//==== DAS - Tags ====\\.
DAS_DECLARE_SHARED_DI_GAMEPLAY_TAG_EXTERN(UnShared);
DAS_DECLARE_SHARED_DI_GAMEPLAY_TAG_EXTERN(Default);
DAS_DECLARE_SHARED_DI_GAMEPLAY_TAG_EXTERN(ActorComponentInstSelector_Default);
DAS_DECLARE_SHARED_DI_GAMEPLAY_TAG_EXTERN(ActorInstSelector_Default);
DAS_DECLARE_SHARED_DI_GAMEPLAY_TAG_EXTERN(ClassSelector_Default);
DAS_DECLARE_SHARED_DI_GAMEPLAY_TAG_EXTERN(EnumSelector_Default);
DAS_DECLARE_SHARED_DI_GAMEPLAY_TAG_EXTERN(FloatSlider_Default);
DAS_DECLARE_SHARED_DI_GAMEPLAY_TAG_EXTERN(IntSlider_Default);
DAS_DECLARE_SHARED_DI_GAMEPLAY_TAG_EXTERN(LocalPlayerSelector_Default);