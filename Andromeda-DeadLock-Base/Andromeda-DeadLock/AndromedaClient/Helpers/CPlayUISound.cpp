#include "CPlayUISound.hpp"

#include <AndromedaClient/Resources/UI_Sounds/UI_Click.hpp>
#include <AndromedaClient/Resources/UI_Sounds/UI_Hover.hpp>

static CPlayUISound g_CPlayUISound{};

auto CPlayUISound::PlayUISound( const UISound Sound ) -> void
{
	if ( Sound == UISound::UI_Click )
		PlaySoundA( (LPCTSTR)ui_click_data , 0 , SND_MEMORY | SND_ASYNC );
	else if ( Sound == UISound::UI_Hover )
		PlaySoundA( (LPCTSTR)ui_hover_data , 0 , SND_MEMORY | SND_ASYNC );
}

auto GetPlayUISound() -> CPlayUISound*
{
	return &g_CPlayUISound;
}
