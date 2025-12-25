#include "CAndromedaMenu.hpp"

#include <string>

#include <AndromedaClient/CAndromedaGUI.hpp>
#include <AndromedaClient/Settings/Settings.hpp>
#include <AndromedaClient/Settings/CSettingsJson.hpp>
#include <AndromedaClient/Helpers/CPlayUISound.hpp>
#include <AndromedaClient/Fonts/FontAwesomeIcon.hpp>

static CAndromedaMenu g_CAndromedaMenu{};

static auto ÑolorsGroupItemListGetter = []( void* vec , int idx , const char** out_text )
{
	auto pColorList = static_cast<CAndromedaMenu::ColorItemsList_t*>( vec );
	if ( idx < 0 || idx >= static_cast<int>( pColorList->size() ) ) { return false; }
	*out_text = pColorList->at( idx ).Name.c_str();
	return true;
};

static auto ÑolorsGroupListGetter = []( void* vec , int idx , const char** out_text )
{
	auto& pSelected = GetAndromedaMenu()->GetColorGroupNameFromSelectIndex( idx );
	*out_text = pSelected.c_str();
	return true;
};

auto CAndromedaMenu::InitColors() -> void
{
	m_ColorsVisual[XorStr( "Visual" )] =
	{
		{ &Settings::Colors::Visual::SoundStepEsp[0] , XorStr( "Sound Step Esp" ) },
	};

	m_ColorsManagerGroupList =
	{
		&m_ColorsVisual,
	};
}

auto CAndromedaMenu::OnRenderMenu() -> void
{
	const float MenuAlpha = static_cast<float>( Settings::Misc::MenuAlpha ) / 255.f;

	ImGui::PushStyleVar( ImGuiStyleVar_Alpha , MenuAlpha );
	ImGui::SetNextWindowSize( ImVec2( g_MainWindowSizeX , g_MainWindowSizeY ) , ImGuiCond_Always );

	if ( ImGui::Begin( XorStr( CHEAT_NAME ) , 0 , ImGuiWindowFlags_NoResize ) )
	{
		RenderRandomBorderColor();

		ImGui::BeginGroup();
		RenderLeftChild();
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		RenderRightChild();
		ImGui::EndGroup();
	}

	ImGui::End();

	OnRenderVisual();
	OnRenderMisc();
	OnRenderColors();

	ImGui::PopStyleVar();

	if ( ImGui::IsMouseClicked( ImGuiMouseButton_Left ) )
		PlayClick();
}

auto CAndromedaMenu::RenderLeftChild() -> void
{
	ImGui::BeginChild( XorStr( "AndromedaChildLeft" ) , ImVec2( g_ChildSizeX , -1.f ) , true );

	const auto ConfigList = GetSettingsJson()->GetConfigList();
	const auto ConfigLoaded = GetSettingsJson()->GetConfigLoadedIndex();
	const auto ConfigExist = ConfigList.empty() ? false : true;

	for ( auto idx = 0u; idx < ConfigList.size(); idx++ )
	{
		auto ConfigName = ConfigList[idx];
		auto ConfigColor = ImColor( 255 , 255 , 255 );

		if ( ConfigLoaded == idx && m_nConfigSelected == idx )
		{
			ConfigName += XorStr( " Loaded+" );
			ConfigColor = ImColor( 0 , 255 , 0 );
		}
		else if ( ConfigLoaded == idx )
		{
			ConfigName += XorStr( " Loaded" );
			ConfigColor = ImColor( 0 , 255 , 0 );
		}
		else if ( m_nConfigSelected == idx )
		{
			ConfigName += XorStr( " Selected" );
			ConfigColor = ImColor( 255 , 140 , 0 );
		}

		ImGui::PushStyleVar( ImGuiStyleVar_ButtonTextAlign , ImVec2( 0.f , 0.5f ) );
		ImGui::PushStyleColor( ImGuiCol_Text , ConfigColor.operator ImVec4() );

		if ( ImGui::Button( ConfigName.c_str() , ImVec2( -1.f , 0.f ) ) )
			m_nConfigSelected = idx;

		InternalPlaySoundHoveredItem( XorStr( "Config##" ) + std::to_string( idx ) );

		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
	}

	ImGui::EndChild();
}

