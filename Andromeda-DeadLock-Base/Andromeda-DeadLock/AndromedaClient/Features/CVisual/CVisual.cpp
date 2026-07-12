#include "CVisual.hpp"

#include <algorithm>

#include <DeadLock/SDK/SDK.hpp>
#include <DeadLock/SDK/Math/Math.hpp>
#include <DeadLock/SDK/Update/CUserCmd.hpp>
#include <DeadLock/SDK/Types/CEntityData.hpp>

#include <DeadLock/SDK/Interface/CGameEntitySystem.hpp>
#include <DeadLock/SDK/Interface/IEngineToClient.hpp>

#include <GameClient/CEntityCache/CEntityCache.hpp>

#include <GameClient/CL_CitadelPlayerPawn.hpp>
#include <GameClient/CL_CitadelPlayerController.hpp>
#include <GameClient/CL_Bones.hpp>
#include <GameClient/CL_VisibleCheck.hpp>

#include <AndromedaClient/Settings/Settings.hpp>
#include <AndromedaClient/Fonts/CFontManager.hpp>
#include <AndromedaClient/Render/CRenderStackSystem.hpp>

static CVisual g_CVisual{};

auto CVisual::OnRender() -> void
{
	const auto CachedVec = GetEntityCache()->GetCachedEntity();

	std::scoped_lock Lock( GetEntityCache()->GetLock() );

	for ( const auto& CachedEntity : *CachedVec )
	{
		auto pEntity = CachedEntity.m_Handle.Get();

		if ( !pEntity )
			continue;

		auto hEntity = pEntity->pEntityIdentity()->Handle();

		if ( hEntity != CachedEntity.m_Handle )
			continue;

		switch ( CachedEntity.m_Type )
		{
			case CachedEntity_t::CITADEL_PLAYER_CONTROLLER:
			{
				auto* pCCitadelPlayerController = reinterpret_cast<CCitadelPlayerController*>( pEntity );

				if ( pCCitadelPlayerController != GetCL_CitadelPlayerController()->GetLocal() )
					OnRenderPlayerEsp( pCCitadelPlayerController , CachedEntity.m_bVisible );
			}
			break;
			case CachedEntity_t::NPC_TROOPER:
			{
				auto* pC_NPC_Trooper = reinterpret_cast<C_NPC_Trooper*>( pEntity );

				if ( CachedEntity.m_bDraw )
					OnRenderTrooperEsp( pC_NPC_Trooper , CachedEntity.m_Bbox , CachedEntity.m_bVisible );
			}
			break;
			case CachedEntity_t::NPC_TROOPER_NEUTRAL:
			{
				auto* pC_NPC_TrooperNeutral = reinterpret_cast<C_NPC_TrooperNeutral*>( pEntity );

				if ( CachedEntity.m_bDraw )
					OnRenderTrooperNeutralEsp( pC_NPC_TrooperNeutral , CachedEntity.m_bVisible );
			}
			break;
			default:
				break;
		}
	}

	if ( Settings::Visual::SoundStepEsp )
		OnRenderSound();
}

auto CVisual::OnStartSound( const Vector3& Pos , const int SourceEntityIndex , const char* szSoundName ) -> void
{
	if ( strstr( szSoundName , XorStr( "Footstep" ) ) )
	{
		if ( auto* pBaseEntity = SDK::Interfaces::GameEntitySystem()->GetBaseEntity( SourceEntityIndex ); pBaseEntity )
		{
			if ( auto* pLocalCitadelPlayerController = GetCL_CitadelPlayerController()->GetLocal(); pLocalCitadelPlayerController )
			{
				if ( pLocalCitadelPlayerController->m_iTeamNum() != pBaseEntity->m_iTeamNum() )
				{
					if ( pBaseEntity->IsCitadelPlayerPawn() )
					{
						std::scoped_lock m_Lock( m_SoundLock );

						m_SoundList.emplace_back( GetTickCount64() , Pos );
					}
				}
			}
		}
	}
}

auto CVisual::OnClientOutput() -> void
{
	if ( !Settings::Visual::Active )
		return;

	OnRender();
}

