#pragma once

class C_CitadelPlayerPawn;

class CL_CitadelPlayerPawn final
{
public:
	auto GetLocal() -> C_CitadelPlayerPawn*;
};

auto GetCL_CitadelPlayerPawn() -> CL_CitadelPlayerPawn*;
