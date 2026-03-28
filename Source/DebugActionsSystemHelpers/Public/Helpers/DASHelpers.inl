// Copyright Wiz Productions. All Rights Reserved.

#pragma once

template <typename C>
C* DASHelpers::AddComponentInConstruction(AActor* Owner, EComponentMobility::Type MobilityType) {
	
	C* Component = NewObject<C>(Owner);
	Component->SetMobility(MobilityType);
	Component->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	Component->RegisterComponent();
	
	return Component;
}