auto CVisual::OnCreateMove( CUserCmd* pCUserCmd ) -> void
{
	const auto CachedVec = GetEntityCache()->GetCachedEntity();

	Vector3 CameraPos = Vector3( pCUserCmd->cmd.vec_camera_position().x() ,
								 pCUserCmd->cmd.vec_camera_position().y() ,
								 pCUserCmd->cmd.vec_camera_position().z() );

	for ( auto& CachedEntity : *CachedVec )
	{
		auto pEntity = CachedEntity.m_Handle.Get();

		if ( !pEntity )
			continue;

		auto hEntity = pEntity->pEntityIdentity()->Handle();

		if ( hEntity != CachedEntity.m_Handle )
			continue;

		switch ( CachedEntity.m_Type )
		{
			case CachedEntity_t::CITADEL_PLAYER_CONTROLLER:
			{
				auto* pCCitadelPlayerController = reinterpret_cast<CCitadelPlayerController*>( pEntity );

				if ( pCCitadelPlayerController != GetCL_CitadelPlayerController()->GetLocal() )
					CachedEntity.m_bVisible = GetCL_VisibleCheck()->IsPlayerControllerVisible( CameraPos , pCCitadelPlayerController );
			}
			break;
			case CachedEntity_t::NPC_TROOPER:
			{
				auto* pC_NPC_Trooper = reinterpret_cast<C_NPC_Trooper*>( pEntity );
				CachedEntity.m_bVisible = GetCL_VisibleCheck()->IsTropperVisible( CameraPos , pC_NPC_Trooper );
			}
			break;
			case CachedEntity_t::NPC_TROOPER_NEUTRAL:
			{
				auto* pC_NPC_TrooperNeutral = reinterpret_cast<C_NPC_TrooperNeutral*>( pEntity );
				CachedEntity.m_bVisible = GetCL_VisibleCheck()->IsTropperNeutralVisible( CameraPos , pC_NPC_TrooperNeutral );
			}
			break;
			default:
				break;
		}
	}
}

auto CVisual::OnRenderSound() -> void
{
	std::scoped_lock m_Lock( m_SoundLock );

	auto NewEnd = std::remove_if( m_SoundList.begin() , m_SoundList.end() , []( const SoundData_t& Sound )
	{
		return GetTickCount64() - Sound.dwTime >= g_SoundShowTime;
	} );

	m_SoundList.erase( NewEnd , m_SoundList.end() );

	for ( const auto& Sound : m_SoundList )
	{
		auto Ratio = static_cast<float>( GetTickCount64() - Sound.dwTime ) / static_cast<float>( g_SoundShowTime );
		auto Alpha = std::lerp( 1.f , 0.f , Ratio );

		ImVec2 Screen;

		if ( Math::WorldToScreen( Sound.Pos , Screen ) )
		{
			constexpr static auto SoundSize = 20.f;
			auto Radius = std::lerp( SoundSize , 0.f , Ratio );

			GetRenderStackSystem()->DrawCircle3D( Sound.Pos , Radius , ImColor( 1.f , 1.f , 0.f , Alpha ) );
		}
	}
}

