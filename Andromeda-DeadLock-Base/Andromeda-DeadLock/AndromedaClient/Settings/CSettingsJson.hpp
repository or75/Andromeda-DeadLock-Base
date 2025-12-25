#pragma once

#include <vector>
#include <Common/Common.hpp>

#include <ImGui/imgui.h>

#include <rapidjson/prettywriter.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/error/en.h>

class CSettingsJson final
{
public:
	using VecConfigList_t = std::vector<std::string>;

public:
	auto LoadConfig( const std::string& JsonFile ) -> void;
	auto SaveConfig( const std::string& JsonFile ) -> void;

public:
	auto DeleteConfig( const std::string& JsonFile ) -> void;

public:
	auto UpdateConfigList() -> void;

private:
	auto GetIntJson( const rapidjson::Value& JsonValue , const char* Name , int& Output , const int Min , const int Max ) -> void;
	auto GetBoolJson( const rapidjson::Value& JsonValue , const char* Name , bool& Output ) -> void;
	auto GetFloatJson( const rapidjson::Value& JsonValue , const char* Name , float& Output , const float Min , const float Max ) -> void;
	auto GetColorJson( const rapidjson::Value& JsonValue , const char* Name , float* Output ) -> void;

private:
	auto AddIntJson( rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& Writer , const char* Name , int& Output ) -> void;
	auto AddUInt64Json( rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& Writer , const char* Name , uint64_t& Output ) -> void;
	auto AddBoolJson( rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& Writer , const char* Name , bool& Output ) -> void;
	auto AddStringJson( rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& Writer , const char* Name , std::string& Output ) -> void;
	auto AddFloatJson( rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& Writer , const char* Name , float& Output ) -> void;
	auto AddColorJson( rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& Writer , const char* Name , float* Output ) -> void;

public:
	inline auto GetConfigList() -> VecConfigList_t&
	{
		return m_vecConfigList;
	}

	inline auto GetConfigLoadedIndex() -> uint32_t
	{
		return m_nConfigLoadedIndex;
	}

private:
	VecConfigList_t m_vecConfigList;
	uint32_t m_nConfigLoadedIndex = 0;
};

auto GetSettingsJson() -> CSettingsJson*;
