#include "Hook_MouseInputEnabled.hpp"

#include <AndromedaClient/CAndromedaGUI.hpp>

auto Hook_MouseInputEnabled( CCitadelInput* pCCitadelInput ) -> bool
{
	if ( GetAndromedaGUI()->IsVisible() )
		return false;

	return MouseInputEnabled_o( pCCitadelInput );
}