auto CVisual::OnRenderPlayerEsp( CCitadelPlayerController* pCCitadelPlayerController , const bool bVisible ) -> void
{
	if ( !pCCitadelPlayerController->IsAlive() )
		return;

	auto* pC_CitadelPlayerPawn = pCCitadelPlayerController->m_hHeroPawn().Get<C_CitadelPlayerPawn>();

	if ( pC_CitadelPlayerPawn && !pC_CitadelPlayerPawn->m_pGameSceneNode()->m_bDormant() )
	{
		const auto vOrigin = pC_CitadelPlayerPawn->m_vOldOrigin();

		Vector3 vHeadPos;
		Rect_t bBox;

		auto Draw = false;
		auto ForceDraw = false;

		if ( !GetCL_CitadelPlayerController()->GetLocal() )
			ForceDraw = true;

		const auto LocalPlayerControllerTeamNum = ForceDraw ? 0 : GetCL_CitadelPlayerController()->GetLocal()->m_iTeamNum();
		const auto IsEnemy = pCCitadelPlayerController->m_iTeamNum() != LocalPlayerControllerTeamNum;

		if ( Settings::Visual::HeroEnemy && IsEnemy )
			Draw = true;

		if ( Settings::Visual::HeroTeam && !IsEnemy )
			Draw = true;

		if ( Settings::Visual::OnlyVisible && !bVisible && Draw )
			Draw = false;

		if ( ForceDraw && !Draw )
			Draw = true;

		vHeadPos = GetCL_Bones()->GetBonePositionByName( pC_CitadelPlayerPawn , XorStr( "head" ) );

		if ( Draw )
		{
			if ( !vOrigin.IsZero() && !vHeadPos.IsZero() )
			{
				ImVec2 OriginScreen , HeadScreen;

				auto W2SOrigin = Math::WorldToScreen( vOrigin , OriginScreen );
				auto W2SHead = Math::WorldToScreen( vHeadPos , HeadScreen );

				if ( W2SOrigin && W2SHead )
				{
					const auto BoxHeight = floor( HeadScreen.y - OriginScreen.y );
					const auto BoxWidth = floor( BoxHeight / 2.f );

					bBox.x = floor( HeadScreen.x - BoxWidth / 2.f );
					bBox.y = floor( HeadScreen.y );
					bBox.w = floor( HeadScreen.x + BoxWidth / 2.f );
					bBox.h = floor( OriginScreen.y );

					const ImVec2 min = { bBox.x, bBox.y };
					const ImVec2 max = { bBox.w, bBox.h };

					auto PlayerColor = ImColor( 255 , 255 , 255 );

					if ( IsEnemy )
					{
						PlayerColor = ImColor( Settings::Colors::Visual::HeroEnemy[0] , Settings::Colors::Visual::HeroEnemy[1] , Settings::Colors::Visual::HeroEnemy[2] );

						if ( bVisible )
							PlayerColor = ImColor( Settings::Colors::Visual::HeroEnemyVisible[0] , Settings::Colors::Visual::HeroEnemyVisible[1] , Settings::Colors::Visual::HeroEnemyVisible[2] );
					}
					else
					{
						PlayerColor = ImColor( Settings::Colors::Visual::HeroTeam[0] , Settings::Colors::Visual::HeroTeam[1] , Settings::Colors::Visual::HeroTeam[2] );

						if ( bVisible )
							PlayerColor = ImColor( Settings::Colors::Visual::HeroTeamVisible[0] , Settings::Colors::Visual::HeroTeamVisible[1] , Settings::Colors::Visual::HeroTeamVisible[2] );
					}

					if ( Settings::Visual::HeroBox )
					{
						if ( Settings::Visual::HeroBoxType == EVisualBoxType_t::BOX )
							GetRenderStackSystem()->DrawBox( min , max , PlayerColor );
						else if ( Settings::Visual::HeroBoxType == EVisualBoxType_t::OUTLINE_BOX )
							GetRenderStackSystem()->DrawOutlineBox( min , max , PlayerColor );
						else if ( Settings::Visual::HeroBoxType == EVisualBoxType_t::COAL_BOX )
							GetRenderStackSystem()->DrawCoalBox( min , max , PlayerColor );
						else if ( Settings::Visual::HeroBoxType == EVisualBoxType_t::OUTLINE_COAL_BOX )
							GetRenderStackSystem()->DrawOutlineCoalBox( min , max , PlayerColor );
					}
				}
			}

			if ( Settings::Visual::HeroSkeleton )
				OnRenderHeroSkeleton( pC_CitadelPlayerPawn );
		}
	}
}

