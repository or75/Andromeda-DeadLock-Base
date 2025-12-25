#pragma once

#include <Common/Common.hpp>

class CCitadelInput;

auto Hook_CreateMove( CCitadelInput* pCitadelInput , uint32_t split_screen_index , char a3 ) -> void;

using CreateMove_t = decltype( &Hook_CreateMove );
inline CreateMove_t CreateMove_o = nullptr;
