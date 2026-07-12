#pragma once

#include <Common/Common.hpp>

#include <DeadLock/SDK/CFunctionList.hpp>
#include <DeadLock/SDK/Math/Vector3.hpp>
#include <DeadLock/SDK/Math/QAngle.hpp>
#include <DeadLock/SDK/Update/Offsets.hpp>

class CSkeletonInstance;
class CCitadelInput;
class C_BaseEntity;
class CGameEntitySystem;
class CCitadelPlayerController;
class IGameEvent;
class CUserCmdArray;
class CUserCmd;
class CHitBoxSet;
struct Ray_t;
class CTraceFilter;
class CGameTrace;
class KeyValues3;
class CUtlString;
struct KV3ID_t;

DECLARATE_DEADLOCK_FUNCTION_SDK_FASTCALL( void , CSkeletonInstance_CalcWorldSpaceBones , ( CSkeletonInstance* pCSkeletonInstance , unsigned int mask ) , ( CSkeletonInstance* , unsigned int ) , ( pCSkeletonInstance , mask ) );
DECLARATE_DEADLOCK_FUNCTION_SDK_FASTCALL( bool , ScreenTransform , ( const Vector3& vOrigin , Vector3& vOut ) , ( const Vector3& , Vector3& ) , ( vOrigin , vOut ) );
DECLARATE_DEADLOCK_FUNCTION_SDK_FASTCALL( QAngle* , CCitadelInput_GetViewAngles , ( CCitadelInput* pCCitadelInput , int32_t slot ) , ( CCitadelInput* , int32_t ) , ( pCCitadelInput , slot ) );
DECLARATE_DEADLOCK_FUNCTION_SDK_FASTCALL( void* , CGameEntitySystem_GetBaseEntity , ( CGameEntitySystem* pGameEntitySystem , int iIndex ) , ( CGameEntitySystem* , int ) , ( pGameEntitySystem , iIndex ) );
DECLARATE_DEADLOCK_FUNCTION_SDK_FASTCALL( CCitadelPlayerController* , CGameEntitySystem_GetLocalCitadelPlayerController , ( int iSlot ) , ( int ) , ( iSlot ) );
DECLARATE_DEADLOCK_FUNCTION_SDK_FASTCALL( const char* , IGameEvent_GetName , ( IGameEvent* pIGameEvent ) , ( IGameEvent* ) , ( pIGameEvent ) );
DECLARATE_DEADLOCK_FUNCTION_SDK_FASTCALL( void , GetCUserCmdTick , ( CCitadelPlayerController* pCitadelPlayerController , int32_t* pOutputTick ) , ( CCitadelPlayerController* , int32_t* ) , ( pCitadelPlayerController , pOutputTick ) );
DECLARATE_DEADLOCK_FUNCTION_SDK_FASTCALL( CUserCmdArray* , GetCUserCmdArray , ( CUserCmd** ppCUserCmd , int Tick ) , ( CUserCmd** , int ) , ( ppCUserCmd , Tick ) );
DECLARATE_DEADLOCK_FUNCTION_SDK_FASTCALL( CUserCmd* , GetCUserCmdBySequenceNumber , ( CCitadelPlayerController* pCitadelPlayerController , uint32_t SequenceNumber ) , ( CCitadelPlayerController* , uint32_t ) , ( pCitadelPlayerController , SequenceNumber ) );
DECLARATE_DEADLOCK_FUNCTION_SDK_FASTCALL( int , C_BaseEntity_GetBoneIdByName , ( C_BaseEntity* pC_BaseEntity , const char* szName ) , ( C_BaseEntity* , const char* ) , ( pC_BaseEntity , szName ) );
DECLARATE_DEADLOCK_FUNCTION_SDK_FASTCALL( CHitBoxSet* , C_BaseEntity_GetHitBoxSet , ( C_BaseEntity* pC_BaseEntity , uint32_t Index = 0 ) , ( C_BaseEntity* , uint32_t ) , ( pC_BaseEntity , Index ) );
DECLARATE_DEADLOCK_FUNCTION_SDK_FASTCALL( bool , CVPhys2World_TraceShape , ( CVPhys2World** ppCVPhys2World , Ray_t* ray , Vector3* Start , Vector3* End , CTraceFilter* pTraceFilter , CGameTrace* pGameTrace ) , ( CVPhys2World** , Ray_t* , Vector3* , Vector3* , CTraceFilter* , CGameTrace* ) , ( ppCVPhys2World , ray , Start , End , pTraceFilter , pGameTrace ) );
DECLARATE_DEADLOCK_FUNCTION_SDK_FASTCALL( void , CTraceFilter_Constructor , ( CTraceFilter* pThis , uint64_t uMask , void* pSkip1 , int nLayer , uint16_t unkNum ) , ( CTraceFilter* , void* , uint64_t , int , uint16_t ) , ( pThis , pSkip1 , uMask , nLayer , unkNum ) );
DECLARATE_DEADLOCK_FUNCTION_SDK_FASTCALL( bool , KeyValues3_LoadKV3 , ( KeyValues3* pKV3 , const char* vmatBuffer , KV3ID_t* pID ) , ( KeyValues3* , CUtlString* , const char* , KV3ID_t* , const char* , unsigned int ) , ( pKV3 , nullptr , vmatBuffer , pID , "" , 0 ) );

inline auto CGameEntitySystem_GetHighestEntityIndex( CGameEntitySystem* pGameEntitySystem , int& HighestIdx ) -> void
{
	// FF 89 ? ? ? ? EB ? 48 85 F6
	HighestIdx = *(int32_t*)( (uintptr_t)pGameEntitySystem + g_OFFSET_CGameEntitySystem_GetHighestEntityIndex );
}
