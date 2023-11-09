#ifndef __MOUSEKEYINPUT_H__
#define __MOUSEKEYINPUT_H__

//+-------------------------------------------------------
//| Keyboard & Mouse Input using Direct Input.
//| Version 1.002
//+-------------------------------------------------------

#ifndef __IINPUT_H__
#include "IInput.h"
#endif

#define DIRECTINPUT_VERSION 0x0800

#ifndef __DINPUT_INCLUDED__
#include <dinput.h>
#endif

// map mouse input to some unused keys
// so that they can be used with IsDown and IsPressed
#define MOUSE_INPUT_LEFT  0x70
#define MOUSE_INPUT_RIGHT 0x73

class MouseKeyInput : public IInput
{
public:
	MouseKeyInput();
	~MouseKeyInput();

	bool					Init(HINSTANCE, HWND);
	bool					Update();

	bool					IsDown(UINT);
	bool					IsPressed(UINT);
	bool					IsReleased(UINT);
	bool					IsAnyKeyPressed();
	bool					IsAnyKeyClicked();

	char					GetClickedChar();

	long					GetXAmt();
	long					GetYAmt();

private:
	void					Release();

	LPDIRECTINPUT8			m_pDirectInput; 
	LPDIRECTINPUTDEVICE8	m_pKeyboard;
    LPDIRECTINPUTDEVICE8	m_pMouse;
	DIMOUSESTATE2			m_MouseInput;

	char					m_Keys[256];		//key states
	char					m_OldKeys[256];
	long					xamt, yamt;

};

#endif
