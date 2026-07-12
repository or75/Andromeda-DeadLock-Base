#include "CL_Bones.hpp"

#include <DeadLock/SDK/Types/CEntityData.hpp>

static CL_Bones g_CL_Bones{};

VecCheckBones_t g_AllTraceVisibleHeroCheckBones =
{
	"head",
	"pelvis",
	"spine_0",
	"arm_lower_R",
	"arm_lower_L",
	"leg_lower_R" ,
	"leg_lower_L" ,
};

VecCheckBones_t g_AllTraceVisibleTropperCheckBones =
{
	"spine_0",
	"arm_lower_R",
	"arm_lower_L",
	"leg_lower_R",
	"leg_lower_L",
};

VecCheckBones_t g_AllTraceVisibleTropperNeutralCheckBones =
{
	"neck_0",
	"chest",
	"eye_0",
	"arm_lower_R",
	"arm_lower_L",
};

std::vector<std::pair<std::string , std::string>> g_AllSkeletonHeroPairBones =
{
	{ "spine_0" , "spine_1" } ,
	{ "spine_1" , "spine_2" } ,
	{ "spine_2" , "spine_3" } ,
	{ "spine_3" , "head" } ,
	{ "spine_3" , "arm_upper_L" } ,
	{ "spine_3" , "arm_upper_R" } ,
	{ "arm_upper_R" , "arm_upper_R_TWIST" } ,
	{ "arm_upper_L" , "arm_upper_L_TWIST" } ,
	{ "arm_upper_L" , "arm_lower_L" } ,
	{ "arm_upper_R" , "arm_lower_R" } ,
	{ "leg_upper_L" , "leg_lower_L" } ,
	{ "leg_upper_R" , "leg_lower_R" } ,
	{ "arm_upper_R_TWIST" , "arm_upper_R_TWIST1" } ,
	{ "arm_upper_L_TWIST" , "arm_upper_L_TWIST1" } ,
	{ "arm_upper_R_TWIST1" , "arm_lower_R" } ,
	{ "arm_upper_L_TWIST1" , "arm_lower_L" } ,
	{ "arm_lower_R" , "arm_lower_R_TWIST" } ,
	{ "arm_lower_R_TWIST" , "arm_lower_R_TWIST1" } ,
	{ "arm_lower_L" , "arm_lower_L_TWIST" } ,
	{ "arm_lower_L_TWIST" , "arm_lower_L_TWIST1" } ,
	{ "spine_0" , "leg_upper_R" } ,
	{ "spine_0" , "leg_upper_L" } ,
	{ "leg_upper_R" , "leg_upper_R_TWIST" } ,
	{ "leg_upper_L" , "leg_upper_L_TWIST" } ,
	{ "leg_upper_R_TWIST" , "leg_upper_R_TWIST1" } ,
	{ "leg_upper_L_TWIST" , "leg_upper_L_TWIST1" } ,
	{ "leg_upper_R_TWIST1" , "leg_lower_R" } ,
	{ "leg_upper_L_TWIST1" , "leg_lower_L" } ,
	{ "leg_lower_R" , "leg_R_IKTARGET" } ,
	{ "leg_lower_L" , "leg_L_IKTARGET" } ,
};

std::vector<std::pair<std::string , std::string>> g_AllSkeletonTrooperPairBones =
{
	{ "pelvis" , "spine_1" } ,
	{ "pelvis" , "leg_upper_R" } ,
	{ "pelvis" , "leg_upper_L" } ,
	{ "spine_1" , "head_end" } ,
	{ "spine_1" , "clavicle_R" } ,
	{ "spine_1" , "clavicle_L" } ,
	{ "clavicle_R" , "arm_upper_R" } ,
	{ "clavicle_L" , "arm_upper_L" } ,
	{ "arm_upper_R" , "arm_lower_R" } ,
	{ "arm_upper_L" , "arm_lower_L" } ,
	{ "arm_lower_R" , "hand_R" } ,
	{ "arm_lower_L" , "hand_L" } ,
	{ "leg_upper_L" , "leg_lower_L" } ,
	{ "leg_upper_R" , "leg_lower_R" } ,
	{ "leg_lower_R" , "leg_R_IKTARGET" } ,
	{ "leg_lower_L" , "leg_L_IKTARGET" } ,
};

std::vector<std::pair<std::string , std::string>> g_AllSkeletonTrooperNeutralPairBones =
{
	{ "neck_0" , "arm_upper_L" } ,
	{ "arm_upper_L" , "arm_lower_L" } ,
	{ "arm_lower_L" , "L_Hand_IK" } ,
	{ "neck_0" , "chest" } ,
	{ "chest" , "tail_1" } ,
	{ "tail_1" , "tail_2" } ,
	{ "tail_2" , "tail_3" } ,
	{ "tail_3" , "tail_4" } ,
	{ "neck_0" , "arm_upper_R" } ,
	{ "arm_upper_R" , "arm_lower_R" } ,
	{ "arm_lower_R" , "R_Hand_IK" } ,
	{ "neck_0" , "eye_0" } ,
};

auto CL_Bones::GetBonePositionByName( C_BaseEntity* pC_BaseEntity , const char* szBoneName ) -> Vector3
{
	Vector3 BonePosition;

	if ( auto* pSkeletonInstance = pC_BaseEntity->m_pGameSceneNode()->GetSkeletonInstance(); pSkeletonInstance )
	{
		pSkeletonInstance->CalcWorldSpaceBones( FLAG_ALL_BONE_FLAGS );

		const auto& Model = pSkeletonInstance->m_modelState().m_hModel();

		if ( !Model.is_valid() )
			return BonePosition;

		const auto BoneIndex = pC_BaseEntity->GetBoneIdByName( szBoneName );

		if ( BoneIndex != -1 && pSkeletonInstance->GetBonePosition( BoneIndex , BonePosition ) )
			return BonePosition;
		//else
		//	DEV_LOG( "[error] GetBoneIdByName: %s\n" , szBoneName );
	}

	return BonePosition;
}

auto GetCL_Bones() -> CL_Bones*
{
	return &g_CL_Bones;
}