auto CVisual::OnRenderHeroSkeleton( C_CitadelPlayerPawn* pC_CitadelPlayerPawn ) -> void
{
	Vector3 BonePosStart , BonePosEnd;

	for ( const auto& Bones : g_AllSkeletonHeroPairBones )
	{
		const auto& [Start , End] = Bones;

		BonePosStart = GetCL_Bones()->GetBonePositionByName( pC_CitadelPlayerPawn , Start.c_str() );
		BonePosEnd = GetCL_Bones()->GetBonePositionByName( pC_CitadelPlayerPawn , End.c_str() );

		ImVec2 ScreenStart , ScreenEnd;

		if ( !BonePosStart.IsZero() && !BonePosEnd.IsZero() &&
			 Math::WorldToScreen( BonePosStart , ScreenStart ) &&
			 Math::WorldToScreen( BonePosEnd , ScreenEnd ) )
		{
			if ( !BonePosStart.IsZero() && !BonePosEnd.IsZero() )
			{
				GetRenderStackSystem()->DrawLine( ScreenStart , ScreenEnd ,
												  ImColor( Settings::Colors::Visual::HeroSkeleton[0] ,
														   Settings::Colors::Visual::HeroSkeleton[1] ,
														   Settings::Colors::Visual::HeroSkeleton[2] ) ,
												  2.f );
			}
		}
	}
}

auto CVisual::OnRenderTrooperEsp( C_NPC_Trooper* pC_NPC_Trooper , const Rect_t& bBox , const bool bVisible ) -> void
{
	if ( pC_NPC_Trooper->m_NPCState() == NPC_STATE_INIT ||
		 pC_NPC_Trooper->m_NPCState() == NPC_STATE_IDLE ||
		 pC_NPC_Trooper->m_NPCState() == NPC_STATE_ALERT ||
		 pC_NPC_Trooper->m_NPCState() == NPC_STATE_COMBAT )
	{
		const ImVec2 min = { floor( bBox.x ), floor( bBox.y ) };
		const ImVec2 max = { floor( bBox.w ), floor( bBox.h ) };

		auto Draw = false;
		auto ForceDraw = false;

		if ( !GetCL_CitadelPlayerController()->GetLocal() )
			ForceDraw = true;

		const auto LocalPlayerControllerTeamNum = ForceDraw ? 0 : GetCL_CitadelPlayerController()->GetLocal()->m_iTeamNum();
		const auto IsEnemy = pC_NPC_Trooper->m_iTeamNum() != LocalPlayerControllerTeamNum;

		if ( Settings::Visual::TrooperEnemy && IsEnemy )
			Draw = true;

		if ( Settings::Visual::TrooperTeam && !IsEnemy )
			Draw = true;

		if ( Settings::Visual::OnlyVisible && !bVisible && Draw )
			Draw = false;

		if ( ForceDraw && !Draw )
			Draw = true;

		if ( Draw )
		{
			auto TrooperColor = ImColor( 255 , 255 , 255 );

			if ( IsEnemy )
			{
				TrooperColor = ImColor( Settings::Colors::Visual::TrooperEnemy[0] , Settings::Colors::Visual::TrooperEnemy[1] , Settings::Colors::Visual::TrooperEnemy[2] );

				if ( bVisible )
					TrooperColor = ImColor( Settings::Colors::Visual::TrooperEnemyVisible[0] , Settings::Colors::Visual::TrooperEnemyVisible[1] , Settings::Colors::Visual::TrooperEnemyVisible[2] );
			}
			else
			{
				TrooperColor = ImColor( Settings::Colors::Visual::TrooperTeam[0] , Settings::Colors::Visual::TrooperTeam[1] , Settings::Colors::Visual::TrooperTeam[2] );

				if ( bVisible )
					TrooperColor = ImColor( Settings::Colors::Visual::TrooperTeamVisible[0] , Settings::Colors::Visual::TrooperTeamVisible[1] , Settings::Colors::Visual::TrooperTeamVisible[2] );
			}

			if ( Settings::Visual::TrooperBoxType == EVisualBoxType_t::BOX )
				GetRenderStackSystem()->DrawBox( min , max , TrooperColor );
			else if ( Settings::Visual::TrooperBoxType == EVisualBoxType_t::OUTLINE_BOX )
				GetRenderStackSystem()->DrawOutlineBox( min , max , TrooperColor );
			else if ( Settings::Visual::TrooperBoxType == EVisualBoxType_t::COAL_BOX )
				GetRenderStackSystem()->DrawCoalBox( min , max , TrooperColor );
			else if ( Settings::Visual::TrooperBoxType == EVisualBoxType_t::OUTLINE_COAL_BOX )
				GetRenderStackSystem()->DrawOutlineCoalBox( min , max , TrooperColor );

			if ( Settings::Visual::TrooperSkeleton )
				OnRenderTrooperSkeleton( pC_NPC_Trooper );
		}
	}
}

