#include "CAndromedaClient.hpp"
#include "CAndromedaGUI.hpp"

#include "Fonts/CFontManager.hpp"

#include <DeadLock/SDK/SDK.hpp>
#include <DeadLock/SDK/Interface/IEngineToClient.hpp>

#include <GameClient/CEntityCache/CEntityCache.hpp>

#include <AndromedaClient/Features/CVisual/CVisual.hpp>

#include <AndromedaClient/GUI/CAndromedaMenu.hpp>
#include <AndromedaClient/Settings/CSettingsJson.hpp>
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

}

auto GetAndromedaClient() -> CAndromedaClient*
{
	return &g_CAndromedaClient;
}
