#pragma once

#include <Common/Common.hpp>

#include "Color.hpp"

#include "CBaseTypes.hpp"
#include "CHandle.hpp"

#include "CUtlMemory.hpp"
#include "CUtlString.hpp"
#include "CUtlSymbol.hpp"
#include "CUtlSymbolLarge.hpp"
#include "CUtlVector.hpp"
#include "CStrongHandle.hpp"
#include "CUtlStringToken.hpp"

#include <DeadLock/SDK/Math/Rect_t.hpp>
#include <DeadLock/SDK/CSchemaOffset.hpp>
#include <DeadLock/SDK/Interface/CShemaSystemSDK.hpp>

class CSkeletonInstance;

struct alignas( 16 ) CBoneData
{
	Vector3 position;
	float scale;
	Vector3 rotation;
};

class CModel
{
private:
	PAD( 0x130 + 0x38 );

public:
	const char** m_szBoneNames;
	uint32 m_nBoneCount;
};

class CHitBox
{
private:
	PAD( 0x70 );

public:
	SCHEMA_OFFSET( "CHitBox" , "m_name" , m_name , CUtlString );
	SCHEMA_OFFSET( "CHitBox" , "m_sBoneName" , m_sBoneName , CUtlString );
};

class CHitBoxSet
{
public:
	SCHEMA_OFFSET( "CHitBoxSet" , "m_HitBoxes" , m_HitBoxes , CUtlVector< CHitBox > );
};

class CModelState
{
private:
	PAD( 0x80 );

public:
	CBoneData* m_pBones;

public:
	SCHEMA_OFFSET( "CModelState" , "m_hModel" , m_hModel , CStrongHandle<CModel> );
	SCHEMA_OFFSET( "CModelState" , "m_ModelName" , m_ModelName , CUtlSymbolLarge );
};

class PlayerDataGlobal_t
{
public:
	SCHEMA_OFFSET( "PlayerDataGlobal_t" , "m_iLevel" , m_iLevel , int32 );
	SCHEMA_OFFSET( "PlayerDataGlobal_t" , "m_iHealthMax" , m_iHealthMax , int32 );
	SCHEMA_OFFSET( "PlayerDataGlobal_t" , "m_flHealthRegen" , m_flHealthRegen , float32 );
	SCHEMA_OFFSET( "PlayerDataGlobal_t" , "m_nHeroID" , m_nHeroID , HeroID_t );
	SCHEMA_OFFSET( "PlayerDataGlobal_t" , "m_iHealth" , m_iHealth , int32 );
	SCHEMA_OFFSET( "PlayerDataGlobal_t" , "m_bAlive" , m_bAlive , bool );
};

class CCollisionProperty
{
public:
	SCHEMA_OFFSET( "CCollisionProperty" , "m_vecMins" , m_vecMins , Vector3 );
	SCHEMA_OFFSET( "CCollisionProperty" , "m_vecMaxs" , m_vecMaxs , Vector3 );
};

class CPlayerPawnComponent
{
public:
};

class IHandleEntity
{
public:
	virtual ~IHandleEntity() {}
};

class CEntityIdentity 
{
public:
	SCHEMA_OFFSET_CUSTOM( pBaseEntity , 0x0 , C_BaseEntity* );
	SCHEMA_OFFSET_CUSTOM( Handle , 0x10 , CHandle );

public:
	SCHEMA_OFFSET( "CEntityIdentity" , "m_name" , Name , CUtlSymbolLarge );
	SCHEMA_OFFSET( "CEntityIdentity" , "m_designerName" , DesingerName , CUtlSymbolLarge );
};

class CEntityInstance : public IHandleEntity
{
public:
	auto GetSchemaClassBinding() -> CSchemaClassBinding*
	{
		CSchemaClassBinding* pBinding = nullptr;

		VirtualFn( void )( CEntityInstance* , CSchemaClassBinding** );
		vget< Fn >( this , index::CSchemaSystem::SchemaClassInfo )( this , &pBinding );

		return pBinding;
	}

public:
	SCHEMA_OFFSET( "CEntityInstance" , "m_pEntity" , pEntityIdentity , CEntityIdentity* );
};

class CGameSceneNode
{
public:
	SCHEMA_OFFSET( "CGameSceneNode" , "m_bDormant" , m_bDormant , bool );
	SCHEMA_OFFSET( "CGameSceneNode" , "m_vecAbsOrigin" , m_vecAbsOrigin , Vector3 );

public:
	auto GetBonePosition( int32 BoneIndex , Vector3& BonePos ) -> bool;

public:
	auto GetSkeletonInstance() -> CSkeletonInstance*
	{
		return reinterpret_cast<CSkeletonInstance*>( this );
	}
};

