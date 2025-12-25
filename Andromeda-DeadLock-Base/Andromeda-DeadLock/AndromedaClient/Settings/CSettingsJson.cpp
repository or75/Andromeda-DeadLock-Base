#include "CSettingsJson.hpp"
#include "DllLauncher.hpp"

#include <filesystem>
#include <fstream>

#include <AndromedaClient/Settings/Settings.hpp>

static CSettingsJson g_CSettingsJson{};

auto CSettingsJson::LoadConfig( const std::string& JsonFile ) -> void
{
	auto ConfigLoadedIndex = 0u;
	const auto ConfigFilePath = GetDllDir() + JsonFile;

	for ( const auto& Config : m_vecConfigList )
	{
		if ( Config == JsonFile )
		{
			m_nConfigLoadedIndex = ConfigLoadedIndex;
			break;
		}

		ConfigLoadedIndex++;
	}

	std::ifstream ConfigFile( ConfigFilePath );

	rapidjson::IStreamWrapper StreamWrapper( ConfigFile );
	rapidjson::Document DocumentConfig;

	DocumentConfig.ParseStream( StreamWrapper );

	if ( !DocumentConfig.HasParseError() )
	{
		const auto& SettingsVisual = DocumentConfig[XorStr( "Settings" )][XorStr( "Visual" )];

		if ( !SettingsVisual.IsNull() )
		{
			GetBoolJson( SettingsVisual , XorStr( "Active" ) , Settings::Visual::Active );
			GetBoolJson( SettingsVisual , XorStr( "SoundStepEsp" ) , Settings::Visual::SoundStepEsp );
		}

		const auto& SettingsMisc = DocumentConfig[XorStr( "Settings" )][XorStr( "Misc" )];

		if ( !SettingsMisc.IsNull() )
		{
			GetIntJson( SettingsMisc , XorStr( "MenuAlpha" ) , Settings::Misc::MenuAlpha , 100 , 255 );
			GetIntJson( SettingsMisc , XorStr( "MenuStyle" ) , Settings::Misc::MenuStyle , 0 , 2 );
			GetBoolJson( SettingsMisc , XorStr( "MenuSounds" ) , Settings::Misc::MenuSounds );
		}

		const auto& SettingsColors = DocumentConfig[XorStr( "Settings" )][XorStr( "Colors" )];

		if ( !SettingsColors.IsNull() )
		{
			const auto& ColorsVisual = SettingsColors[XorStr( "Visual" )];

			if ( !ColorsVisual.IsNull() )
			{
				GetColorJson( ColorsVisual , XorStr( "SoundStepEsp" ) , Settings::Colors::Visual::SoundStepEsp );
			}
		}
	}
	else
	{
		DEV_LOG( "[error] LoadConfig: %s -> %s , %i\n" , ConfigFilePath.c_str() , rapidjson::GetParseError_En( DocumentConfig.GetParseError() ) , DocumentConfig.GetErrorOffset() );
	}

	DocumentConfig.Clear();
	ConfigFile.close();
}

auto CSettingsJson::SaveConfig( const std::string& JsonFile ) -> void
{
	const auto ConfigFilePath = GetDllDir() + JsonFile;

	std::ofstream ConfigFile( ConfigFilePath );

	rapidjson::OStreamWrapper StreamWrapper( ConfigFile );
	rapidjson::PrettyWriter<rapidjson::OStreamWrapper> ConfigWriter( StreamWrapper );

	ConfigWriter.SetIndent( '\t' , 1 );
	ConfigWriter.SetFormatOptions( rapidjson::PrettyFormatOptions::kFormatSingleLineArray );
	ConfigWriter.SetMaxDecimalPlaces( 2 );

	ConfigWriter.StartObject();
	{
		ConfigWriter.String( XorStr( "Settings" ) );
		{
			ConfigWriter.StartObject();
			{
				ConfigWriter.String( XorStr( "Visual" ) );
				{
					ConfigWriter.StartObject();
					{
						AddBoolJson( ConfigWriter , XorStr( "Active" ) , Settings::Visual::Active );

						AddBoolJson( ConfigWriter , XorStr( "SoundStepEsp" ) , Settings::Visual::SoundStepEsp );
					}
					ConfigWriter.EndObject();
				}

				ConfigWriter.String( XorStr( "Misc" ) );
				{
					ConfigWriter.StartObject();
					{
						AddIntJson( ConfigWriter , XorStr( "MenuAlpha" ) , Settings::Misc::MenuAlpha );
						AddIntJson( ConfigWriter , XorStr( "MenuStyle" ) , Settings::Misc::MenuStyle );
						AddBoolJson( ConfigWriter , XorStr( "MenuSounds" ) , Settings::Misc::MenuSounds );
					}
					ConfigWriter.EndObject();
				}

				ConfigWriter.String( XorStr( "Colors" ) );
				{
					ConfigWriter.StartObject();
					{
						ConfigWriter.String( XorStr( "Visual" ) );
						{
							ConfigWriter.StartObject();
							{
								AddColorJson( ConfigWriter , XorStr( "SoundStepEsp" ) , Settings::Colors::Visual::SoundStepEsp );
							}
							ConfigWriter.EndObject();
						}
					}
					ConfigWriter.EndObject();
				}

			}
			ConfigWriter.EndObject();
		}
	}
	ConfigWriter.EndObject();

	ConfigFile.close();
}

