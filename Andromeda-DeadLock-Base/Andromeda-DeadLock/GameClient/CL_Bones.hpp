#pragma once

#include <Common/Common.hpp>
#include <vector>

#include <DeadLock/SDK/Math/Vector3.hpp>

class C_BaseEntity;

using VecCheckBones_t = std::vector<std::string_view>;

extern VecCheckBones_t g_AllTraceVisibleHeroCheckBones;
extern VecCheckBones_t g_AllTraceVisibleTropperCheckBones;
extern VecCheckBones_t g_AllTraceVisibleTropperNeutralCheckBones;

extern std::vector<std::pair<std::string , std::string>> g_AllSkeletonHeroPairBones;
extern std::vector<std::pair<std::string , std::string>> g_AllSkeletonTrooperPairBones;
extern std::vector<std::pair<std::string , std::string>> g_AllSkeletonTrooperNeutralPairBones;

class CL_Bones final
{
public:
	// dont use CCitadelPlayerController
	auto GetBonePositionByName( C_BaseEntity* pC_BaseEntity , const char* szBoneName ) -> Vector3;
};

auto GetCL_Bones() -> CL_Bones*;
