#pragma once

class CCitadelPlayerController;

class CL_CitadelPlayerController final
{
public:
	auto GetLocal() -> CCitadelPlayerController*;
};

auto GetCL_CitadelPlayerController() -> CL_CitadelPlayerController*;
