#pragma once

#include <map>
#include <vector>

#include <Common/Common.hpp>
#include <ImGui/imgui.h>

class CAndromedaMenu final
{
public:
	auto InitColors() -> void;

public:
	auto OnRenderMenu() -> void;

private:
	auto RenderLeftChild() -> void;
	auto RenderRightChild() -> void;

private:
	auto OnRenderVisual() -> void;
	auto OnRenderMisc() -> void;
	auto OnRenderColors() -> void;

private:
	auto ButtonIcon( const char8_t* szIcon , const char* szText , ImVec2 Size ) -> bool;

private:
	auto RenderInputText( const char* szTitle , const char* szStrID , char* szBuffer , int BufferSize , float left_padding = 0.f , float max_width = -1.f ) -> bool;
	auto RenderCheckBox( const char* szTitle , const char* szStrID , bool& SettingsItem ) -> bool;
	auto RenderComboBox( const char* szTitle , const char* szStrID , int& v , const char* Items[] , int ItemsCount , float left_padding = 90.f ) -> bool;
	auto RenderSliderInt( const char* szTitle , const char* szStrID , int& v , int min , int max , float left_padding = 90.f ) -> void;
	auto RenderSliderFloat( const char* szTitle , const char* szStrID , float& v , float min , float max , float left_padding = 90.f ) -> void;

private:
	auto RenderRandomBorderColor() -> void;

private:
	float m_flMainMenuColorStepH = 0.0f;
	float m_flMainMenuColorStepS = 0.5f;

	bool m_bVisualWindow = false;
	bool m_bMiscWindow = false;
	bool m_bColorsWindow = false;

	static constexpr auto g_MainWindowSizeX = 460.f;
	static constexpr auto g_MainWindowSizeY = 490.f;

	static constexpr auto g_ChildSizeX = 220.f;
	static constexpr auto g_ButtonSizeY = 25.f;

	static constexpr auto g_FirstWindowSizeX = 595.f;
	static constexpr auto g_FirstWindowSizeY = 450.f;

public:
	inline auto SetConfigSelected( uint32_t Index ) -> void
	{
		m_nConfigSelected = Index;
	}

public:
	struct ColorsManagerItem_t
	{
		float* Color = nullptr;
		std::string Name;
	};

	using ColorItemsList_t = std::vector<ColorsManagerItem_t>;
	using ColorGroupItem_t = std::map<std::string , ColorItemsList_t>;
	using ColorGroupList_t = std::vector<ColorGroupItem_t*>;

	auto GetColorGroupNameFromSelectIndex( uint32_t SelectedGroup ) -> const std::string&;
	auto GetColorListFromSelectIndex( uint32_t SelectedGroup ) -> ColorItemsList_t*;

private:
	auto RenderListBoxColors( const char* Title , const char* str_id , uint32_t* current_item , CAndromedaMenu::ColorItemsList_t* pList , int height_in_items , float left_padding = 75.f ) -> bool;
	auto RenderComboBoxColorGroupList( const char* Title , const char* str_id , uint32_t* current_item , int items_count , int popup_max_height_in_items = -1 , float left_padding = 75.f ) -> bool;

private:
	auto PlayClick() -> void;
	auto PlayHover() -> void;

private:
	uint32_t m_nConfigSelected = 0;
	char m_szNewConfigFileName[32] = { 0 };
	ImColor m_MainMenuBorderColor;

private:
	std::map<std::string , bool> m_HoveredUIItems;

private:
	inline auto IsInternalUIItemHovered( std::string ItemID ) -> bool
	{
		if ( auto it = m_HoveredUIItems.find( ItemID ); it != m_HoveredUIItems.end() )
			return it->second;

		return false;
	}

	inline auto SetInternalUIItemHovered( std::string ItemID , bool Hovered ) -> void
	{
		m_HoveredUIItems[ItemID] = Hovered;
	}

	inline auto InternalPlaySoundHoveredItem( std::string ItemID ) -> void
	{
		if ( ImGui::IsItemHovered() )
		{
			if ( !IsInternalUIItemHovered( ItemID ) )
			{
				PlayHover();
				SetInternalUIItemHovered( ItemID , true );
			}
		}
		else
		{
			SetInternalUIItemHovered( ItemID , false );
		}
	}

private:
	uint32_t m_nSelectGroupItem = 0;
	uint32_t m_nSelectColorItem = 0;

	ImVec4 m_ImVecRefColor = ImVec4( 0.f , 0.f , 0.f , 1.f );

	ColorGroupItem_t m_ColorsVisual;

	ColorGroupList_t m_ColorsManagerGroupList;
};

auto GetAndromedaMenu() -> CAndromedaMenu*;
