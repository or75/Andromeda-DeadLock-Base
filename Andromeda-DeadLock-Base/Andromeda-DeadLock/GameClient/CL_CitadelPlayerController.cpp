#include "CL_CitadelPlayerController.hpp"

#include <DeadLock/SDK/Interface/CGameEntitySystem.hpp>

static CL_CitadelPlayerController g_CL_CitadelPlayerController{};

auto CL_CitadelPlayerController::GetLocal() -> CCitadelPlayerController*
{
	return CGameEntitySystem::GetLocalCitadelPlayerController();
}

auto GetCL_CitadelPlayerController() -> CL_CitadelPlayerController*
{
	return &g_CL_CitadelPlayerController;
}
