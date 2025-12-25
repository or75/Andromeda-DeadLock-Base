#pragma once

#include <vector>
#include <Common/Common.hpp>

#include <DeadLock/SDK/SDK.hpp>
#include <DeadLock/CBasePattern.hpp>

#define DECLARATE_DEADLOCK_FUNCTION_SDK_FASTCALL(Ret,Function,Param,UsingParam,CallParam)\
inline Ret Function Param\
{\
	using Fn = Ret ( __fastcall* ) UsingParam;\
	Fn Original = static_cast<Fn>( GetFunctionList()->##Function##.GetFunction() );\
	return Original##CallParam##;\
}

class CFunctionList final
{
public:
	auto OnInit() -> bool;

public:
	CBasePattern CSkeletonInstance_CalcWorldSpaceBones = { VmpStr( "CSkeletonInstance::CalcWorldSpaceBones" ) , VmpStr( "48 89 4C 24 ? 55 53 56 57 41 54 41 55 41 56 41 57 B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 48 8D 6C 24 ? 48 8B 81" ) , CLIENT_DLL , 0 , SEARCH_TYPE_NONE };
	CBasePattern ScreenTransform = { VmpStr( "ScreenTransform" ) , VmpStr( "48 89 5C 24 ? 57 48 83 EC ? 48 8B F9 48 8B DA 48 8B 0D ? ? ? ? 48 85 C9 0F 84" ) , CLIENT_DLL , 0 , SEARCH_TYPE_NONE };
	CBasePattern CCitadelInput_GetViewAngles = { VmpStr( "CCitadelInput::GetViewAngles" ) , VmpStr( "E8 ? ? ? ? EB ? 48 8B 01 48 8D 54 24" ) , CLIENT_DLL , 0 , SEARCH_TYPE_CALL };
	CBasePattern CGameEntitySystem_GetBaseEntity = { VmpStr( "CGameEntitySystem::GetBaseEntity" ) , VmpStr( "4C 8D 49 ? 81 FA ? ? ? ? 77" ) , CLIENT_DLL };
	CBasePattern CGameEntitySystem_GetLocalCitadelPlayerController = { VmpStr( "CGameEntitySystem::GetLocalCitadelPlayerController" ) , VmpStr( "E8 ? ? ? ? 48 8B C8 E8 ? ? ? ? 48 3B C3 0F 84" ) , CLIENT_DLL , 0 , SEARCH_TYPE_CALL };
	CBasePattern IGameEvent_GetName = { VmpStr( "IGameEvent::GetName" ) , VmpStr( "8B 41 14 0F BA E0 1E 73 05 48 8D 41 18 C3" ) , CLIENT_DLL , 0 , SEARCH_TYPE_NONE };
	CBasePattern GetCUserCmdTick = { VmpStr( "GetCUserCmdTick" ) , VmpStr( "48 83 EC ? 4C 8B 0D ? ? ? ? 4C 8B DA" ) , CLIENT_DLL , 0 , SEARCH_TYPE_NONE };
	CBasePattern GetCUserCmdArray = { VmpStr( "GetCUserCmdArray" ) , VmpStr( "48 89 4C 24 ? 41 56 41 57" ) , CLIENT_DLL , 0 , SEARCH_TYPE_NONE };
	CBasePattern GetCUserCmdBySequenceNumber = { VmpStr( "GetCUserCmdBySequenceNumber" ) , VmpStr( "40 53 48 83 EC ? 8B DA E8 ? ? ? ? 4C 8B C0" ) , CLIENT_DLL , 0 , SEARCH_TYPE_NONE };
	CBasePattern C_EnvSky_Update = { VmpStr( "C_EnvSky::Update" ) , VmpStr( "40 53 48 83 EC 30 48 8B D9 E8 ? ? ? ? 48 8B 43" ) , CLIENT_DLL , 0 , SEARCH_TYPE_NONE };
	CBasePattern C_BaseEntity_GetBoneIdByName = { VmpStr( "C_BaseEntity::GetBoneIdByName" ) , VmpStr( "40 53 48 83 EC 20 48 8B 89 ? ? ? ? 48 8B DA 48 8B 01 FF 50 ? 48 8B C8" ) , CLIENT_DLL , 0 , SEARCH_TYPE_NONE };
	CBasePattern C_BaseEntity_GetHitBoxSet = { VmpStr( "C_BaseEntity::GetHitBoxSet" ) , VmpStr( "48 89 5C 24 ? 48 89 74 24 ? 57 48 81 EC 40 01 00 00 8B DA" ) , CLIENT_DLL , 0 , SEARCH_TYPE_NONE };
};

auto GetFunctionList() -> CFunctionList*;
