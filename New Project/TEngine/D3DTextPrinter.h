#ifndef __TTEXTPRINTER_H__
#define __TTEXTPRINTER_H__

//+-------------------------------------------------------
//| D3D text printer. Uses LPD3DXFONT and ID3DXSprite to draw the text.
//| Version 1.001
//+-------------------------------------------------------

#ifndef __D3DX9_H__
#include <d3dx9.h>
#endif

#ifndef _VECTOR_
#include <vector>
#endif

#include "ITextPrinter.h"

class D3DTextPrinter : public ITextPrinter
{
public:
	D3DTextPrinter();
	~D3DTextPrinter();

	bool CreateFont(const char *chType, int nWeight, 
							 bool bItalic, DWORD dwSize, TFontHandle *pID);

	void DrawText(TFontHandle nID, int x, int y, UCHAR r, UCHAR g, UCHAR b, const char *ch, ...);

	void OnLostDevice();
	void OnResetDevice();

private:
	LPD3DXFONT*		m_pFont;
	unsigned int	m_nNumFonts;
	ID3DXSprite*	m_pD3DXSprite;
};

#endif
