#pragma once

#include <Common/Common.hpp>
#include <vector>

#include <DeadLock/SDK/Math/Vector3.hpp>

class C_CitadelPlayerPawn;

extern std::vector<std::pair<std::string , std::string>> g_AllSkeletonPairBones;

class CL_Bones final
{
public:
	auto GetBonePositionByName( C_CitadelPlayerPawn* pC_CitadelPlayerPawn , const char* szBoneName ) -> Vector3;
};

auto GetCL_Bones() -> CL_Bones*;
