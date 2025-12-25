#include "CL_CitadelPlayerPawn.hpp"
#include "CL_CitadelPlayerController.hpp"

#include <DeadLock/SDK/Types/CEntityData.hpp>

static CL_CitadelPlayerPawn g_CL_CitadelPlayerPawn{};

auto CL_CitadelPlayerPawn::GetLocal() -> C_CitadelPlayerPawn*
{
	if ( auto* pLocalCitadelPlayerController = GetCL_CitadelPlayerController()->GetLocal(); pLocalCitadelPlayerController )
	{
		auto* pLocalCitadelPlayerPawn = pLocalCitadelPlayerController->m_hHeroPawn().Get<C_CitadelPlayerPawn>();

		if ( pLocalCitadelPlayerPawn && pLocalCitadelPlayerPawn->IsCitadelPlayerPawn() )
			return pLocalCitadelPlayerPawn;
	}

	return nullptr;
}

auto GetCL_CitadelPlayerPawn() -> CL_CitadelPlayerPawn*
{
	return &g_CL_CitadelPlayerPawn;
}
