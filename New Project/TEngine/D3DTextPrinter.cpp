#include "D3DTextPrinter.h"
#include "SafeRelease.h"
#include "Locator.h"
#include "IRenderDevice.h"
#include "TError.h"

//-------------------------------------------------------------------------------
D3DTextPrinter::D3DTextPrinter()
:	m_pFont(0),
	m_nNumFonts(0),
	m_pD3DXSprite(0)
{

}

//-------------------------------------------------------------------------------
D3DTextPrinter::~D3DTextPrinter()
{
	// Release Fonts
	for (UINT i=0; i < m_nNumFonts; i++)
	{
		SafeRelease(m_pFont[i]);
	}

	SafeDeleteArray(m_pFont);
}

//-------------------------------------------------------------------------------
bool D3DTextPrinter::CreateFont(const char *chType, int nWeight, 
								bool bItalic, DWORD dwSize, TFontHandle *pID)
{
	// create the sprite once
	if (m_pD3DXSprite == NULL)
	{
		if(FAILED(D3DXCreateSprite( 
			Locator::GetRenderDevice()->GetDevice(), &m_pD3DXSprite )))
		{
			TASSERT(false, "[D3DTextPrinter::CreateFont] Failed to create sprite");
			return false;
		}
	}

	HDC		hDC;
	int		nHeight;

	if (!pID) { return false; }	// invalid id

	hDC = GetDC( NULL );
	nHeight = -MulDiv(dwSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	ReleaseDC(NULL, hDC);

	m_pFont = (LPD3DXFONT*)realloc(m_pFont, sizeof(LPD3DXFONT)*(m_nNumFonts+1));

	if (FAILED(D3DXCreateFont(
		Locator::GetRenderDevice()->GetDevice(),
		nHeight, (nHeight/2), nWeight, 1,
		bItalic, DEFAULT_CHARSET,
		OUT_STROKE_PRECIS, ANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, chType,
		&m_pFont[m_nNumFonts] )))
	{
		TDebugPrint(("[D3DTextPrinter::CreateFont] Couldn't create font"));
		return false;
	}

	(*pID) = m_nNumFonts;
	m_nNumFonts++;
	return true;
}

//-------------------------------------------------------------------------------
void D3DTextPrinter::DrawText(TFontHandle nID, int x, int y, UCHAR r, UCHAR g, UCHAR b, const char *ch, ...)
{
	TASSERT( (nID >= 0 && nID < m_nNumFonts), "[D3DTextPrinter::DrawText] Invalid font supplied");

	//move optional parameters into the string
	static char cch[1024];
	char *pArgs;
	pArgs = (char*) &ch + sizeof(ch);
	vsprintf_s(cch, 1024, ch, pArgs);

	m_pD3DXSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );

	RECT rc = {x, y, 0, 0};
	m_pFont[nID]->DrawText(m_pD3DXSprite, cch, -1, &rc, DT_NOCLIP, D3DCOLOR_ARGB(255, r,g,b));

	m_pD3DXSprite->End();
}

//-------------------------------------------------------------------------------
void D3DTextPrinter::OnLostDevice()
{
	for (int i=0; i<(int)m_nNumFonts; i++)
	{ 
		m_pFont[i]->OnLostDevice(); 
	}

	m_pD3DXSprite->OnLostDevice();
}

//-------------------------------------------------------------------------------
void D3DTextPrinter::OnResetDevice()
{
	for (UINT i=0; i <(int)m_nNumFonts; i++) 
	{
		m_pFont[i]->OnResetDevice(); 
	}

	m_pD3DXSprite->OnResetDevice();
}
