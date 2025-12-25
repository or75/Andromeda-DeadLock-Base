#pragma once

#include <Common/Common.hpp>

namespace Settings
{
	namespace Visual
	{
		inline auto Active = true;

		inline auto SoundStepEsp = true;
	}
	namespace Misc
	{
		inline auto MenuAlpha = 200;
		inline auto MenuStyle = 0;
		inline auto MenuSounds = false;
	}
	namespace Colors
	{
		namespace Visual
		{
			inline float SoundStepEsp[3] = { 1.f , 1.f , 0.f };
		}
	}
}
