#include "GuiItem.h"
#include "TMemory.h"

//-------------------------------------------------------------------------------
GuiItem::GuiItem() 
:	m_bVisible(true),
	m_bSelectable(false),
	m_pPressCallback(0),
	m_pReleaseCallback(0)
{
}

//-------------------------------------------------------------------------------
GuiItem::~GuiItem()
{
	UnregisterPressCallback();
	UnregisterReleaseCallback();
}

//-------------------------------------------------------------------------------
bool GuiItem::IsSelected(const TVector2 & position) const
{
	return false;
}

//-------------------------------------------------------------------------------
void GuiItem::SetValue(const char* value)
{
}

//-------------------------------------------------------------------------------
const TString& GuiItem::GetName() const
{
	return m_name;
}

//-------------------------------------------------------------------------------
void GuiItem::SetVisible(bool visible)
{
	m_bVisible = visible;
}

//-------------------------------------------------------------------------------
bool GuiItem::IsVisible() const
{
	return m_bVisible;
}

//-------------------------------------------------------------------------------
void GuiItem::SetSelectable(bool selectable)
{
	m_bSelectable = selectable;
}

//-------------------------------------------------------------------------------
bool GuiItem::IsSelectable() const 
{
	return m_bSelectable;
}

//-------------------------------------------------------------------------------
void GuiItem::SetPosition(const TVector2 & position)
{
	m_position = position;
}

//-------------------------------------------------------------------------------
const TVector2 & GuiItem::GetPosition() const
{
	return m_position;
}

//-------------------------------------------------------------------------------
void GuiItem::OnPress(const TVector2& pressPosition)
{
	if (m_pPressCallback != NULL)
	{
		m_pPressCallback->Execute(this);
	}
}

//-------------------------------------------------------------------------------
void GuiItem::OnRelease(const TVector2& releasePosition)
{
	if (m_pReleaseCallback != NULL)
	{
		m_pReleaseCallback->Execute(this);
	}
}

//-------------------------------------------------------------------------------
void GuiItem::OnDrag(const TVector2& dragPosition)
{

}

//-------------------------------------------------------------------------------
void GuiItem::UnregisterPressCallback()
{
	TSafeDelete(m_pPressCallback);
}

//-------------------------------------------------------------------------------
void GuiItem::UnregisterReleaseCallback()
{
	TSafeDelete(m_pReleaseCallback);
}