auto CAndromedaMenu::RenderRightChild() -> void
{
	ImGui::BeginChild( XorStr( "AndromedaChildRight" ) , ImVec2( g_ChildSizeX , ImGui::GetContentRegionAvail().y - 2.f ) , true );

	if ( ButtonIcon( ICON_FA_EYE , XorStr( "Visual##AndromedaVisualIcon" ) , ImVec2( -1.f , g_ButtonSizeY ) ) )
	{
		m_bVisualWindow = !m_bVisualWindow;
	}

	if ( ButtonIcon( ICON_FA_PUZZLE_PIECE , XorStr( "Misc##AndromedaMiscIcon" ) , ImVec2( -1.f , g_ButtonSizeY ) ) )
	{
		m_bMiscWindow = !m_bMiscWindow;
	}

	if ( ButtonIcon( ICON_FA_RAINBOW , XorStr( "Colors##AndromedaColorsIcon" ) , ImVec2( -1.f , g_ButtonSizeY ) ) )
	{
		m_bColorsWindow = !m_bColorsWindow;
	}

	ImGui::Separator();

	RenderInputText( XorStr( "Config Name:" ) , XorStr( "##NewConfigFileName" ) , m_szNewConfigFileName , 32 , 0.f , -1.f );

	if ( ButtonIcon( ICON_FA_FILE_ALT , XorStr( "Create & Save New Config##CreateAndSaveNewConfig" ) , ImVec2( -1.f , g_ButtonSizeY ) ) )
	{
		const std::string ConfigFileName = m_szNewConfigFileName;

		if ( !ConfigFileName.empty() )
		{
			GetSettingsJson()->SaveConfig( ConfigFileName + XorStr( ".json" ) );
			GetSettingsJson()->UpdateConfigList();
		}
	}

	const auto ConfigList = GetSettingsJson()->GetConfigList();
	const auto ConfigExist = ConfigList.empty() ? false : true;

	if ( ButtonIcon( ICON_FA_DOWNLOAD , XorStr( "Load Selected Config##LoadSelectedConfig" ) , ImVec2( -1.f , g_ButtonSizeY ) ) && ConfigExist )
	{
		GetSettingsJson()->LoadConfig( ConfigList[m_nConfigSelected] );
	}

	if ( ButtonIcon( ICON_FA_SAVE , XorStr( "Save Selected Config##SaveSelectedConfig" ) , ImVec2( -1.f , g_ButtonSizeY ) ) && ConfigExist )
	{
		GetSettingsJson()->SaveConfig( ConfigList[m_nConfigSelected] );
	}

	if ( ButtonIcon( ICON_FA_CUT , XorStr( "Delete Selected Config##DeleteSelectedConfig" ) , ImVec2( -1.f , g_ButtonSizeY ) ) && ConfigExist )
	{
		GetSettingsJson()->DeleteConfig( ConfigList[m_nConfigSelected] );
		GetSettingsJson()->UpdateConfigList();
	}

	if ( ButtonIcon( ICON_FA_UNDO , XorStr( "Refresh Config List##RefreshConfigList" ) , ImVec2( -1.f , g_ButtonSizeY ) ) && ConfigExist )
	{
		GetSettingsJson()->UpdateConfigList();
	}

	ImGui::Separator();

	ImGui::Text( XorStr( "Cheat Version: %s" ) , CHEAT_VERSION );
	ImGui::Text( XorStr( "Cheat Build Date: %s" ) , __DATE__ );
	ImGui::Text( XorStr( "Cheat Build Time: %s" ) , __TIME__ );

	ImGui::EndChild();
}

auto CAndromedaMenu::OnRenderVisual() -> void
{
	if ( m_bVisualWindow )
	{
		ImGui::SetNextWindowSize( ImVec2( g_FirstWindowSizeX - 250.f , g_FirstWindowSizeY - 150.f ) , ImGuiCond_FirstUseEver );

		if ( ImGui::Begin( XorStr( "Visual##VisualWindow" ) , &m_bVisualWindow ) )
		{
			ImGui::BeginGroup();
			ImGui::BeginChild( XorStr( "##AndromedaVisualLeftChild" ) , ImVec2( ImGui::GetContentRegionAvail().x , ImGui::GetContentRegionAvail().y ) , ImGuiChildFlags_Borders );

			RenderCheckBox( XorStr( "Active" ) , XorStr( "##Settings.Visual.Active" ) , Settings::Visual::Active );

			ImGui::Separator();

			RenderCheckBox( XorStr( "Sound Step Esp" ) , XorStr( "##Settings.Visual.SoundStepEsp" ) , Settings::Visual::SoundStepEsp );

			ImGui::EndChild();
			ImGui::EndGroup();
		}

		ImGui::End();
	}
}

