#pragma warning (disable : 4244) // converting atof (double) to float

#include "GuiSlider.h"

//-------------------------------------------------------------------------------
GuiSlider* GuiSlider::Create(std::vector<std::string>& parts)
{
	if (parts.size() != 9)
	{
		return NULL;
	}

	GuiSlider* pSlider = TNew GuiSlider(
								parts[1].c_str(),		// name
								parts[2].c_str(),		// background art
								parts[3].c_str(),		// slider art
								atof(parts[4].c_str()),	// position
								atof(parts[5].c_str()),
								atof(parts[6].c_str()),	// dimensions
								atof(parts[7].c_str()),
								atof(parts[8].c_str())); // initial value
	return pSlider;
}

//-------------------------------------------------------------------------------
GuiSlider::GuiSlider(const char* name, const char* backgroundAsset,
					  const char* sliderAsset, float x, float y,
					  float width, float height, float fInitialValue)
:	GuiButton(name, backgroundAsset, x, y, width, height)
{
	m_sliderSprite.LoadTexture(sliderAsset);

	m_relativePosition.x = m_position.x + fInitialValue * m_sprite.GetDimensions().x;

	m_relativePosition.y = 
		m_sprite.GetDimensions().y / 2 - m_sliderSprite.GetDimensions().y / 2;

	SetSelectable(true);

	// only drag up to the end (not over it)
	m_fMaxPosition = m_sprite.GetDimensions().x - m_sliderSprite.GetDimensions().x;
}

//-------------------------------------------------------------------------------
GuiSlider::~GuiSlider()
{
}

//-------------------------------------------------------------------------------
void GuiSlider::OnDrag(const TVector2& dragPosition)
{
	OnPress(dragPosition);
}

//-------------------------------------------------------------------------------
void GuiSlider::OnPress(const TVector2& pressPosition)
{
	// calculate relative position
	m_relativePosition.x = pressPosition.x - m_position.x;

	// clamp to the end of the bar
	m_relativePosition.x = TMin(m_relativePosition.x, m_fMaxPosition);

	// on press callback
	GuiItem::OnPress(pressPosition);
}

//-------------------------------------------------------------------------------
void GuiSlider::Render()
{
	m_sprite.Render(m_position);
	m_sliderSprite.Render(m_position + m_relativePosition);
}

//-------------------------------------------------------------------------------
float GuiSlider::GetSliderValue() const
{
	float ret = m_relativePosition.x / m_fMaxPosition;
	
	if (ret < 0.05)
	{
		ret = 0.f;
	}

	return ret;
}

//-------------------------------------------------------------------------------
void GuiSlider::SetSliderValue(float fValue)
{
	m_relativePosition.x = fValue * m_fMaxPosition;
}

//-------------------------------------------------------------------------------
void GuiSlider::SetValue(const char* value)
{
	SetSliderValue( atof(value) );
}