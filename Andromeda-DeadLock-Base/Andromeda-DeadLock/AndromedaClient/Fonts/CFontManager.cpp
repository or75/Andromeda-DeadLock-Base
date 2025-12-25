#include "CFontManager.hpp"

#include <AndromedaClient/CAndromedaGUI.hpp>

static CFontManager g_CFontManager{};

auto CFontManager::FirstInitFonts() -> void
{
	if ( !m_bInit )
	{
		if ( FAILED( FW1CreateFactory( FW1_VERSION , &m_pFW1Factory ) ) )
		{
			DEV_LOG( "[error] FW1CreateFactory\n" );
			m_bInit = true;
			return;
		}

		m_VerdanaFont.InitFont( m_pFW1Factory , L"Verdana" , 9.f );

		float screenWidth = ImGui::GetIO().DisplaySize.x;
		float screenHeight = ImGui::GetIO().DisplaySize.y;

		D3D11_VIEWPORT viewport;

		viewport.Width = screenWidth;
		viewport.Height = screenHeight;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		GetAndromedaGUI()->GetDeviceContext()->RSSetViewports( 1 , &viewport );

		m_pFW1Factory->Release();

		m_bInit = true;
	}
}

auto GetFontManager() -> CFontManager*
{
	return &g_CFontManager;
}
