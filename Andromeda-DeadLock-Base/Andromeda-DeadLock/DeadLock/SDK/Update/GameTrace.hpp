#pragma once

#include <DeadLock/SDK/Types/CEntityData.hpp>
#include <DeadLock/SDK/FunctionListSDK.hpp>

struct Ray_t
{
	Vector3 Start;
	Vector3 End;
	Vector3 Mins;
	Vector3 Maxs;
private:
	PAD( 0x4 );
public:
	uint8_t UnkType;
};

class CGameTrace
{
public:
	void* pSurfaceProperties; // 0x00
	C_BaseEntity* pHitEntity; // 0x08
	CHitBox* pHitBox; // 0x10
private:
	PAD( 0x38 ); // 0x18
public:
	uint32_t nSurfaceFlags; // 0x50
private:
	PAD( 0x24 ); // 0x54
public:
	Vector3 vecStart; // 0x78 // initial position
	Vector3 vecEnd; // 0x84 // final position
	Vector3 vecNormal; // 0x90 // surface normal at impact
	Vector3 vecPosition;
private:
	PAD( 0x4 );
public:
	float flFraction; // 0xAC // time completed, 1.0 = didn't hit anything
private:
	PAD( 0x6 ); // 0xB0
public:
	uint8_t nShapeType; // 0xB6
	bool bStartSolid; // 0xB7 // if true, the initial point was in a solid area
private:
	PAD( 0x9 );
public:

	inline bool DidHit() const
	{
		return ( flFraction < 1.0f || bStartSolid );
	}

	inline bool IsVisible() const
	{
		return ( flFraction > 0.97f );
	}
};

class CTraceFilter
{
public:
	CTraceFilter( std::uint64_t uMask , C_CitadelPlayerPawn* pPassEntity , int nLayer , uint16_t unkNum )
	{
		CTraceFilter_Constructor( this , uMask , pPassEntity , nLayer , unkNum );
	}

public:
	virtual ~CTraceFilter() {}
	virtual bool ShouldHitEntity( CEntityInstance* pEntity ) { return true; }

private:
	PAD( 0x100 );
};
