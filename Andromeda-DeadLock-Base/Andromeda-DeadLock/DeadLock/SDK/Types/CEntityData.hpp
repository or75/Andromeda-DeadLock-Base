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
#include <DeadLock/SDK/Update/Offsets.hpp>

class CSkeletonInstance;

struct alignas( 16 ) CBoneData
{
	Vector3 position;
	float scale;
	Vector3 rotation;
};

class CNetworkVelocityVector
{
public:
	SCHEMA_OFFSET( "CNetworkVelocityVector" , "m_vecX" , m_vecX , Vector2 );
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

class CGlowProperty
{
public:
	SCHEMA_OFFSET( "CGlowProperty" , "m_glowColorOverride" , m_glowColorOverride , Color );
	SCHEMA_OFFSET( "CGlowProperty" , "m_iGlowType" , m_iGlowType , int32 );
	SCHEMA_OFFSET( "CGlowProperty" , "m_iGlowTeam" , m_iGlowTeam , int32 );
	SCHEMA_OFFSET( "CGlowProperty" , "m_bGlowing" , m_bGlowing , bool );
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
	inline auto GetUnknownMask() -> uint16 { return CUSTOM_OFFSET( uint16 , g_CCollisionProperty_UnknownMask ); }

public:
	SCHEMA_OFFSET( "CCollisionProperty" , "m_vecMins" , m_vecMins , Vector3 );
	SCHEMA_OFFSET( "CCollisionProperty" , "m_vecMaxs" , m_vecMaxs , Vector3 );
	SCHEMA_OFFSET( "CCollisionProperty" , "m_usSolidFlags" , m_usSolidFlags , uint8 );
};

class CPanoramaImageName
{
private:
	PAD( 0x8 );
public:
	const char* szImagePath;
};

class CEntityComponent
{
public:
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
	auto IsNpcTrooperNeutral() -> bool;
	auto IsItemXP() -> bool;
	auto IsWorldItemPanel() -> bool;
	auto IsCitadelObserverPawn() -> bool;

public:
	auto GetOrigin() -> const Vector3&;

public:
	SCHEMA_OFFSET( "C_BaseEntity" , "m_pGameSceneNode" , m_pGameSceneNode , CGameSceneNode* );
	SCHEMA_OFFSET( "C_BaseEntity" , "m_iTeamNum" , m_iTeamNum , uint8 );
	SCHEMA_OFFSET( "C_BaseEntity" , "m_fFlags" , m_fFlags , uint32 );
	SCHEMA_OFFSET( "C_BaseEntity" , "m_vecVelocity" , m_vecVelocity , CNetworkVelocityVector );
	SCHEMA_OFFSET( "C_BaseEntity" , "m_MoveType" , m_MoveType , MoveType_t );
	SCHEMA_OFFSET( "C_BaseEntity" , "m_hOwnerEntity" , m_hOwnerEntity , CHandle );

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
	SCHEMA_OFFSET( "C_BaseModelEntity" , "m_Glow" , m_Glow , CGlowProperty );
	SCHEMA_OFFSET( "C_BaseModelEntity" , "m_vecViewOffset" , m_vecViewOffset , CNetworkViewOffsetVector );
};

class C_CitadelBaseAbility : public C_BaseEntity
{
public:
};

class CItemXP : public C_BaseModelEntity
{
public:
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

class CPlayer_CameraServices : public CPlayerPawnComponent
{
public:
	SCHEMA_OFFSET( "CPlayer_CameraServices" , "m_vecPunchAngle" , m_vecPunchAngle , QAngle );
	SCHEMA_OFFSET( "CPlayer_CameraServices" , "m_hActivePostProcessingVolume" , m_hActivePostProcessingVolume , CHandle ); // C_PostProcessingVolume
};

class CPlayer_ObserverServices : public CPlayerPawnComponent
{
public:
	SCHEMA_OFFSET( "CPlayer_ObserverServices" , "m_hObserverTarget" , m_hObserverTarget , CHandle );
};

class C_BasePlayerPawn : public C_BaseCombatCharacter
{
public:
	SCHEMA_OFFSET( "C_BasePlayerPawn" , "m_pCameraServices" , m_pCameraServices , CPlayer_CameraServices* );
	SCHEMA_OFFSET( "C_BasePlayerPawn" , "m_pObserverServices" , m_pObserverServices , CPlayer_ObserverServices* );
	SCHEMA_OFFSET( "C_BasePlayerPawn" , "m_vOldOrigin" , m_vOldOrigin , Vector3 );
	SCHEMA_OFFSET( "C_BasePlayerPawn" , "m_hController" , m_hController , CHandle ); // CCitadelPlayerController
};

class CCitadelPlayerPawnBase : public C_BasePlayerPawn
{
public:
};

class C_CitadelPlayerPawn : public CCitadelPlayerPawnBase
{
public:
	SCHEMA_OFFSET( "C_CitadelPlayerPawn" , "m_angEyeAngles" , m_angEyeAngles , QAngle );

public:
	inline auto GetCollisionMask() -> uint16
	{
		return m_Collision().GetUnknownMask();
	}

public:
	inline auto GetOwnerHandle() -> uint32
	{
		uint32 Result = INVALID_EHANDLE_INDEX;

		if ( !( m_Collision().m_usSolidFlags() & 4 ) )
		{
			auto pC_BaseEntity = m_hOwnerEntity().Get();

			if ( pC_BaseEntity )
				Result = pC_BaseEntity->pEntityIdentity()->Handle().GetEntryIndex();
		}

		return Result;
	}

public:
	inline auto GetEyeOrigin() -> Vector3
	{
		return GetOrigin() + m_vecViewOffset();
	}
};

class CBasePlayerController : public C_BaseEntity
{
public:
	SCHEMA_OFFSET( "CBasePlayerController" , "m_hPawn" , m_hPawn , CHandle ); // C_BasePlayerPawn
	PSCHEMA_OFFSET( "CBasePlayerController" , "m_iszPlayerName" , m_iszPlayerName , const char );
};

class CCitadelPlayerController : public CBasePlayerController
{
public:
	SCHEMA_OFFSET( "CCitadelPlayerController" , "m_hHeroPawn" , m_hHeroPawn , CHandle ); // C_CitadelPlayerPawn
	SCHEMA_OFFSET( "CCitadelPlayerController" , "m_PlayerDataGlobal" , m_PlayerDataGlobal , PlayerDataGlobal_t );

