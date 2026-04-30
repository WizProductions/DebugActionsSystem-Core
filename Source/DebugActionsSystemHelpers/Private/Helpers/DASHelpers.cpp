// Copyright Wiz Productions. All Rights Reserved.

#include "Helpers/DASHelpers.h"
#include "Components/GridPanel.h"
#include "Components/RichTextBlock.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"
#include "GameFramework/GameStateBase.h"
#include "Components/PrimitiveComponent.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/SOverlay.h"
#include "Kismet/GameplayStatics.h"

bool DASHelpers::IsInLevelEditor() {
#if UE_EDITOR
	
	if (!GIsEditor || IsRunningCommandlet() || !GEditor) {
		return false;
	}

	UWorld* World = GEditor->GetEditorWorldContext().World();
	return World && World->WorldType == EWorldType::Editor;
#else
	return false;
#endif
}

bool DASHelpers::IsCurrentPlayer(const UWorld* WorldContext, int32 playerIndex, const AActor* ActorToTest) {

	AActor* Actor = UGameplayStatics::GetPlayerPawn(WorldContext, playerIndex);
	//if Actor or ActorToTest is nullptr, both are different -> not player
	return (ActorToTest && Actor == ActorToTest);
}

void DASHelpers::DisableCollisionOfComponent(UPrimitiveComponent* Component) {
	Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Component->SetGenerateOverlapEvents(false);
}

void DASHelpers::ForceGridPanelUpdate(UGridPanel* GridPanel) {
	
	if (!GridPanel)
		return;

	TSharedPtr<SWidget> SlateWidget = GridPanel->GetCachedWidget();
	if (!SlateWidget.IsValid())
		return;

	//Runtime, try to directly get sGridPanel
	SGridPanel* sGridPanel = static_cast<SGridPanel*>(SlateWidget.Get());

	//In design time sGridPanel is stored in SOverlay, it's because GP_Inventory is stored in Overlay? Or default behavior? probably default
	if (sGridPanel->GetType() != FName("SGridPanel") && GridPanel->IsDesignTime()) {
		SOverlay* sOverlay = static_cast<SOverlay*>(SlateWidget.Get());
		sOverlay->GetAllChildren()->ForEachWidget([&sGridPanel] (SWidget& widget)
				{
				if (widget.GetType() == FName("SGridPanel")) {
					sGridPanel = static_cast<SGridPanel*>(&widget);
				}
				}
			);
	}
	if (!sGridPanel) {
		WIZ_RET_LOG(, "Can't find SGridPanel", Error);
	}

	//Literally set the same value for column and row
	for (int32 i = 0; i < GridPanel->ColumnFill.Num(); i++) {
		sGridPanel->SetColumnFill(i, GridPanel->ColumnFill[i]);
	}

	for (int32 i = 0; i < GridPanel->RowFill.Num(); i++) {
		sGridPanel->SetRowFill(i, GridPanel->RowFill[i]);
	}

	SlateWidget->Invalidate(EInvalidateWidgetReason::Layout);
}

bool DASHelpers::IsHosting(UWorld* ContextWorld) {
	
	if (!ContextWorld) {
		WIZ_RET_LOG(false, "World invalid to check hosting!", Error);
	}
	
	if (ContextWorld->GetNetMode() == NM_ListenServer || ContextWorld->GetNetMode() == NM_DedicatedServer)
		return true;
	
	return false;
}

bool DASHelpers::IsClientOfHost(UWorld* ContextWorld) {
	
	if (!ContextWorld) {
		WIZ_RET_LOG(true, "World invalid to check client!", Error);
	}
	
	if (ContextWorld->GetNetMode() == NM_Client)
		return true;
	
	return false;
}

bool DASHelpers::IsStandaloneWorld(UWorld* ContextWorld) {

	if (!ContextWorld) {
		WIZ_RET_LOG(true, "World invalid to check standalone!", Error);
	}
	
	if (ContextWorld->GetNetMode() == NM_Standalone)
		return true;
	
	return false;
	
}

bool DASHelpers::IsStandaloneWorldOrAloneInServer(UWorld* ContextWorld) {
	
	if (!ContextWorld) {
		WIZ_RET_LOG(true, "World invalid to check standalone!", Error);
	}
	
	AGameStateBase* GameState = ContextWorld->GetGameState();
	if (GameState == NULL) {
		WIZ_RET_LOG(true, "GameState invalid to check alone!", Error);
	}
	
	if (DASHelpers::IsClientOfHost(ContextWorld))
		return false;
	
	int NumPlayers = GameState->PlayerArray.Num();
	if (NumPlayers == 0) {
		WIZ_RET_LOG(true, "0 player connected in the server?", Error);
	}
	
	bool bStandalone = IsStandaloneWorld(ContextWorld);
	bool bAlone = NumPlayers == 1; 

	return (bStandalone or bAlone);
}

