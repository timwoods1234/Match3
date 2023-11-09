#pragma warning (disable : 4244) // converting atof (double) to float

#include "TTexture.h"
#include "IRenderer.h"
#include "Locator.h"
#include "GuiButton.h"
#include "TMathClasses.h"

//-------------------------------------------------------------------------------
GuiButton* GuiButton::Create(std::vector<std::string> &parts)
{
	if (parts.size() != 7)
	{
		return NULL;
	}

	GuiButton *pButton = TNew GuiButton(
								parts[1].c_str(),		// name
								parts[2].c_str(),		// art
								atof(parts[3].c_str()),	// position
								atof(parts[4].c_str()),	
								atof(parts[5].c_str()),	// dimensions
								atof(parts[6].c_str()));
	return pButton;
}

//-------------------------------------------------------------------------------
GuiButton::GuiButton(const char* name,
		const char* graphicFileName,
		float x, float y, float width, float height)
{
	m_name = name;
	m_position.x = x;
	m_position.y = y;
	m_sprite.LoadTexture(graphicFileName, &TVector2(width,height));
}

//-------------------------------------------------------------------------------
GuiButton::~GuiButton()
{

}

//-------------------------------------------------------------------------------
void GuiButton::SetValue(const char* value)
{
	m_sprite.LoadTexture(value);
}

//-------------------------------------------------------------------------------
bool GuiButton::IsSelected(const TVector2 & position) const
{
	if (!m_bSelectable || !m_bVisible)
	{
		return false;
	}

	if (position.x > m_position.x &&
		position.x < m_position.x + m_sprite.GetDimensions().x &&
		position.y > m_position.y &&
		position.y < m_position.y + m_sprite.GetDimensions().y )
	{
		return true;
	}

	return false;
}

//-------------------------------------------------------------------------------
void GuiButton::Render()
{
	if (m_bVisible)
	{
		m_sprite.Render(m_position);
	}
}

//-------------------------------------------------------------------------------
void GuiButton::SetTexture(UINT texture)
{
	m_sprite.SetTexture(texture);
}

//-------------------------------------------------------------------------------
void GuiButton::SetDimensions(const TVector2 & dimensions)
{
	m_sprite.SetDimensions(dimensions);
}
