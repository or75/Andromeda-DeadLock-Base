#include "CVisual.hpp"

#include <algorithm>

#include <DeadLock/SDK/SDK.hpp>
#include <DeadLock/SDK/Math/Math.hpp>
#include <DeadLock/SDK/Types/CEntityData.hpp>

#include <DeadLock/SDK/Interface/CGameEntitySystem.hpp>
#include <DeadLock/SDK/Interface/IEngineToClient.hpp>

#include <GameClient/CEntityCache/CEntityCache.hpp>

#include <GameClient/CL_CitadelPlayerPawn.hpp>
#include <GameClient/CL_CitadelPlayerController.hpp>
#include <GameClient/CL_Bones.hpp>

#include <AndromedaClient/Settings/Settings.hpp>
#include <AndromedaClient/Fonts/CFontManager.hpp>
#include <AndromedaClient/Render/CRenderStackSystem.hpp>

static CVisual g_CVisual{};

auto CVisual::OnRender() -> void
{
	if ( !Settings::Visual::Active )
		return;

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
				auto* pC_CitadelPlayerPawn = pCCitadelPlayerController->m_hHeroPawn().Get<C_CitadelPlayerPawn>();

				if ( pC_CitadelPlayerPawn )
				{
					/*static bool DumpHitBox = true;

					if ( DumpHitBox )
					{
						if ( auto pLocalCitadelPlayerPawn = GetCL_CitadelPlayerPawn()->GetLocal(); pLocalCitadelPlayerPawn )
						{
							auto* pHitBoxSet = pLocalCitadelPlayerPawn->GetHitBoxSet();

							if ( pHitBoxSet )
							{
								for ( auto HitBoxIndex = 0; HitBoxIndex < pHitBoxSet->m_HitBoxes().Count(); HitBoxIndex++ )
								{
									DEV_LOG( "%i , %s -> %s\n" , HitBoxIndex ,
											 pHitBoxSet->m_HitBoxes()[HitBoxIndex].m_name().Get() ,
											 pHitBoxSet->m_HitBoxes()[HitBoxIndex].m_sBoneName().Get() );
								}

								DumpHitBox = false;
							}
						}
					}*/

					/*static bool DumpBones = true;

					if ( DumpBones )
					{
						if ( auto pLocalCitadelPlayerPawn = GetCL_CitadelPlayerPawn()->GetLocal(); pLocalCitadelPlayerPawn )
						{
							auto& hModel = pLocalCitadelPlayerPawn->m_pGameSceneNode()->GetSkeletonInstance()->m_modelState().m_hModel();

							if ( hModel.is_valid() )
							{
								for ( auto BoneIndex = 0u; BoneIndex < hModel->m_nBoneCount; BoneIndex++ )
								{
									DEV_LOG( "%i -> %s\n" , BoneIndex , hModel->m_szBoneNames[BoneIndex] );
								}

								DumpBones = false;
							}
						}
					}*/

					const auto vOrigin = pC_CitadelPlayerPawn->m_vOldOrigin();
					Vector3 vHeadPos;
					
					if ( pC_CitadelPlayerPawn->m_pGameSceneNode()->GetBonePosition( pC_CitadelPlayerPawn->GetBoneIdByName( XorStr( "head" ) ) , vHeadPos ) )
					{
						if ( !vOrigin.IsZero() && !vHeadPos.IsZero() )
						{
							ImVec2 OriginScreen , HeadScreen;

							if ( Math::WorldToScreen( vOrigin , OriginScreen ) && Math::WorldToScreen( vHeadPos , HeadScreen ) )
							{
								const auto BoxHeight = floor( OriginScreen.y - HeadScreen.y );
								const auto BoxWidth = floor( BoxHeight / 2.f );

								Rect_t Rect;

								Rect.x = floor( HeadScreen.x - BoxWidth / 2.f );
								Rect.y = floor( HeadScreen.y );
								Rect.w = floor( HeadScreen.x + BoxWidth / 2.f );
								Rect.h = floor( OriginScreen.y );

								OnRenderPlayerEsp( pCCitadelPlayerController , Rect );
							}
						}
					}
				}
			}
			break;
			case CachedEntity_t::NPC_TROOPER:
			{
				auto* pC_NPC_Trooper = reinterpret_cast<C_NPC_Trooper*>( pEntity );

				if ( pC_NPC_Trooper->m_NPCState() == NPC_STATE_INIT ||
					pC_NPC_Trooper->m_NPCState() == NPC_STATE_IDLE ||
					pC_NPC_Trooper->m_NPCState() == NPC_STATE_ALERT ||
					 pC_NPC_Trooper->m_NPCState() == NPC_STATE_COMBAT )
				{
					Vector3 root_motion;

					if ( pC_NPC_Trooper->m_pGameSceneNode()->GetBonePosition( pC_NPC_Trooper->GetBoneIdByName( XorStr( "root_motion" ) ) , root_motion ) )
					{
						root_motion.m_z += 30.f;

						ImVec2 Screen;

						if ( Math::WorldToScreen( root_motion , Screen ) )
						{
							GetRenderStackSystem()->DrawString( &GetFontManager()->m_VerdanaFont , static_cast<int>( Screen.x ) , static_cast<int>( Screen.y ) , FW1_CENTER , ImColor( 255 , 255 , 255 ) , "X" );
						}
					}

					if ( CachedEntity.m_bDraw )
					{
						const auto bBox = CachedEntity.m_Bbox;

						const ImVec2 min = { bBox.x, bBox.y };
						const ImVec2 max = { bBox.w, bBox.h };

						GetRenderStackSystem()->DrawBox( min , max , ImColor( 255 , 255 , 255 ) );
					}
				}
			}
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
	OnRender();

	FOR_EACH_ENTITY( idx )
	{
		auto pBaseEntity = SDK::Interfaces::GameEntitySystem()->GetBaseEntity( idx );

		if ( pBaseEntity && pBaseEntity->IsCitadelPlayerController() )
		{
			auto* pCCitadelPlayerController = reinterpret_cast<CCitadelPlayerController*>( pBaseEntity );

			if ( pCCitadelPlayerController == GetCL_CitadelPlayerController()->GetLocal() )
				continue;

			if ( !pCCitadelPlayerController->m_PlayerDataGlobal().m_bAlive() )
				continue;

			auto* pPawn = pCCitadelPlayerController->m_hHeroPawn().Get<C_CitadelPlayerPawn>();

			if ( !pPawn || !pPawn->IsCitadelPlayerPawn() )
				continue;
			
			OnRenderSkeleton( pPawn );
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

auto CVisual::OnRenderPlayerEsp( CCitadelPlayerController* pCCitadelPlayerController , const Rect_t& Box ) -> void
{
	if ( !pCCitadelPlayerController->m_PlayerDataGlobal().m_bAlive() )
		return;

	const ImVec2 min = { Box.x, Box.y };
	const ImVec2 max = { Box.w, Box.h };

	auto PlayerColor = ImColor( 255 , 255 , 255 );

	if ( pCCitadelPlayerController->m_iTeamNum() == TEAM_DIER )
	{
		PlayerColor = ImColor( 255 , 0 , 0 );
	}
	else if ( pCCitadelPlayerController->m_iTeamNum() == TEAM_RADIANT )
	{
		PlayerColor = ImColor( 0 , 0 , 255 );
	}

	GetRenderStackSystem()->DrawOutlineCoalBox( min , max , PlayerColor );
}

auto CVisual::OnRenderSkeleton( C_CitadelPlayerPawn* pC_CitadelPlayerPawn ) -> void
{
	Vector3 BonePosStart , BonePosEnd;

	for ( const auto& Bones : g_AllSkeletonPairBones )
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
												  ImColor( 255 , 255 , 255 ) ,
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
				auto pC_NPC_Trooper = reinterpret_cast<C_NPC_Trooper*>( pEntity );
				it.m_bDraw = pC_NPC_Trooper->GetBoundingBox( it.m_Bbox ) && !pC_NPC_Trooper->m_pGameSceneNode()->m_bDormant();
				break;
		}
	}
}

auto GetVisual() -> CVisual*
{
	return &g_CVisual;
}
