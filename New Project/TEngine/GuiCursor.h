#ifndef __GUICURSOR_H__
#define __GUICURSOR_H__

//+-------------------------------------------------------
//| Windows cursor, uses direct input
//| Version 1.001
//+-------------------------------------------------------

#ifndef __TSPRITE_H__
#include "TSprite.h"
#endif

//-------------------------------------------------------------------------------
enum GuiCursor_State
{
	Cursor_Pressed,
	Cursor_Down,
	Cursor_Released,
	Cursor_Up,
};

//-------------------------------------------------------------------------------
class GuiCursor
{
public:
	GuiCursor();
	~GuiCursor();

	const TVector2&	GetPosition() const;
	void			SetPosition(const TVector2 & position);

	GuiCursor_State GetState() const;
	void			SetState(GuiCursor_State state);

	void			SetBounds(float minX, float minY, float maxX, float maxY);
	bool			LoadTexture(const char* filename, const TVector2 * dimensions = 0);
	void			Update();
	void			Render();

protected:
	GuiCursor_State m_state;

	TVector2 m_position;

	// min & max of the screen, cursor will stay inside these bounds
	TVector2 m_min;
	TVector2 m_max;

	TSprite m_sprite;
};

#endif