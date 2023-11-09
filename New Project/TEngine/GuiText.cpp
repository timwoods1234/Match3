#pragma warning (disable : 4244) // converting atof (double) to float

#include "Locator.h"
#include "ITextPrinter.h"
#include "GuiText.h"
#include "EngineGlobals.h"

//-------------------------------------------------------------------------------
GuiText* GuiText::Create(std::vector<std::string>& parts)
{
	if (parts.size() != 8)
	{
		return NULL;
	}

	GuiText * pText = TNew GuiText(
						parts[1].c_str(),		// name
						parts[2].c_str(),		// value
						atof(parts[3].c_str()),	// position
						atof(parts[4].c_str()),
						atoi(parts[5].c_str()),	// color (rgb)
						atoi(parts[6].c_str()),
						atoi(parts[7].c_str()));
	return pText;
}

//-------------------------------------------------------------------------------
GuiText::GuiText(const char *name, 
				  const char* value,
				  float x, float y,
				  UCHAR r, UCHAR g, UCHAR b)
{
	m_name = name;
	SetValue(value);

	m_position.x = x;
	m_position.y = y;
	m_r = r;
	m_g = g;
	m_b = b;
}

//-------------------------------------------------------------------------------
GuiText::~GuiText()
{

}

//-------------------------------------------------------------------------------
void GuiText::SetValue(const char* value)
{
	m_value = value;
}

//-------------------------------------------------------------------------------
void GuiText::SetValue(int value)
{
	static char buffer[16];
	sprintf_s(buffer, 16, "%d", value);

	SetValue(buffer);
}

//-------------------------------------------------------------------------------
void GuiText::SetValue(float value)
{
	static char buffer[16];
	sprintf_s(buffer, 16, "%f", value);

	SetValue(buffer);
}

//-------------------------------------------------------------------------------
void GuiText::Render()
{
	if (m_bVisible)
	{
		Locator::GetTextPrinter()->DrawText(
			g_nGameFont, (int)m_position.x, (int)m_position.y,
			m_r, m_g, m_b, m_value.c_str());
	}
}
