#pragma once

#include <Common/Common.hpp>

class IGameEvent
{
public:
	auto GetName() -> const char*;
};
