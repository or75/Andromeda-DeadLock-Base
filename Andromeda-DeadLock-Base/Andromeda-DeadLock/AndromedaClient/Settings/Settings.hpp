#pragma once

#include <Common/Common.hpp>

namespace Settings
{
	namespace Visual
	{
		inline auto Active = true;
		inline auto OnlyVisible = false;

		inline auto HeroTeam = true;
		inline auto HeroEnemy = true;
		inline auto HeroBox = true;
		inline auto HeroSkeleton = true;

		inline auto SoundStepEsp = true;

		inline auto TrooperTeam = true;
		inline auto TrooperEnemy = true;
		inline auto TrooperSkeleton = true;

		inline auto TrooperNeutral = true;
		inline auto TrooperNeutralSkeleton = true;

		inline auto HeroBoxType = 3;
		inline auto TrooperBoxType = 3;
		inline auto TrooperNeutralBoxType = 3;
	}
	namespace Misc
	{
		inline auto UnlockMiniMap = true;
		inline auto MenuAlpha = 200;
		inline auto MenuStyle = 0;
		inline auto MenuSounds = false;
	}
	namespace Colors
	{
		namespace Visual
		{
			inline float SoundStepEsp[3] = { 1.f , 1.f , 0.f };

			inline float HeroEnemy[3] = { 1.f , 75.f / 255.f , 75.f / 255.f };
			inline float HeroEnemyVisible[3] = { 0.f , 1.f , 0.f };

			inline float HeroTeam[3] = { 0.f , 160.f / 255.f , 1.f };
			inline float HeroTeamVisible[3] = { 0.f , 1.f , 0.f };

			inline float HeroSkeleton[3] = { 1.f , 1.f , 1.f };

			inline float TrooperEnemy[3] = { 1.f , 75.f / 255.f , 75.f / 255.f };
			inline float TrooperEnemyVisible[3] = { 0.f , 1.f , 0.f };
			inline float TrooperTeam[3] = { 0.f , 160.f / 255.f , 1.f };
			inline float TrooperTeamVisible[3] = { 0.f , 1.f , 0.f };

			inline float TrooperSkeleton[3] = { 1.f , 1.f , 1.f };

			inline float TrooperNeutral[3] = { 1.f , 174.f / 255.f , 0.f };
			inline float TrooperNeutralVisible[3] = { 0.f , 1.f , 0.f };
			inline float TrooperNeutralSkeleton[3] = { 1.f , 174.f / 255.f , 0.f };
		}
	}
}