void DASHelpers::SetRichText(URichTextBlock* RichTextBlock, const FText& Text, const FText& RichID) {
	RichTextBlock->SetText(FText::FromString(FString::Printf(TEXT("<%s>%s</>"), *RichID.ToString(), *Text.ToString())));
}

#pragma region Logs
#if !UE_BUILD_SHIPPING
void DASHelpers::PrivateInternal_WizDebugLog(
	const ANSICHAR* FunctionSignature,
	int32 FileLine,
	const FString& Message,
	ELogVerbosity::Type DebugLogType,
	FLogCategoryBase& LogCategory,
	bool bAddOnScreenMessage,
	FColor OnScreenMessageColor,
	float OnScreenMessageDuration,
	uint64 OnScreenMessageKey
) {
	
	FString FinalMessage = Message;
	if (FunctionSignature) {
		FString Prefix = DASHelpers::ParseFunctionPrefixFromAnsi(FunctionSignature, FileLine);
		if (!Prefix.IsEmpty()) {
			FinalMessage = FString::Printf(TEXT("%s %s"), *Prefix, *Message);
		}
	}

	if (!LogCategory.IsSuppressed(DebugLogType)) {
		switch (DebugLogType) {
			case ELogVerbosity::NoLogging: break;
			case ELogVerbosity::Fatal: UE_LOG_REF(LogCategory, Fatal, TEXT("%s"), *FinalMessage);
				break;
			case ELogVerbosity::Error: UE_LOG_REF(LogCategory, Error, TEXT("%s"), *FinalMessage);
				break;
			case ELogVerbosity::Warning: UE_LOG_REF(LogCategory, Warning, TEXT("%s"), *FinalMessage);
				break;
			case ELogVerbosity::Display: UE_LOG_REF(LogCategory, Display, TEXT("%s"), *FinalMessage);
				break;
			case ELogVerbosity::Log: UE_LOG_REF(LogCategory, Log, TEXT("%s"), *FinalMessage);
				break;
			case ELogVerbosity::Verbose: UE_LOG_REF(LogCategory, Verbose, TEXT("%s"), *FinalMessage);
				break;
			case ELogVerbosity::VeryVerbose: UE_LOG_REF(LogCategory, VeryVerbose, TEXT("%s"), *FinalMessage);
				break;
			default: UE_LOG_REF(LogCategory, Log, TEXT("%s"), *FinalMessage);
				break;
		}
	}

	if (bAddOnScreenMessage && GEngine) {
		GEngine->AddOnScreenDebugMessage(OnScreenMessageKey, OnScreenMessageDuration, OnScreenMessageColor, FinalMessage);
	}
}

FString DASHelpers::ParseFunctionPrefixFromString(FString FunctionSignature, int32 Line) {
	int32 ParenIdx = INDEX_NONE;
	if (FunctionSignature.FindChar('(', ParenIdx)) {
		FunctionSignature = FunctionSignature.Left(ParenIdx);
	}

	FunctionSignature = FunctionSignature.Replace(TEXT("class "), TEXT(""));
	FunctionSignature = FunctionSignature.Replace(TEXT("struct "), TEXT(""));
	FunctionSignature = FunctionSignature.Replace(TEXT("enum "), TEXT(""));

	FunctionSignature.TrimStartAndEndInline();

	int32 LastSpaceIdx = INDEX_NONE;
	if (FunctionSignature.FindLastChar(' ', LastSpaceIdx)) {
		FunctionSignature = FunctionSignature.Mid(LastSpaceIdx + 1);
	}

	FunctionSignature = FunctionSignature.Replace(TEXT(" noexcept"), TEXT(""));
	FunctionSignature = FunctionSignature.Replace(TEXT(" override"), TEXT(""));
	FunctionSignature = FunctionSignature.Replace(TEXT(" const"), TEXT(""));

	FunctionSignature = FunctionSignature.Replace(TEXT("  "), TEXT(" "));
	FunctionSignature.TrimStartAndEndInline();

	if (Line >= 0) {
		return FString::Printf(TEXT("%s(%d):"), *FunctionSignature, Line);
	}
	else {
		return FString::Printf(TEXT("%s:"), *FunctionSignature);
	}
}

FString DASHelpers::ParseFunctionPrefixFromAnsi(const ANSICHAR* FunctionSignatureAnsi, int32 Line) {
	if (!FunctionSignatureAnsi) {
		return Line >= 0
				   ? FString::Printf(TEXT("?(%d):"), Line)
				   : FString(TEXT("?:"));
	}
	return ParseFunctionPrefixFromString(ANSI_TO_TCHAR(FunctionSignatureAnsi), Line);
}

FString DASHelpers::ParseFunctionPrefixFromTChar(const TCHAR* FunctionSignatureTChar, int32 Line) {
	if (!FunctionSignatureTChar) {
		return Line >= 0
				   ? FString::Printf(TEXT("?(%d):"), Line)
				   : FString(TEXT("?:"));
	}
	return ParseFunctionPrefixFromString(FString(FunctionSignatureTChar), Line);
}
#endif
#pragma endregion