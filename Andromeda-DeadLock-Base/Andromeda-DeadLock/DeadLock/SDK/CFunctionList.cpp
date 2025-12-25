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
		&C_EnvSky_Update,
		&C_BaseEntity_GetBoneIdByName,
		&C_BaseEntity_GetHitBoxSet,
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
