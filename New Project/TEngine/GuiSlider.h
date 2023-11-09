#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

//+-------------------------------------------------------
//| UI Slider
//| Version 1.000
//+-------------------------------------------------------

#ifndef __GUIBUTTON_H__
#include "GuiButton.h"
#endif

class GuiSlider : public GuiButton
{
public:
	static GuiSlider* Create(std::vector<std::string>& parts);
	~GuiSlider();

	void Render();

	// calculate and set the slider's position, then GuiItem::OnPress
	void OnPress(const TVector2& pressPosition);
	void OnDrag(const TVector2& dragPosition);

	void SetValue(const char* value);

	void  SetSliderValue(float fValue);
	float GetSliderValue() const;

private:
	GuiSlider(const char* name, const char* backgroundAsset,
			  const char* sliderAsset, float x, float y,
			  float width, float height, float fInitialValue);

	TVector2 m_relativePosition;
	TSprite m_sliderSprite;
	float m_fMaxPosition;
};

#endif
