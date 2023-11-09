#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

//+-------------------------------------------------------
//| Clickable UI button.
//| Version 1.001
//+-------------------------------------------------------

#ifndef __GUIITEM_H__
#include "GuiItem.h"
#endif

#ifndef __TSPRITE_H__
#include "TSprite.h"
#endif

class GuiButton : public GuiItem
{
public:
	static GuiButton* Create(std::vector<std::string>& parts);
	virtual ~GuiButton();

	bool IsSelected(const TVector2 & position) const;
	void Render();
	void SetValue(const char* value);
	void SetTexture(UINT texture);
	void SetDimensions(const TVector2 & dimensions);

protected:
	GuiButton(const char* name, const char* graphicFileName,
			float x, float y, float width, float height);

	TSprite m_sprite;
};

#endif