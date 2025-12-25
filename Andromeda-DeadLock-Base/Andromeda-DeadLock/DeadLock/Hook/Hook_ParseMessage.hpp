#pragma once

#include <Common/Common.hpp>

class CDemoRecorder;
class CNetworkSerializerPB;
class CNetMessagePB;

auto Hook_ParseMessage( CDemoRecorder* pDemoRecorder , CNetworkSerializerPB* pSerializer , CNetMessagePB* pNetMessage ) -> bool;

using ParseMessage_t = decltype( &Hook_ParseMessage );
inline ParseMessage_t ParseMessage_o = nullptr;
