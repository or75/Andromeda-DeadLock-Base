#pragma once

#include <Common/Common.hpp>

enum class UISound : uint32_t
{
	UI_Click,
	UI_Hover,
};

class CPlayUISound final
{
public:
	auto PlayUISound( const UISound Sound ) -> void;
};

auto GetPlayUISound() -> CPlayUISound*;
