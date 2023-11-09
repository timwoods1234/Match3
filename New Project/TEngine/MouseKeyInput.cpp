#include "MouseKeyInput.h"
#include "SafeRelease.h"

//-------------------------------------------------------------------------------
MouseKeyInput::MouseKeyInput()
{
    m_pDirectInput	= NULL;
    m_pKeyboard		= NULL;
	m_pMouse		= NULL;
}

//-------------------------------------------------------------------------------
MouseKeyInput::~MouseKeyInput()
{
	Release();
}

//-------------------------------------------------------------------------------
bool MouseKeyInput::Init(HINSTANCE hInst, HWND hWnd)
{
	//Clear Memory
	memset(m_OldKeys, 0, sizeof(m_OldKeys));
	memset(m_Keys, 0, sizeof(m_Keys));
	ZeroMemory(&m_MouseInput, sizeof(m_MouseInput));
	xamt = yamt = 0;

	//Create the DirecMouseKeyInput object
	if(FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION,
			  IID_IDirectInput8, (void**)&m_pDirectInput, NULL)))
		return false;
	//Obtain interface to keyboard
	if(FAILED(m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL)))
		return false;
	//Set the data format for the keyboard
	if(FAILED(m_pKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		return false;
	//Set the cooperative level for the keyboard
	if(FAILED(m_pKeyboard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return false;
	//Acquire the keyboard
	if(m_pKeyboard)
		m_pKeyboard->Acquire();

    // Obtain an interface to the system mouse device.
    if( FAILED(m_pDirectInput->CreateDevice( GUID_SysMouse, &m_pMouse, NULL ) ) )
        return false;
    // Set the data format for the mouse
    if( FAILED(m_pMouse->SetDataFormat( &c_dfDIMouse2 ) ) )
        return false;
    // Set the cooperative level on the mouse
    if( FAILED(m_pMouse->SetCooperativeLevel( hWnd, 
                                            DISCL_EXCLUSIVE | 
                                            DISCL_FOREGROUND ) ) )
        return false;

	//Initial Values
	Update();

	return true;
}

//-------------------------------------------------------------------------------
void MouseKeyInput::Release()
{
	if (m_pKeyboard)
	{
        m_pKeyboard->Unacquire();
	}

    SafeRelease(m_pKeyboard);
	SafeRelease(m_pMouse);
    SafeRelease(m_pDirectInput);
}

//-------------------------------------------------------------------------------
bool MouseKeyInput::Update()
{
	memcpy(m_OldKeys, m_Keys, sizeof(m_Keys));

	if (FAILED(m_pKeyboard->GetDeviceState( sizeof(char)*256, &m_Keys )))
	{
		ZeroMemory(m_pKeyboard, sizeof(m_pKeyboard));

		if (FAILED(m_pKeyboard->Acquire()))
		{
			return false;
		}
	}

	if (FAILED(m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (void**)&m_MouseInput)))
    {
		ZeroMemory(&m_MouseInput, sizeof(m_MouseInput));

        if (FAILED(m_pMouse->Acquire()))
		{
			return false;
		}
    }

	m_Keys[0x70]	= m_MouseInput.rgbButtons[0];
	m_Keys[0x73]	= m_MouseInput.rgbButtons[1];

	xamt = m_MouseInput.lX;
	yamt = m_MouseInput.lY;

	return true;
}

//-------------------------------------------------------------------------------
bool MouseKeyInput::IsDown(UINT nID)
{
	if ((m_Keys[nID] & 0x80) == 0x80)
		return true;
	return false;
}

//-------------------------------------------------------------------------------
bool MouseKeyInput::IsPressed(UINT nID)
{
	if ((m_Keys[nID] & 0x80) && !(m_OldKeys[nID] & 0x80))
		return true;
	return false;
}

//-------------------------------------------------------------------------------
bool MouseKeyInput::IsReleased(UINT nID)
{
	if ((m_OldKeys[nID] & 0x80) && !(m_Keys[nID] & 0x80))
		return true;
	return false;
}

//-------------------------------------------------------------------------------
bool MouseKeyInput::IsAnyKeyPressed()
{
	for (int i=0; i<256; ++i)
		if (IsDown(i))
			return true;

	return false;
}

//-------------------------------------------------------------------------------
bool MouseKeyInput::IsAnyKeyClicked()
{
	for (int i=0; i<256; ++i)
		if (IsPressed(i))
			return true;

	return false;
}

//-------------------------------------------------------------------------------
char MouseKeyInput::GetClickedChar()
{
	HKL layout=GetKeyboardLayout(0);

	if (IsPressed(DIK_BACKSPACE))
	{
		return 8;	// backspace value
	}

	char c = 0;

	if		(IsPressed(DIK_Q))	c =  'q';
	else if (IsPressed(DIK_W))	c =  'w';
	else if (IsPressed(DIK_E))	c =  'e';
	else if (IsPressed(DIK_R))	c =  'r';
	else if (IsPressed(DIK_T))	c =  't';
	else if (IsPressed(DIK_Y))	c =  'y';
	else if (IsPressed(DIK_U))	c =  'u';
	else if (IsPressed(DIK_I))	c =  'i';
	else if (IsPressed(DIK_O))	c =  'o';
	else if (IsPressed(DIK_P))	c =  'p';
	else if (IsPressed(DIK_A))	c =  'a';
	else if (IsPressed(DIK_S))	c =  's';
	else if (IsPressed(DIK_D))	c =  'd';
	else if (IsPressed(DIK_F))	c =  'f';
	else if (IsPressed(DIK_G))	c =  'g';
	else if (IsPressed(DIK_H))	c =  'h';
	else if (IsPressed(DIK_J))	c =  'j';
	else if (IsPressed(DIK_K))	c =  'k';
	else if (IsPressed(DIK_L))	c =  'l';
	else if (IsPressed(DIK_Z))	c =  'z';
	else if (IsPressed(DIK_X))	c =  'x';
	else if (IsPressed(DIK_C))	c =  'c';
	else if (IsPressed(DIK_V))	c =  'v';
	else if (IsPressed(DIK_B))	c =  'b';
	else if (IsPressed(DIK_N))	c =  'n';
	else if (IsPressed(DIK_M))	c =  'm';
	else if (IsPressed(DIK_1))	c =  '1';
	else if (IsPressed(DIK_2))	c =  '2';
	else if (IsPressed(DIK_3))	c =  '3';
	else if (IsPressed(DIK_4))	c =  '4';
	else if (IsPressed(DIK_5))	c =  '5';
	else if (IsPressed(DIK_6))	c =  '6';
	else if (IsPressed(DIK_7))	c =  '7';
	else if (IsPressed(DIK_8))	c =  '8';
	else if (IsPressed(DIK_9))	c =  '9';
	else if (IsPressed(DIK_0))	c =  '0';

	if (c!=0 && (IsDown(DIK_LSHIFT) || IsDown(DIK_RSHIFT)) )
	{
		c -= ('a' - 'A');
	}

	// special characters
	if		(IsPressed(DIK_PERIOD))	c = '.';
	else if (IsPressed(DIK_MINUS))	c = '-';
	else if (IsPressed(DIK_EQUALS))	c = '=';
	else if (IsPressed(DIK_SPACE))	c = ' ';

	return c;
}

//-------------------------------------------------------------------------------
long MouseKeyInput::GetXAmt() 
{
	return xamt;
}

//-------------------------------------------------------------------------------
long MouseKeyInput::GetYAmt()
{
	return yamt; 
}