	inline auto IsAlive() -> bool
	{
		return m_PlayerDataGlobal().m_bAlive();
	}
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

class C_NPC_TrooperNeutral : public C_AI_CitadelNPC
{
public:
};

class C_Citadel_Pickup : public CBaseAnimGraph
{
public:
	SCHEMA_OFFSET( "C_Citadel_Pickup" , "m_bActive" , m_bActive , bool );
};

class C_Citadel_Pickup_Modifier : public C_Citadel_Pickup
{
public:
};

class C_BaseClientUIEntity : public C_BaseModelEntity
{
public:
};

class C_PointClientUIWorldPanel : public C_BaseClientUIEntity
{
public:
};

struct ID3D11ShaderResourceView;

class CTextureDx11
{
public:
	CUSTOM_OFFSET_FIELD( ID3D11ShaderResourceView* , m_pTextureSRV0 , 0x10 );
};

class ppCTextureDx11
{
public:
	class pCTextureDx11
	{
	public:
		CUSTOM_OFFSET_FIELD( CTextureDx11* , m_pDx11Texture , 0x0 );
	};

	CUSTOM_OFFSET_FIELD( pCTextureDx11* , m_ppCTextureDx11 , 0x0 );
};

class CInWorldItemPanel : public C_PointClientUIWorldPanel
{
public:
	SCHEMA_OFFSET( "CInWorldItemPanel" , "m_hTrackedEntity" , m_hTrackedEntity , CHandle );
	SCHEMA_OFFSET_CUSTOM( TextImageTextureDX11 , 0xA80 , ppCTextureDx11 );
};

class C_CitadelObserverPawn : public CCitadelPlayerPawnBase
{
public:
};

class C_Team : public C_BaseEntity
{
public:
};

class STeamFOWEntity
{
private:
	PAD( 0x60 );
public:
	SCHEMA_OFFSET( "STeamFOWEntity" , "m_bVisibleOnMap" , m_bVisibleOnMap , bool );
};

class C_CitadelTeam : public C_Team
{
public:
	SCHEMA_OFFSET( "C_CitadelTeam" , "m_vecFOWEntities" , m_vecFOWEntities , CUtlVector< STeamFOWEntity > );
};