auto CAndromedaMenu::OnRenderMisc() -> void
{
	if ( m_bMiscWindow )
	{
		ImGui::SetNextWindowSize( ImVec2( g_FirstWindowSizeX , g_FirstWindowSizeY - 150.f ) , ImGuiCond_FirstUseEver );

		if ( ImGui::Begin( XorStr( "Misc##MiscWindow" ) , &m_bMiscWindow ) )
		{
			ImGui::BeginGroup();
			ImGui::BeginChild( XorStr( "##AndromedaMiscRightChild" ) , ImVec2( -1.f , ImGui::GetContentRegionAvail().y ) , ImGuiChildFlags_Borders );

			const char* szMenuStyle[] =
			{
				"Indigo",
				"Vermillion",
				"Classic Steam"
			};

			RenderSliderInt( XorStr( "Menu Alpha" ) , XorStr( "##Settings.Misc.MenuAlpha" ) , Settings::Misc::MenuAlpha , 100 , 255 , 150.f );

			if ( RenderComboBox( XorStr( "Menu Style" ) , XorStr( "##Settings.Misc.MenuStyle" ) , Settings::Misc::MenuStyle , szMenuStyle , IM_ARRAYSIZE( szMenuStyle ) , 150.f ) )
				GetAndromedaGUI()->UpdateStyle();

			RenderCheckBox( XorStr( "Menu Sounds" ) , XorStr( "##Settings.Misc.MenuSounds" ) , Settings::Misc::MenuSounds );

			ImGui::EndChild();
			ImGui::EndGroup();
		}

		ImGui::End();
	}
}

auto CAndromedaMenu::OnRenderColors() -> void
{
	if ( m_bColorsWindow )
	{
		ImGui::SetNextWindowSize( ImVec2( g_FirstWindowSizeX - 105.f , g_FirstWindowSizeY - 105.f ) , ImGuiCond_FirstUseEver );

		if ( ImGui::Begin( XorStr( "Colors##ColorsWindow" ) , &m_bColorsWindow ) )
		{
			if ( ImGui::BeginChild( XorStr( "ColorsManagerChild" ) , ImVec2( ImGui::GetContentRegionAvail().x , ImGui::GetContentRegionAvail().y ) , true ) )
			{
				ImGui::BeginGroup();

				auto OnColorGroupChanged = RenderComboBoxColorGroupList( 0 , XorStr( "##ColorsManagerGroup" ) , &m_nSelectGroupItem , static_cast<int>( m_ColorsManagerGroupList.size() ) , 5 , 0.f );

				auto pSelectedGroup = GetColorGroupNameFromSelectIndex( m_nSelectGroupItem );
				auto pSelectedColorList = GetColorListFromSelectIndex( m_nSelectGroupItem );

				if ( pSelectedColorList && m_nSelectColorItem >= pSelectedColorList->size() )
					m_nSelectColorItem = static_cast<uint32_t>( pSelectedColorList->size() ) - 1;

				auto OnChangeSelectColor = false;

				if ( pSelectedColorList )
				{
					ImGui::PushItemWidth( 190.f );
					OnChangeSelectColor = RenderListBoxColors( 0 , XorStr( "##ColorsManagerList" ) , &m_nSelectColorItem , pSelectedColorList , 12 );
					ImGui::PopItemWidth();

					ImGui::SameLine();
				}

				float* pSelectedColor = nullptr;
				float NullColor[4] = { 0.f };

				if ( pSelectedColorList )
					pSelectedColor = &pSelectedColorList->at( m_nSelectColorItem ).Color[0];
				else
					pSelectedColor = &NullColor[0];

				if ( pSelectedColor )
				{
					if ( OnChangeSelectColor || OnColorGroupChanged || ( m_ImVecRefColor.x == 0.f && m_ImVecRefColor.y == 0.f && m_ImVecRefColor.z == 0.f && m_ImVecRefColor.w == 1.f ) )
					{
						m_ImVecRefColor.x = pSelectedColor[0];
						m_ImVecRefColor.y = pSelectedColor[1];
						m_ImVecRefColor.z = pSelectedColor[2];
					}
				}
				else
					pSelectedColor = &NullColor[0];

				ImGui::PushItemWidth( 190.f );

				bool OnChangeColorBar = ImGui::ColorPicker4( XorStr( "New Color##ColorsManagerPickerBar" ) , pSelectedColor , ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_PickerHueBar , &m_ImVecRefColor.x );

				ImGui::PopItemWidth();

				ImGui::EndGroup();
				ImGui::EndChild();
			}
		}

		ImGui::End();
	}
}