auto CSettingsJson::DeleteConfig( const std::string& JsonFile ) -> void
{
	const auto ConfigFilePath = GetDllDir() + JsonFile;

	DeleteFileA( ConfigFilePath.c_str() );
}

auto CSettingsJson::UpdateConfigList() -> void
{
	m_vecConfigList.clear();

	for ( const auto& Entry : std::filesystem::directory_iterator( GetDllDir().c_str() ) )
	{
		if ( Entry.is_regular_file() )
		{
			if ( Entry.path().extension().string() == XorStr( ".json" ) )
				m_vecConfigList.emplace_back( Entry.path().filename().string() );
		}
	}
}

auto CSettingsJson::GetIntJson( const rapidjson::Value& JsonValue , const char* Name , int& Output , const int Min , const int Max ) -> void
{
	if ( !JsonValue.IsNull() && JsonValue.HasMember( Name ) )
	{
		auto& Value = JsonValue[Name];

		if ( !Value.IsNull() && Value.IsInt() )
		{
			const auto IntValue = Value.GetInt();

			if ( IntValue < Min )
				Output = Min;
			else if ( IntValue > Max )
				Output = Max;
			else
				Output = IntValue;
		}
	}
}

auto CSettingsJson::GetBoolJson( const rapidjson::Value& JsonValue , const char* Name , bool& Output ) -> void
{
	if ( !JsonValue.IsNull() && JsonValue.HasMember( Name ) )
	{
		auto& Value = JsonValue[Name];

		if ( !Value.IsNull() && Value.IsBool() )
			Output = Value.GetBool();
	}
}

auto CSettingsJson::GetFloatJson( const rapidjson::Value& JsonValue , const char* Name , float& Output , const float Min , const float Max ) -> void
{
	if ( !JsonValue.IsNull() && JsonValue.HasMember( Name ) )
	{
		auto& Value = JsonValue[Name];

		if ( !Value.IsNull() && Value.IsFloat() )
		{
			Output = std::clamp( Value.GetFloat() , Min , Max );
		}
	}
}

auto CSettingsJson::GetColorJson( const rapidjson::Value& JsonValue , const char* Name , float* Output ) -> void
{
	if ( !JsonValue.IsNull() && JsonValue.HasMember( Name ) )
	{
		auto& Value = JsonValue[Name];

		if ( !Value.IsNull() && Value.IsArray() && Value.GetArray().Size() == 3 )
		{
			Output[0] = Value.GetArray()[0].GetFloat();
			Output[1] = Value.GetArray()[1].GetFloat();
			Output[2] = Value.GetArray()[2].GetFloat();

			Output[0] = std::clamp( Output[0] , 0.f , 1.f );
			Output[1] = std::clamp( Output[1] , 0.f , 1.f );
			Output[2] = std::clamp( Output[2] , 0.f , 1.f );
		}
	}
}

auto CSettingsJson::AddIntJson( rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& Writer , const char* Name , int& Output ) -> void
{
	Writer.String( Name );
	Writer.Int( Output );
}

auto CSettingsJson::AddUInt64Json( rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& Writer , const char* Name , uint64_t& Output ) -> void
{
	Writer.String( Name );
	Writer.Uint64( Output );
}

auto CSettingsJson::AddBoolJson( rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& Writer , const char* Name , bool& Output ) -> void
{
	Writer.String( Name );
	Writer.Bool( Output );
}

auto CSettingsJson::AddStringJson( rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& Writer , const char* Name , std::string& Output ) -> void
{
	Writer.String( Name );
	Writer.String( Output.c_str() );
}

auto CSettingsJson::AddFloatJson( rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& Writer , const char* Name , float& Output ) -> void
{
	Writer.String( Name );
	Writer.Double( static_cast<double>( Output ) );
}

auto CSettingsJson::AddColorJson( rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& Writer , const char* Name , float* Output ) -> void
{
	Writer.String( Name );
	Writer.StartArray();

	Writer.Double( static_cast<double>( Output[0] ) );
	Writer.Double( static_cast<double>( Output[1] ) );
	Writer.Double( static_cast<double>( Output[2] ) );

	Writer.EndArray();
}

auto GetSettingsJson() -> CSettingsJson*
{
	return &g_CSettingsJson;
}
