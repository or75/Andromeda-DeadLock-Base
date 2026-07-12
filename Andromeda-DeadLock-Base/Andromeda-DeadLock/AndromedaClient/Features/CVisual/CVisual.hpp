#pragma once

#include <vector>

#include <Common/Common.hpp>
#include <ImGui/imgui.h>

#include <DeadLock/SDK/Math/Vector3.hpp>
#include <DeadLock/SDK/Math/Rect_t.hpp>

class CCitadelPlayerController;
class C_CitadelPlayerPawn;
class CUserCmd;
class C_NPC_Trooper;
class C_NPC_TrooperNeutral;

struct SoundData_t
{
	ULONGLONG dwTime = 0;
	Vector3 Pos;
};

class IVisual
{
public:
	virtual void OnRender() = 0;
	virtual void OnStartSound( const Vector3& Pos , const int SourceEntityIndex , const char* szSoundName ) = 0;
	virtual void OnClientOutput() = 0;
	virtual void OnCreateMove( CUserCmd* pCUserCmd ) = 0;
};

class CVisual final : public IVisual
{
	using SoundListVecType_t = std::vector<SoundData_t>;
	using Lock_t = std::mutex;

	enum EVisualBoxType_t : uint32_t
	{
		BOX ,
		OUTLINE_BOX ,
		COAL_BOX ,
		OUTLINE_COAL_BOX ,
		CIRCLE_3D ,
	};

public:
	virtual void OnRender() override;
	virtual void OnStartSound( const Vector3& Pos , const int SourceEntityIndex , const char* szSoundName ) override;
	virtual void OnClientOutput() override;
	virtual void OnCreateMove( CUserCmd* pCUserCmd ) override;

private:
	auto OnRenderSound() -> void;
	auto OnRenderPlayerEsp( CCitadelPlayerController* pCCitadelPlayerController , const bool bVisible ) -> void;
	auto OnRenderHeroSkeleton( C_CitadelPlayerPawn* pC_CitadelPlayerPawn ) -> void;

private:
	auto OnRenderTrooperEsp( C_NPC_Trooper* pC_NPC_Trooper , const Rect_t& bBox , const bool bVisible ) -> void;
	auto OnRenderTrooperSkeleton( C_NPC_Trooper* pC_NPC_Trooper ) -> void;

private:
	auto OnRenderTrooperNeutralEsp( C_NPC_TrooperNeutral* pC_NPC_TrooperNeutral , const bool bVisible ) -> void;
	auto OnRenderTrooperNeutralSkeleton( C_NPC_TrooperNeutral* pC_NPC_TrooperNeutral ) -> void;

public:
	auto CalculateBoundingBoxes() -> void;

private:
	SoundListVecType_t m_SoundList;
	Lock_t m_SoundLock;

private:
	constexpr static auto g_SoundShowTime = 1000;
};

auto GetVisual() -> CVisual*;
