// Copyright Wiz Productions. All Rights Reserved.

#pragma once

#include "CoreMinimal.h" 
#include "Engine/EngineTypes.h"

enum class ETriggerEvent : uint8;
class AActor;
class UWorld;
class UPrimitiveComponent;
class URichTextBlock;
class UGridPanel;

namespace DASHelpers
{

//#############################################################################
//##-------------------------------- HELPERS --------------------------------##
//#############################################################################

/** Returns true if called during the level editor, false on PIE, commandlet, and release */
DEBUGACTIONSSYSTEMHELPERS_API bool IsInLevelEditor();
DEBUGACTIONSSYSTEMHELPERS_API bool IsCurrentPlayer(const UWorld* WorldContext, int32 playerIndex, const class AActor* ActorToTest);
DEBUGACTIONSSYSTEMHELPERS_API void DisableCollisionOfComponent(UPrimitiveComponent* Component);
DEBUGACTIONSSYSTEMHELPERS_API void ForceGridPanelUpdate(UGridPanel* GridPanel);
DEBUGACTIONSSYSTEMHELPERS_API bool IsHosting(UWorld* ContextWorld);
DEBUGACTIONSSYSTEMHELPERS_API bool IsClientOfHost(UWorld* ContextWorld);
DEBUGACTIONSSYSTEMHELPERS_API bool IsStandaloneWorld(UWorld* ContextWorld);
DEBUGACTIONSSYSTEMHELPERS_API bool IsStandaloneWorldOrAloneInServer(UWorld* ContextWorld);
DEBUGACTIONSSYSTEMHELPERS_API void SetRichText(URichTextBlock* RichTextBlock, const FText& Text, const FText& RichID);

template <typename C>
C* AddComponentInConstruction(AActor* Owner, EComponentMobility::Type MobilityType);

#pragma region Logs
#if !UE_BUILD_SHIPPING

#if defined(_MSC_VER)
#define WIZ_FUNCTION_SIG __FUNCSIG__
#else
#define WIZ_FUNCTION_SIG __PRETTY_FUNCTION__
#endif

/** 	const FString& Message = "NoLogMessage, use WIZ_LOG macro!"\n
*		ELogVerbosity::Type DebugLogType = ELogVerbosity::Type::Log\n
*		FLogCategoryBase& LogCategory = LogTemp\n
*		bool bAddOnScreenMessage = false\n
*		uint64 OnScreenMessageKey = INDEX_NONE\n
*		float OnScreenMessageDuration = 2.f\n
*		FColor OnScreenMessageColor = FColor::White\n
*/
#define WIZ_LOG(Message, Verbosity, ...) \
DASHelpers::PrivateInternal_WizDebugLog(WIZ_FUNCTION_SIG, __LINE__, Message, ELogVerbosity::Type::Verbosity, ##__VA_ARGS__)

/** A specific version of WIZ_LOG with a return value. \n
*   eg: WIZ_RET_LOG( , "Access denied", Error) \n
*   \n
* 	const FString& Message = "NoLogMessage, use WIZ_LOG macro!"\n
*		ELogVerbosity::Type DebugLogType = ELogVerbosity::Type::Log\n
*		FLogCategoryBase& LogCategory = LogTemp\n
*		bool bAddOnScreenMessage = false\n
*		FColor OnScreenMessageColor = FColor::White)\n
*		float OnScreenMessageDuration = 2.f\n
*		uint64 OnScreenMessageKey = INDEX_NONE\n
*/
#define WIZ_RET_LOG(ReturnValue, Message, Verbosity, ...) \
	do { \
		WIZ_LOG(Message, Verbosity, ##__VA_ARGS__); \
		return ReturnValue; \
} while(0)

#define WIZ_NET_LOG(Message, Verbosity, ...) \
do { \
	const FString LocalRoleStr = UEnum::GetValueAsString(GetLocalRole()); \
	const FString RemoteRoleStr = UEnum::GetValueAsString(GetRemoteRole()); \
	const FString ContextPrefix = FString::Printf(TEXT("[%s/%s] "), *LocalRoleStr, *RemoteRoleStr); \
	WIZ_LOG(ContextPrefix + Message, Verbosity, ##__VA_ARGS__); \
} while(0)

#define WIZ_NET_RET_LOG(ReturnValue, Message, Verbosity, ...) \
do { \
	const FString LocalRoleStr = UEnum::GetValueAsString(GetLocalRole()); \
	const FString RemoteRoleStr = UEnum::GetValueAsString(GetRemoteRole()); \
	const FString ContextPrefix = FString::Printf(TEXT("[%s/%s] "), *LocalRoleStr, *RemoteRoleStr); \
	WIZ_RET_LOG(ReturnValue, ContextPrefix + Message, Verbosity, ##__VA_ARGS__); \
} while(0)

#define WIZ_NET_LOG_C(AuthorityCheckActor, Message, Verbosity, ...) \
do { \
	const FString LocalRoleStr = UEnum::GetValueAsString(AuthorityCheckActor->GetLocalRole()); \
	const FString RemoteRoleStr = UEnum::GetValueAsString(AuthorityCheckActor->GetRemoteRole()); \
	const FString ContextPrefix = FString::Printf(TEXT("[%s/%s] "), *LocalRoleStr, *RemoteRoleStr); \
	WIZ_LOG(ContextPrefix + Message, Verbosity, ##__VA_ARGS__); \
} while(0)

#else //--------------------------- SHIPPING ----------------------//.
#define WIZ_LOG(...) do { } while (0)
#define WIZ_RET_LOG(ReturnValue, ...) return ReturnValue;
#define WIZ_NET_LOG(...) do { } while (0)
#define WIZ_NET_RET_LOG(ReturnValue, ...) ReturnValue;
#define WIZ_NET_LOG_C(...) do { } while (0)
#endif


#define CheckedFString(String) String ? String : FString("None")
// Available on all AActor derived class (AI code)
// True on the Server or the Host (Listen Server). 
// Use this for logic that should only run on the source of truth (e.g., giving damage, spawning items).
#define SERVER_SIDE (GetLocalRole() == ROLE_Authority)
// True on any Client machine (Simulated or Autonomous).
#define CLIENT_SIDE (GetLocalRole() < ROLE_Authority)
// True if this actor is the one I am currently controlling on my screen.
// Perfect for hiding your own overhead nameplate or enabling local input.
#define IS_LOCAL_CONTROL (IsLocallyControlled())
// True if this actor belongs to another player (from the perspective of the local machine).
#define IS_REMOTE_CONTROL (!IsLocallyControlled())
// True for "ghost" copies of other players on your screen. 
// You only receive updates for these; you don't predict their movement.
#define IS_SIMULATED (GetLocalRole() == ROLE_SimulatedProxy)
// True for your own character on your client screen. 
// This role allows for Client-Side Prediction (moving before the server confirms).
#define IS_AUTONOMOUS (GetLocalRole() == ROLE_AutonomousProxy)

#if !UE_BUILD_SHIPPING
/** Don't use this method directly, use WIZ_LOG macro instead */
DEBUGACTIONSSYSTEMHELPERS_API void PrivateInternal_WizDebugLog(
	const ANSICHAR* FunctionSignature = nullptr,
	int32 FileLine                    = -1,
	const FString& Message            = "NoLogMessage, use WIZ_LOG macro!",
	ELogVerbosity::Type DebugLogType  = ELogVerbosity::Type::Log,
	FLogCategoryBase& LogCategory     = LogTemp,
	bool bAddOnScreenMessage          = false,
	FColor OnScreenMessageColor       = FColor::White,
	float OnScreenMessageDuration     = 7.f,
	uint64 OnScreenMessageKey         = INDEX_NONE
);
FString ParseFunctionPrefixFromString(FString FunctionSignature, int32 Line = -1);
FString ParseFunctionPrefixFromAnsi(const ANSICHAR* FunctionSignatureAnsi, int32 Line = -1);
FString ParseFunctionPrefixFromTChar(const TCHAR* FunctionSignatureTChar, int32 Line = -1);
#endif
#pragma endregion
};

#if CPP
#include "DASHelpers.inl"
#endif
