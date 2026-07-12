#include "CL_Trace.hpp"

#include <Common/Include/Fnv1a/Hash_Fnv1a_Constexpr.hpp>

#include <DeadLock/SDK/SDK.hpp>
#include <DeadLock/SDK/Math/Math.hpp>
#include <DeadLock/SDK/Types/CEntityData.hpp>
#include <DeadLock/SDK/Update/GameTrace.hpp>
#include <DeadLock/SDK/Update/CUserCmd.hpp>
#include <DeadLock/SDK/FunctionListSDK.hpp>

#include <GameClient/CL_CitadelPlayerPawn.hpp>

static CL_Trace g_CL_Trace{};

auto CL_Trace::TraceToBoneEntity( CUserCmd* CUserCmd ) -> std::pair<uint64_t , C_BaseEntity*>
{
	if ( auto pLocalPlayerPawn = GetCL_CitadelPlayerPawn()->GetLocal(); pLocalPlayerPawn )
	{
		Vector3 vForward;
		Ray_t Ray;
		CGameTrace GameTrace;
		CTraceFilter Filter( 0x1C1003 , pLocalPlayerPawn , 3 , 15 );

		auto vStart = Vector3( CUserCmd->cmd.vec_camera_position().x() ,
							   CUserCmd->cmd.vec_camera_position().y() ,
							   CUserCmd->cmd.vec_camera_position().z() );

		auto ViewAngles = QAngle( CUserCmd->cmd.base().viewangles().x() ,
								  CUserCmd->cmd.base().viewangles().y() ,
								  CUserCmd->cmd.base().viewangles().z() );

		Math::AngleVectors( ViewAngles , vForward );

		vForward *= 8192.f;

		auto vEnd = vStart + vForward;

		if ( CVPhys2World_TraceShape( SDK::Pointers::GetCVPhys2World() , &Ray , &vStart , &vEnd , &Filter , &GameTrace ) )
		{
			/*DEV_LOG( "Trace: %s , %s\n" , GameTrace.pHitBox->m_sBoneName().Get(), 
					 GameTrace.pHitEntity->GetSchemaClassBinding()->m_bindingName );*/

			return { hash_64_fnv1a_const( GameTrace.pHitBox->m_sBoneName().Get() ) , GameTrace.pHitEntity };
		}
	}

	return { 0 , nullptr };
}

auto CL_Trace::TraceToEntityEndPos( const Vector3& CameraPos , Vector3* vEnd ) -> C_BaseEntity*
{
	if ( auto pLocalPlayerPawn = GetCL_CitadelPlayerPawn()->GetLocal(); pLocalPlayerPawn )
	{
		Ray_t Ray;
		CGameTrace GameTrace;
		CTraceFilter Filter( 0x1C1003 , pLocalPlayerPawn , 3 , 15 );
		
		auto vStart = CameraPos;

		if ( CVPhys2World_TraceShape( SDK::Pointers::GetCVPhys2World() , &Ray , &vStart , vEnd , &Filter , &GameTrace ) )
		{
			if ( GameTrace.pHitBox && GameTrace.pHitEntity && GameTrace.pHitBox->m_sBoneName().Get() &&
				 hash_64_fnv1a_const( GameTrace.pHitBox->m_sBoneName().Get() ) != hash_64_fnv1a_const( "invalid_bone" ) )
			{
				/*DEV_LOG( "%i , %s , %s\n" , GameTrace.IsVisible() ,
						 GameTrace.pHitEntity->GetSchemaClassBinding()->m_bindingName ,
						 GameTrace.pHitBox->m_sBoneName().Get() );*/

				return GameTrace.pHitEntity;
			}
		}
	}

	return nullptr;
}

auto CL_Trace::PathCheck( const Vector3& CameraPos , Vector3& vTargetPos ) -> bool
{
	if ( auto pLocalPlayerPawn = GetCL_CitadelPlayerPawn()->GetLocal(); pLocalPlayerPawn )
	{
		Ray_t Ray;
		CGameTrace GameTrace = {};
		CTraceFilter Filter( 0x1C1003 , pLocalPlayerPawn , 3 , 15 );

		auto vStart = CameraPos;
		auto vEnd = vTargetPos;

		if ( !CVPhys2World_TraceShape( SDK::Pointers::GetCVPhys2World() , &Ray , &vStart , &vEnd , &Filter , &GameTrace ) )
		{
			return true;
		}

		if ( GameTrace.flFraction >= 0.999f )
		{
			return true;
		}

		if ( GameTrace.pHitEntity )
		{
			const char* hitEntityName = "unknown";

			if ( GameTrace.pHitEntity->GetSchemaClassBinding() )
				hitEntityName = GameTrace.pHitEntity->GetSchemaClassBinding()->m_bindingName();

			if ( GameTrace.pHitEntity->IsCitadelPlayerPawn() )
				return false;

			if ( strcmp( hitEntityName , "C_World" ) == 0 )
				return false;

			return true;
		}

		if ( GameTrace.flFraction < 0.999f )
		{
			return false;
		}

		return true;
	}

	return false;
}

auto GetCL_Trace() -> CL_Trace*
{
	return &g_CL_Trace;
}
