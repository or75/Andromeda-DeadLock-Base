#include "CL_VisibleCheck.hpp"

#include <DeadLock/SDK/Types/CEntityData.hpp>
#include <GameClient/CL_Trace.hpp>

static CL_VisibleCheck g_CL_VisibleCheck{};

auto CL_VisibleCheck::IsPlayerControllerVisible( const Vector3& CameraPos , CCitadelPlayerController* pCCitadelPlayerController ) -> bool
{
	auto Visible = false;

	if ( pCCitadelPlayerController )
		Visible = IsPlayerPawnVisible( CameraPos , pCCitadelPlayerController->m_hPawn().Get<C_CitadelPlayerPawn>() );

	return Visible;
}

auto CL_VisibleCheck::IsPlayerPawnVisible( const Vector3& CameraPos , C_CitadelPlayerPawn* pC_CitadelPlayerPawn ) -> bool
{
	return IsBaseEntityVisible( CameraPos , pC_CitadelPlayerPawn , g_AllTraceVisibleHeroCheckBones );
}

auto CL_VisibleCheck::IsTropperVisible( const Vector3& CameraPos , C_NPC_Trooper* pC_BaseEntity ) -> bool
{
	return IsBaseEntityVisible( CameraPos , pC_BaseEntity , g_AllTraceVisibleTropperCheckBones );
}

auto CL_VisibleCheck::IsTropperNeutralVisible( const Vector3& CameraPos , C_NPC_TrooperNeutral* pC_NPC_TrooperNeutral ) -> bool
{
	return IsBaseEntityVisible( CameraPos , pC_NPC_TrooperNeutral , g_AllTraceVisibleTropperNeutralCheckBones );
}

auto CL_VisibleCheck::IsBaseEntityVisible( const Vector3& CameraPos , C_BaseEntity* pC_BaseEntity , const VecCheckBones_t& CheckBones ) -> bool
{
	auto Visible = false;

	if ( pC_BaseEntity )
	{
		bool IsBoneVisible = false;

		for ( const auto& BoneName : CheckBones )
		{
			Vector3 BonePos = GetCL_Bones()->GetBonePositionByName( pC_BaseEntity , BoneName.data() );

			if ( !BonePos.IsZero() )
			{
				const auto* pTracedEntity = GetCL_Trace()->TraceToEntityEndPos( CameraPos , &BonePos );

				if ( pTracedEntity == pC_BaseEntity )
					IsBoneVisible = true;
				else
					continue;
			}
			else
			{
				IsBoneVisible = false;
			}
		}

		Visible = IsBoneVisible;
	}

	return Visible;
}

auto GetCL_VisibleCheck() -> CL_VisibleCheck*
{
	return &g_CL_VisibleCheck;
}
