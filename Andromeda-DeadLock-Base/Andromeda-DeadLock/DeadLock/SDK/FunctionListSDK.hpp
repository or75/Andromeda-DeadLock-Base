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
class C_EnvSky;
class CHitBoxSet;

DECLARATE_DEADLOCK_FUNCTION_SDK_FASTCALL( void , CSkeletonInstance_CalcWorldSpaceBones , ( CSkeletonInstance* pCSkeletonInstance , unsigned int mask ) , ( CSkeletonInstance* , unsigned int ) , ( pCSkeletonInstance , mask ) );
DECLARATE_DEADLOCK_FUNCTION_SDK_FASTCALL( bool , ScreenTransform , ( const Vector3& vOrigin , Vector3& vOut ) , ( const Vector3& , Vector3& ) , ( vOrigin , vOut ) );
DECLARATE_DEADLOCK_FUNCTION_SDK_FASTCALL( QAngle* , CCitadelInput_GetViewAngles , ( CCitadelInput* pCCitadelInput , int32_t slot ) , ( CCitadelInput* , int32_t ) , ( pCCitadelInput , slot ) );
DECLARATE_DEADLOCK_FUNCTION_SDK_FASTCALL( void* , CGameEntitySystem_GetBaseEntity , ( CGameEntitySystem* pGameEntitySystem , int iIndex ) , ( CGameEntitySystem* , int ) , ( pGameEntitySystem , iIndex ) );
DECLARATE_DEADLOCK_FUNCTION_SDK_FASTCALL( CCitadelPlayerController* , CGameEntitySystem_GetLocalCitadelPlayerController , ( int iSlot ) , ( int ) , ( iSlot ) );
DECLARATE_DEADLOCK_FUNCTION_SDK_FASTCALL( const char* , IGameEvent_GetName , ( IGameEvent* pIGameEvent ) , ( IGameEvent* ) , ( pIGameEvent ) );
DECLARATE_DEADLOCK_FUNCTION_SDK_FASTCALL( void , GetCUserCmdTick , ( CCitadelPlayerController* pCitadelPlayerController , int32_t* pOutputTick ) , ( CCitadelPlayerController* , int32_t* ) , ( pCitadelPlayerController , pOutputTick ) );
DECLARATE_DEADLOCK_FUNCTION_SDK_FASTCALL( CUserCmdArray* , GetCUserCmdArray , ( CUserCmd** ppCUserCmd , int Tick ) , ( CUserCmd** , int ) , ( ppCUserCmd , Tick ) );
DECLARATE_DEADLOCK_FUNCTION_SDK_FASTCALL( CUserCmd* , GetCUserCmdBySequenceNumber , ( CCitadelPlayerController* pCitadelPlayerController , uint32_t SequenceNumber ) , ( CCitadelPlayerController* , uint32_t ) , ( pCitadelPlayerController , SequenceNumber ) );
DECLARATE_DEADLOCK_FUNCTION_SDK_FASTCALL( void* , C_EnvSky_Update , ( C_EnvSky* pC_EnvSky ) , ( C_EnvSky* ) , ( pC_EnvSky ) );
DECLARATE_DEADLOCK_FUNCTION_SDK_FASTCALL( int , C_BaseEntity_GetBoneIdByName , ( C_BaseEntity* pC_BaseEntity , const char* szName ) , ( C_BaseEntity* , const char* ) , ( pC_BaseEntity , szName ) );
DECLARATE_DEADLOCK_FUNCTION_SDK_FASTCALL( CHitBoxSet* , C_BaseEntity_GetHitBoxSet , ( C_BaseEntity* pC_BaseEntity , uint32_t Index = 0 ) , ( C_BaseEntity* , uint32_t ) , ( pC_BaseEntity , Index ) );

inline auto CGameEntitySystem_GetHighestEntityIndex( CGameEntitySystem* pGameEntitySystem , int& HighestIdx ) -> void
{
	// FF 89 ? ? ? ? EB ? 48 85 F6
	HighestIdx = *(int32_t*)( (uintptr_t)pGameEntitySystem + g_OFFSET_CGameEntitySystem_GetHighestEntityIndex );
}
