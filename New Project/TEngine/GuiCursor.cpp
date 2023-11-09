#include "GuiCursor.h"
#include "Locator.h"
#include "MouseKeyInput.h"

//-------------------------------------------------------------------------------
GuiCursor::GuiCursor()
:	m_position(100.f, 100.f),
	m_min(0.f,0.f),
	m_max(0.f,0.f)
{
	m_state = Cursor_Up;
}

//-------------------------------------------------------------------------------
GuiCursor::~GuiCursor()
{

}

//-------------------------------------------------------------------------------
void GuiCursor::Render()
{
	m_sprite.Render(m_position);
}

//-------------------------------------------------------------------------------
void GuiCursor::Update()
{
	POINT point;
	GetCursorPos(&point);
	m_position.x = (float)point.x;
	m_position.y = (float)point.y;

	m_position.x = (m_position.x < m_min.x) ? m_min.x : m_position.x;
	m_position.x = (m_position.x > m_max.x) ? m_max.x : m_position.x;
	m_position.y = (m_position.y < m_min.y) ? m_min.y : m_position.y;
	m_position.y = (m_position.y > m_max.y) ? m_max.y : m_position.y;

	if (Locator::GetInput()->IsPressed(MOUSE_INPUT_LEFT))
	{
		m_state = Cursor_Pressed;
	}
	else if (Locator::GetInput()->IsDown(MOUSE_INPUT_LEFT))
	{
		m_state = Cursor_Down;
	}
	else if (Locator::GetInput()->IsReleased(MOUSE_INPUT_LEFT))
	{
		m_state = Cursor_Released;
	}
	else
	{
		m_state = Cursor_Up;
	}
}

//-------------------------------------------------------------------------------
bool GuiCursor::LoadTexture(const char* filename, const TVector2 * dimensions)
{
	return m_sprite.LoadTexture(filename, dimensions);
}

//-------------------------------------------------------------------------------
const TVector2& GuiCursor::GetPosition() const 
{
	return m_position; 
}

//-------------------------------------------------------------------------------
void GuiCursor::SetPosition(const TVector2 & position)
{
	m_position = position;
}

//-------------------------------------------------------------------------------
void GuiCursor::SetBounds(float minX, float minY, float maxX, float maxY)
{
	m_min.x = minX;
	m_min.y = minY;
	m_max.x = maxX;
	m_max.y = maxY;
}

//-------------------------------------------------------------------------------
GuiCursor_State GuiCursor::GetState() const
{
	return m_state;
}

//-------------------------------------------------------------------------------
void GuiCursor::SetState(GuiCursor_State state)
{
	m_state = state;
}
