#pragma once

#include <vector>

#include <Common/Common.hpp>
#include <ImGui/imgui.h>

#include <DeadLock/SDK/Math/Vector3.hpp>
#include <DeadLock/SDK/Math/Rect_t.hpp>

class CCitadelPlayerController;
class C_CitadelPlayerPawn;

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
};

class CVisual final : public IVisual
{
	using SoundListVecType_t = std::vector<SoundData_t>;
	using Lock_t = std::mutex;

public:
	virtual void OnRender() override;
	virtual void OnStartSound( const Vector3& Pos , const int SourceEntityIndex , const char* szSoundName ) override;
	virtual void OnClientOutput() override;

private:
	auto OnRenderSound() -> void;
	auto OnRenderPlayerEsp( CCitadelPlayerController* pCCitadelPlayerController , const Rect_t& Box ) -> void;
	auto OnRenderSkeleton( C_CitadelPlayerPawn* pC_CitadelPlayerPawn ) -> void;

public:
	auto CalculateBoundingBoxes() -> void;

private:
	SoundListVecType_t m_SoundList;
	Lock_t m_SoundLock;

private:
	constexpr static auto g_SoundShowTime = 1000;
};

auto GetVisual() -> CVisual*;