auto CVisual::OnRenderTrooperSkeleton( C_NPC_Trooper* pC_NPC_Trooper ) -> void
{
	Vector3 BonePosStart , BonePosEnd;

	for ( const auto& Bones : g_AllSkeletonTrooperPairBones )
	{
		const auto& [Start , End] = Bones;

		BonePosStart = GetCL_Bones()->GetBonePositionByName( pC_NPC_Trooper , Start.c_str() );
		BonePosEnd = GetCL_Bones()->GetBonePositionByName( pC_NPC_Trooper , End.c_str() );

		ImVec2 ScreenStart , ScreenEnd;

		if ( !BonePosStart.IsZero() && !BonePosEnd.IsZero() &&
				Math::WorldToScreen( BonePosStart , ScreenStart ) &&
				Math::WorldToScreen( BonePosEnd , ScreenEnd ) )
		{
			if ( !BonePosStart.IsZero() && !BonePosEnd.IsZero() )
			{
				GetRenderStackSystem()->DrawLine( ScreenStart , ScreenEnd ,
													ImColor( Settings::Colors::Visual::TrooperSkeleton[0] ,
															 Settings::Colors::Visual::TrooperSkeleton[1] ,
															 Settings::Colors::Visual::TrooperSkeleton[2] ) ,
													2.f );
			}
		}
	}
}

auto CVisual::OnRenderTrooperNeutralEsp( C_NPC_TrooperNeutral* pC_NPC_TrooperNeutral , const bool bVisible ) -> void
{
	if ( pC_NPC_TrooperNeutral->m_NPCState() == NPC_STATE_INIT ||
		 pC_NPC_TrooperNeutral->m_NPCState() == NPC_STATE_IDLE ||
		 pC_NPC_TrooperNeutral->m_NPCState() == NPC_STATE_ALERT ||
		 pC_NPC_TrooperNeutral->m_NPCState() == NPC_STATE_COMBAT )
	{
		if ( auto BoneNeckID = pC_NPC_TrooperNeutral->GetBoneIdByName( XorStr( "neck_0" ) ); BoneNeckID )
		{
			Rect_t bBox;
			Vector3 vNeckBonePos;

			if ( pC_NPC_TrooperNeutral->m_pGameSceneNode()->GetBonePosition( BoneNeckID , vNeckBonePos ) )
			{
				ImVec2 OriginScreen , NeckScreen;

				auto W2SOrigin = Math::WorldToScreen( pC_NPC_TrooperNeutral->GetOrigin() , OriginScreen );
				auto W2SHead = Math::WorldToScreen( vNeckBonePos , NeckScreen );

				if ( W2SOrigin && W2SHead && Settings::Visual::TrooperNeutral )
				{
					const auto BoxHeight = floor( OriginScreen.y - NeckScreen.y );
					const auto BoxWidth = floor( BoxHeight / 1.5f );

					bBox.x = floor( NeckScreen.x - BoxWidth );
					bBox.y = floor( NeckScreen.y );
					bBox.w = floor( NeckScreen.x + BoxWidth );
					bBox.h = floor( OriginScreen.y );

					const ImVec2 min = { bBox.x, bBox.y };
					const ImVec2 max = { bBox.w, bBox.h };

					auto TrooperNeutralColor = ImColor( Settings::Colors::Visual::TrooperNeutral[0] , Settings::Colors::Visual::TrooperNeutral[1] , Settings::Colors::Visual::TrooperNeutral[2] );

					if ( bVisible )
						TrooperNeutralColor = ImColor( Settings::Colors::Visual::TrooperNeutralVisible[0] , Settings::Colors::Visual::TrooperNeutralVisible[1] , Settings::Colors::Visual::TrooperNeutralVisible[2] );

					if ( Settings::Visual::TrooperNeutralBoxType == EVisualBoxType_t::BOX )
						GetRenderStackSystem()->DrawBox( min , max , TrooperNeutralColor );
					else if ( Settings::Visual::TrooperNeutralBoxType == EVisualBoxType_t::OUTLINE_BOX )
						GetRenderStackSystem()->DrawOutlineBox( min , max , TrooperNeutralColor );
					else if ( Settings::Visual::TrooperNeutralBoxType == EVisualBoxType_t::COAL_BOX )
						GetRenderStackSystem()->DrawCoalBox( min , max , TrooperNeutralColor );
					else if ( Settings::Visual::TrooperNeutralBoxType == EVisualBoxType_t::OUTLINE_COAL_BOX )
						GetRenderStackSystem()->DrawOutlineCoalBox( min , max , TrooperNeutralColor );
				}
			}

			if ( Settings::Visual::TrooperNeutralSkeleton )
				OnRenderTrooperNeutralSkeleton( pC_NPC_TrooperNeutral );
		}
	}
}

