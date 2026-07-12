#pragma once

#include <Common/Common.hpp>
#include <DeadLock/SDK/Math/Vector3.hpp>

#include "CL_Bones.hpp"

class CCitadelPlayerController;
class C_CitadelPlayerPawn;
class C_NPC_Trooper;
class C_NPC_TrooperNeutral;

class CL_VisibleCheck final
{
public:
	auto IsPlayerControllerVisible( const Vector3& CameraPos , CCitadelPlayerController* pCCitadelPlayerController ) -> bool;
	auto IsPlayerPawnVisible( const Vector3& CameraPos , C_CitadelPlayerPawn* pC_CitadelPlayerPawn ) -> bool;
	auto IsTropperVisible( const Vector3& CameraPos , C_NPC_Trooper* pC_NPC_Trooper ) -> bool;
	auto IsTropperNeutralVisible( const Vector3& CameraPos , C_NPC_TrooperNeutral* pC_NPC_TrooperNeutral ) -> bool;

private:
	auto IsBaseEntityVisible( const Vector3& CameraPos , C_BaseEntity* pC_BaseEntity, const VecCheckBones_t& CheckBones ) -> bool;
};

auto GetCL_VisibleCheck() -> CL_VisibleCheck*;
