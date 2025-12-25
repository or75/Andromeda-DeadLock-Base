#include "Hook_CreateMove.hpp"

#include <DeadLock/SDK/Update/CCitadelInput.hpp>

#include <GameClient/CL_CitadelPlayerController.hpp>

#include <AndromedaClient/CAndromedaClient.hpp>

auto Hook_CreateMove( CCitadelInput* pCitadelInput , uint32_t split_screen_index , char a3 ) -> void
{
	CreateMove_o( pCitadelInput , split_screen_index , a3 );

	if ( auto* pLocalCitadelPlayerController = GetCL_CitadelPlayerController()->GetLocal(); pLocalCitadelPlayerController )
	{
		if ( auto* pUserCmd = pCitadelInput->GetUserCmd( pLocalCitadelPlayerController ); pUserCmd )
		{
			GetAndromedaClient()->OnCreateMove( pCitadelInput , pUserCmd );
		}
	}
}
