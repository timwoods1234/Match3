#ifndef __GUITEXT_H__
#define __GUITEXT_H__

//+-------------------------------------------------------
//| UI text class.
//| Font should also be read from file (in UI Manager).
//| Version 1.000
//+-------------------------------------------------------

#include <string>

#ifndef __GUIITEM_H__
#include "GuiItem.h"
#endif

class GuiText : public GuiItem
{
public:
	static GuiText* Create(std::vector<std::string>& parts);
	~GuiText();

	void SetValue(const char* value);
	void SetValue(int value);
	void SetValue(float value);
	void Render();

protected:
	GuiText(const char* name,
			const char* value,
			float x, float y,
			UCHAR r, UCHAR g, UCHAR b);

	std::string		m_value;
	UCHAR			m_r, m_g, m_b;
};

#endif