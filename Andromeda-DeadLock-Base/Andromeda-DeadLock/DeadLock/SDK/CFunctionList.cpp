#include "CFunctionList.hpp"

static CFunctionList g_CFunctionList{};

auto CFunctionList::OnInit() -> bool
{
	std::vector<CBasePattern*> vPatterns =
	{
		&CSkeletonInstance_CalcWorldSpaceBones,
		&ScreenTransform,
		&CCitadelInput_GetViewAngles,
		&CGameEntitySystem_GetBaseEntity,
		&CGameEntitySystem_GetLocalCitadelPlayerController,
		&IGameEvent_GetName,
		&GetCUserCmdTick,
		&GetCUserCmdArray,
		&GetCUserCmdBySequenceNumber,
		&C_BaseEntity_GetBoneIdByName,
		&C_BaseEntity_GetHitBoxSet,
		&CVPhys2World_TraceShape,
		&CTraceFilter_Constructor,
		&KeyValues3_LoadKV3,
	};

	auto Searched = true;

	for ( auto& Pattern : vPatterns )
	{
		if ( !Pattern->Search() )
			Searched = false;
	}

	return Searched;
}

auto GetFunctionList() -> CFunctionList*
{
	return &g_CFunctionList;
}
