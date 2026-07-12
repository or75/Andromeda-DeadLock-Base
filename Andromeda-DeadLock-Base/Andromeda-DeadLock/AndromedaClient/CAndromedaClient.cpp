#include "CAndromedaClient.hpp"
#include "CAndromedaGUI.hpp"

#include "Fonts/CFontManager.hpp"

#include <DeadLock/SDK/SDK.hpp>
#include <DeadLock/SDK/Interface/IEngineToClient.hpp>
#include <DeadLock/SDK/Interface/CGameEntitySystem.hpp>
#include <DeadLock/SDK/Types/CEntityData.hpp>

#include <GameClient/CEntityCache/CEntityCache.hpp>

#include <AndromedaClient/Features/CVisual/CVisual.hpp>
#include <AndromedaClient/GUI/CAndromedaMenu.hpp>
#include <AndromedaClient/Settings/CSettingsJson.hpp>
#include <AndromedaClient/Settings/Settings.hpp>>
#include <AndromedaClient/Render/CRenderStackSystem.hpp>

static CAndromedaClient g_CAndromedaClient{};

auto CAndromedaClient::OnInit() -> void
{
	GetAndromedaMenu()->InitColors();
	GetAndromedaMenu()->SetConfigSelected( GetSettingsJson()->GetConfigLoadedIndex() );
}

auto CAndromedaClient::OnFireEventClientSide( IGameEvent* pGameEvent ) -> void
{

}

auto CAndromedaClient::OnAddEntity( CEntityInstance* pInst , CHandle handle ) -> void
{
	GetEntityCache()->OnAddEntity( pInst , handle );
}

auto CAndromedaClient::OnRemoveEntity( CEntityInstance* pInst , CHandle handle ) -> void
{
	GetEntityCache()->OnRemoveEntity( pInst , handle );
}

auto CAndromedaClient::OnStartSound( const Vector3& Pos , const int SourceEntityIndex , const char* szSoundName ) -> void
{
	GetVisual()->OnStartSound( Pos , SourceEntityIndex , szSoundName );
}

auto CAndromedaClient::OnClientOutput() -> void
{
	if ( SDK::Interfaces::EngineToClient()->IsInGame() )
		GetVisual()->OnClientOutput();
}

auto CAndromedaClient::OnRender() -> void
{
	if ( GetAndromedaGUI()->IsVisible() )
		GetAndromedaMenu()->OnRenderMenu();

	GetFontManager()->FirstInitFonts();
	GetFontManager()->m_VerdanaFont.DrawString( 1 , 1 , ImColor( 1.f , 1.f , 0.f ) , FW1_LEFT , XorStr( CHEAT_NAME ) );

	GetRenderStackSystem()->OnRenderStack();
}

auto CAndromedaClient::OnCreateMove( CCitadelInput* pCitadelInput , CUserCmd* pUserCmd ) -> void
{
	GetVisual()->OnCreateMove( pUserCmd );

	if ( Settings::Misc::UnlockMiniMap )
	{
		FOR_EACH_ENTITY( idx )
		{
			auto* pC_BaseEntity = SDK::Interfaces::GameEntitySystem()->GetBaseEntity( idx );

			if ( pC_BaseEntity && strcmp( pC_BaseEntity->GetSchemaClassBinding()->m_bindingName() , XorStr( "C_CitadelTeam" ) ) == 0 )
			{
				auto* pC_CitadelTeam = reinterpret_cast<C_CitadelTeam*>( pC_BaseEntity );

				if ( pC_CitadelTeam->m_vecFOWEntities().Count() )
				{
					for ( auto& FOWEntity : pC_CitadelTeam->m_vecFOWEntities() )
					{
						if ( !FOWEntity.m_bVisibleOnMap() )
							FOWEntity.m_bVisibleOnMap() = true;
					}
				}
			}
		}
	}
}

auto GetAndromedaClient() -> CAndromedaClient*
{
	return &g_CAndromedaClient;
}