auto CAndromedaMenu::ButtonIcon( const char8_t* szIcon , const char* szText , ImVec2 Size ) -> bool
{
	ImGui::PushFont( GetAndromedaGUI()->m_pFontAwesomeIcons );
	ImGui::Text( XorStr( "%s" ) , szIcon );
	ImGui::PopFont();

	ImGui::SameLine( 40.f );

	auto Ret = ImGui::Button( szText , Size );

	InternalPlaySoundHoveredItem( szText );

	return Ret;
}

auto CAndromedaMenu::RenderInputText( const char* szTitle , const char* szStrID , char* szBuffer , int BufferSize , float left_padding , float max_width ) -> bool
{
	const auto TextSize = ImGui::CalcTextSize( szTitle ).x + 10.f;

	if ( szTitle )
	{
		ImGui::AlignTextToFramePadding();
		ImGui::Text( szTitle );

		if ( left_padding <= 0.f )
			ImGui::SameLine( TextSize );
		else
			ImGui::SameLine( left_padding );
	}

	if ( max_width != -1.f )
		ImGui::PushItemWidth( max_width - TextSize );
	else
		ImGui::PushItemWidth( -1.f );

	auto Ret = ImGui::InputText( szStrID , szBuffer , BufferSize , 0 );
	InternalPlaySoundHoveredItem( szStrID );

	ImGui::PopItemWidth();

	return Ret;
}

auto CAndromedaMenu::RenderCheckBox( const char* szTitle , const char* szStrID , bool& SettingsItem ) -> bool
{
	if ( szTitle )
	{
		ImGui::AlignTextToFramePadding();
		ImGui::Text( szTitle );
		ImGui::SameLine( ImGui::CalcTextSize( szTitle ).x + 10.f );
	}

	const auto LeftPadding = ImGui::GetStyle().FramePadding.x;

	ImGui::Dummy( ImVec2( ImGui::GetContentRegionAvail().x - 27.f - LeftPadding , 0.f ) );
	ImGui::SameLine();

	const auto Ret = ImGui::Checkbox( szStrID , &SettingsItem );
	InternalPlaySoundHoveredItem( szStrID );

	return Ret;
}

auto CAndromedaMenu::RenderComboBox( const char* szTitle , const char* szStrID , int& v , const char* Items[] , int ItemsCount , float left_padding ) -> bool
{
	if ( szTitle )
	{
		ImGui::AlignTextToFramePadding();
		ImGui::Text( szTitle );

		if ( left_padding <= 0.f )
			ImGui::SameLine( ImGui::CalcTextSize( szTitle ).x + 10.f );
		else
			ImGui::SameLine( left_padding );
	}

	ImGui::PushItemWidth( -1.f );
	const auto Ret = ImGui::Combo( szStrID , &v , Items , ItemsCount );
	InternalPlaySoundHoveredItem( szStrID );
	ImGui::PopItemWidth();

	return Ret;
}

auto CAndromedaMenu::RenderSliderInt( const char* szTitle , const char* szStrID , int& v , int min , int max , float left_padding ) -> void
{
	if ( szTitle )
	{
		ImGui::AlignTextToFramePadding();
		ImGui::Text( szTitle );

		if ( left_padding <= 0.f )
			ImGui::SameLine( ImGui::CalcTextSize( szTitle ).x + 10.f );
		else
			ImGui::SameLine( left_padding );
	}

	ImGui::PushItemWidth( -1.f );
	ImGui::SliderInt( szStrID , &v , min , max , "%d" , ImGuiSliderFlags_AlwaysClamp );
	InternalPlaySoundHoveredItem( szStrID );
	ImGui::PopItemWidth();
}

auto CAndromedaMenu::RenderSliderFloat( const char* szTitle , const char* szStrID , float& v , float min , float max , float left_padding ) -> void
{
	if ( szTitle )
	{
		ImGui::AlignTextToFramePadding();
		ImGui::Text( szTitle );

		if ( left_padding <= 0.f )
			ImGui::SameLine( ImGui::CalcTextSize( szTitle ).x + 10.f );
		else
			ImGui::SameLine( left_padding );
	}

	ImGui::PushItemWidth( -1.f );
	ImGui::SliderFloat( szStrID , &v , min , max , "%.3f" , ImGuiSliderFlags_AlwaysClamp );
	InternalPlaySoundHoveredItem( szStrID );
	ImGui::PopItemWidth();
}