class CSkeletonInstance : public CGameSceneNode
{
public:
	SCHEMA_OFFSET( "CSkeletonInstance" , "m_modelState" , m_modelState , CModelState );

public:
	auto CalcWorldSpaceBones( unsigned int Mask ) -> void;
};

class C_BaseEntity : public CEntityInstance
{
public:
	auto IsCitadelPlayerController() -> bool;
	auto IsCitadelPlayerPawn() -> bool;
	auto IsNpcTrooper() -> bool;

public:
	auto GetOrigin() -> const Vector3&;

public:
	SCHEMA_OFFSET( "C_BaseEntity" , "m_pGameSceneNode" , m_pGameSceneNode , CGameSceneNode* );
	SCHEMA_OFFSET( "C_BaseEntity" , "m_iTeamNum" , m_iTeamNum , uint8 );
	SCHEMA_OFFSET( "C_BaseEntity" , "m_fFlags" , m_fFlags , uint32 );
	SCHEMA_OFFSET( "C_BaseEntity" , "m_MoveType" , m_MoveType , MoveType_t );

public:
	auto GetBoneIdByName( const char* szName ) -> int;
	auto GetHitBoxSet() -> CHitBoxSet*;
};

class C_BaseModelEntity : public C_BaseEntity
{
public:
	auto GetBoundingBox( Rect_t& out ) -> bool;

public:
	SCHEMA_OFFSET( "C_BaseModelEntity" , "m_Collision" , m_Collision , CCollisionProperty );
};

class CBaseAnimGraph : public C_BaseModelEntity
{
public:
};

class C_BaseFlex : public CBaseAnimGraph
{
public:
};

class C_BaseCombatCharacter : public C_BaseFlex
{
public:
};

class C_BaseToggle : public C_BaseModelEntity
{
public:
};

class C_BaseTrigger : public C_BaseToggle
{
public:
};

class C_PostProcessingVolume : public C_BaseTrigger
{
public:
	SCHEMA_OFFSET( "C_PostProcessingVolume" , "m_flMinExposure" , m_flMinExposure , float32 );
	SCHEMA_OFFSET( "C_PostProcessingVolume" , "m_flMaxExposure" , m_flMaxExposure , float32 );
	SCHEMA_OFFSET( "C_PostProcessingVolume" , "m_bExposureControl" , m_bExposureControl , bool );
};

class CPlayer_CameraServices : public CPlayerPawnComponent
{
public:
	SCHEMA_OFFSET( "CPlayer_CameraServices" , "m_hActivePostProcessingVolume" , m_hActivePostProcessingVolume , CHandle ); // C_PostProcessingVolume
};

class C_BasePlayerPawn : public C_BaseCombatCharacter
{
public:
	SCHEMA_OFFSET( "C_BasePlayerPawn" , "m_pCameraServices" , m_pCameraServices , CPlayer_CameraServices* );
	SCHEMA_OFFSET( "C_BasePlayerPawn" , "m_vOldOrigin" , m_vOldOrigin , Vector3 );
};

class CCitadelPlayerPawnBase : public C_BasePlayerPawn
{
public:
};

class C_CitadelPlayerPawn : public CCitadelPlayerPawnBase
{
public:
};

class CBasePlayerController : public C_BaseEntity
{
public:
	SCHEMA_OFFSET( "CBasePlayerController" , "m_hPawn" , m_hPawn , CHandle ); // C_BasePlayerPawn
};

class CCitadelPlayerController : public CBasePlayerController
{
public:
	SCHEMA_OFFSET( "CCitadelPlayerController" , "m_hHeroPawn" , m_hHeroPawn , CHandle ); // C_CitadelPlayerPawn
	SCHEMA_OFFSET( "CCitadelPlayerController" , "m_PlayerDataGlobal" , m_PlayerDataGlobal , PlayerDataGlobal_t );
};

class C_AI_BaseNPC : public C_BaseCombatCharacter
{
public:
	SCHEMA_OFFSET( "C_AI_BaseNPC" , "m_NPCState" , m_NPCState , NPC_STATE );
};

class C_AI_CitadelNPC : public C_AI_BaseNPC
{
public:

};

class C_NPC_Trooper : public C_AI_CitadelNPC
{
public:
};

class C_EnvSky : public C_BaseModelEntity
{
public:
	SCHEMA_OFFSET( "C_EnvSky" , "m_vTintColor" , m_vTintColor , Color );
	SCHEMA_OFFSET( "C_EnvSky" , "m_vTintColorLightingOnly" , m_vTintColorLightingOnly , Color );
};
