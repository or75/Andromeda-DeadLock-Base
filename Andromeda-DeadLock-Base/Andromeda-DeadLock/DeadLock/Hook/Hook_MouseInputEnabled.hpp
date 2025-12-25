#pragma once

#include <Common/Common.hpp>

class CCitadelInput;

auto Hook_MouseInputEnabled( CCitadelInput* pCCitadelInput ) -> bool;

using MouseInputEnabled_t = decltype( &Hook_MouseInputEnabled );
inline MouseInputEnabled_t MouseInputEnabled_o = nullptr;
