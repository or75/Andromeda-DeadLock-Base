#include "IGameEvent.hpp"

#include <DeadLock/SDK/FunctionListSDK.hpp>

auto IGameEvent::GetName() -> const char*
{
	return IGameEvent_GetName( this );
}
