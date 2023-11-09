#ifndef __ITEXTPRINTER_H__
#define __ITEXTPRINTER_H__

//+-------------------------------------------------------
//| Interface for a class that draws text to the screen.
//| Version 1.001
//+-------------------------------------------------------

#ifndef _WINDOWS_
#include <windows.h>
#endif

#ifndef __TFONTHANDLEDEFINED__
#define __TFONTHANDLEDEFINED__
#	define TFontHandle unsigned int
#endif

class ITextPrinter
{
public:
	ITextPrinter() {}
	virtual ~ITextPrinter() {}

	virtual bool CreateFont(const char *chType, int nWeight, 
							 bool bItalic, DWORD dwSize, TFontHandle *pID) = 0;

	virtual void DrawText(TFontHandle nID, int x, int y, UCHAR r, 
							UCHAR g, UCHAR b, const char *ch, ...) = 0;

	virtual void OnLostDevice()  = 0;
	virtual void OnResetDevice() = 0;
};

#endif