auto CVisual::OnRenderTrooperNeutralSkeleton( C_NPC_TrooperNeutral* pC_NPC_TrooperNeutral ) -> void
{
	Vector3 BonePosStart , BonePosEnd;

	for ( const auto& Bones : g_AllSkeletonTrooperNeutralPairBones )
	{
		const auto& [Start , End] = Bones;

		BonePosStart = GetCL_Bones()->GetBonePositionByName( pC_NPC_TrooperNeutral , Start.c_str() );
		BonePosEnd = GetCL_Bones()->GetBonePositionByName( pC_NPC_TrooperNeutral , End.c_str() );

		ImVec2 ScreenStart , ScreenEnd;

		if ( !BonePosStart.IsZero() && !BonePosEnd.IsZero() &&
				Math::WorldToScreen( BonePosStart , ScreenStart ) &&
				Math::WorldToScreen( BonePosEnd , ScreenEnd ) )
		{
			if ( !BonePosStart.IsZero() && !BonePosEnd.IsZero() )
			{
				GetRenderStackSystem()->DrawLine( ScreenStart , ScreenEnd ,
													ImColor( Settings::Colors::Visual::TrooperNeutralSkeleton[0] ,
															 Settings::Colors::Visual::TrooperNeutralSkeleton[1] ,
															 Settings::Colors::Visual::TrooperNeutralSkeleton[2] ) ,
													2.f );
			}
		}
	}
}

auto CVisual::CalculateBoundingBoxes() -> void
{
	if ( !SDK::Interfaces::EngineToClient()->IsInGame() )
		return;

	const auto& CachedVec = GetEntityCache()->GetCachedEntity();

	std::scoped_lock Lock( GetEntityCache()->GetLock() );

	for ( auto& it : *CachedVec )
	{
		auto pEntity = it.m_Handle.Get();

		if ( !pEntity )
			continue;

		auto hEntity = pEntity->pEntityIdentity()->Handle();

		if ( hEntity != it.m_Handle )
			continue;

		switch ( it.m_Type )
		{
			case CachedEntity_t::NPC_TROOPER:
			{
				auto* pC_NPC_Trooper = reinterpret_cast<C_NPC_Trooper*>( pEntity );
				it.m_bDraw = pC_NPC_Trooper->GetBoundingBox( it.m_Bbox ) && !pC_NPC_Trooper->m_pGameSceneNode()->m_bDormant();
				break;
			}
			case CachedEntity_t::NPC_TROOPER_NEUTRAL:
			{
				auto* pC_NPC_TrooperNeutral = reinterpret_cast<C_NPC_TrooperNeutral*>( pEntity );
				it.m_bDraw = !pC_NPC_TrooperNeutral->m_pGameSceneNode()->m_bDormant();
				break;
			}
			case CachedEntity_t::ITEM_XP:
			{
				auto* pCItemXP = reinterpret_cast<CItemXP*>( pEntity );
				it.m_bDraw = pCItemXP->GetBoundingBox( it.m_Bbox ) && !pCItemXP->m_pGameSceneNode()->m_bDormant();
				break;
			}
		}
	}
}

auto GetVisual() -> CVisual*
{
	return &g_CVisual;
}
