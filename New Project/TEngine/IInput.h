#ifndef __IINPUT_H__
#define __IINPUT_H__

//+----------------------------------------
//| Interface for game input.
//| Version 1.001
//+----------------------------------------

#ifndef _WINDOWS_
#include <windows.h>
#endif

class IInput
{
public:
	IInput() {}
	virtual ~IInput() {}

	virtual bool Update() = 0;

	virtual bool	IsDown(UINT) = 0;
	virtual bool	IsPressed(UINT) = 0;	// first press
	virtual bool	IsReleased(UINT) = 0;
	virtual bool	IsAnyKeyPressed() = 0;
	virtual bool	IsAnyKeyClicked() = 0;

	virtual char	GetClickedChar() = 0;

	virtual long	GetXAmt() = 0;
	virtual long	GetYAmt() = 0;
};

#endif
