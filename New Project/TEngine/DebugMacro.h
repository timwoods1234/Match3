#include "Locator.h"
#include "MouseKeyInput.h"

//+----------------------------------------
//| Debugging Macros.
//| Version 1.000
//+----------------------------------------

//-------------------------------------------------------------------------------
// control a variable with keyboard input
#define TKEYCONTROL(type, f, up, dn, inc)	\
	static type f = 0; \
	if		(Locator::GetInput()->IsPressed(up)) { f += inc; } \
	else if (Locator::GetInput()->IsPressed(dn)) { f -= inc; }