auto CAndromedaMenu::RenderRandomBorderColor() -> void
{
	auto WindowPos = ImGui::GetWindowPos() - ImVec2( 1.f , 1.f );
	auto WindowSize = ImGui::GetWindowSize() + ImVec2( 2.f , 2.f );

	auto pBackGround = ImGui::GetBackgroundDrawList();

	if ( m_flMainMenuColorStepH > 1.f )
		m_flMainMenuColorStepH = 0.f;
	else
		m_flMainMenuColorStepH += 0.01f;

	if ( m_flMainMenuColorStepS > 1.f )
		m_flMainMenuColorStepS = 0.f;
	else
		m_flMainMenuColorStepS += 0.01f;

	m_MainMenuBorderColor.SetHSV( m_flMainMenuColorStepH , m_flMainMenuColorStepS , 1.f );

	pBackGround->AddRect( WindowPos , WindowPos + WindowSize , m_MainMenuBorderColor , 1.f , 0 , 1.f );
}

auto CAndromedaMenu::GetColorGroupNameFromSelectIndex( uint32_t SelectedGroup ) -> const std::string&
{
	if ( SelectedGroup >= 0 && SelectedGroup < m_ColorsManagerGroupList.size() )
	{
		const auto& pGroup = m_ColorsManagerGroupList[SelectedGroup];

		if ( pGroup->size() == 1 )
			return pGroup->begin()->first;
	}

	static std::string null;
	return null;
}

auto CAndromedaMenu::GetColorListFromSelectIndex( uint32_t SelectedGroup ) -> ColorItemsList_t*
{
	if ( SelectedGroup >= 0 && SelectedGroup < m_ColorsManagerGroupList.size() )
	{
		const auto& pGroup = m_ColorsManagerGroupList[SelectedGroup];

		if ( pGroup && pGroup->size() == 1 && pGroup->begin()->second.size() > 0 )
			return &pGroup->begin()->second;
	}

	return nullptr;
}

auto CAndromedaMenu::RenderListBoxColors( const char* Title , const char* szStrID , uint32_t* current_item , CAndromedaMenu::ColorItemsList_t* pList , int height_in_items , float left_padding ) -> bool
{
	if ( Title )
	{
		ImGui::Text( Title );

		if ( left_padding <= 0.f )
			ImGui::SameLine( ImGui::CalcTextSize( Title ).x + 10.f );
		else
			ImGui::SameLine( left_padding );
	}

	const auto Ret = ImGui::ListBox( szStrID , reinterpret_cast<int*>( current_item ) , ÑolorsGroupItemListGetter , static_cast<void*>( pList ) , static_cast<int>( pList->size() ) , height_in_items );
	InternalPlaySoundHoveredItem( szStrID );
	return Ret;
}

auto CAndromedaMenu::RenderComboBoxColorGroupList( const char* Title , const char* szStrID , uint32_t* current_item , int items_count , int popup_max_height_in_items , float left_padding ) -> bool
{
	if ( Title )
	{
		ImGui::Text( Title );

		if ( left_padding <= 0.f )
			ImGui::SameLine( ImGui::CalcTextSize( Title ).x + 10.f );
		else
			ImGui::SameLine( left_padding );
	}

	ImGui::PushItemWidth( -1.f );
	ImGui::PushStyleVar( ImGuiStyleVar_WindowMinSize , ImVec2( 10.f , 10.f ) );

	const auto Ret = ImGui::Combo( szStrID , (int*)current_item , ÑolorsGroupListGetter , nullptr , items_count , popup_max_height_in_items );
	InternalPlaySoundHoveredItem( szStrID );

	ImGui::PopStyleVar();
	ImGui::PopItemWidth();

	return Ret;
}

auto CAndromedaMenu::PlayClick() -> void
{
	if ( Settings::Misc::MenuSounds )
		GetPlayUISound()->PlayUISound( UISound::UI_Click );
}

auto CAndromedaMenu::PlayHover() -> void
{
	if ( Settings::Misc::MenuSounds )
		GetPlayUISound()->PlayUISound( UISound::UI_Hover );
}

auto GetAndromedaMenu() -> CAndromedaMenu*
{
	return &g_CAndromedaMenu;
}
