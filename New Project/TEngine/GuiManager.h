#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

//+---------------------------------
//| Container for UI Items
//| Note: This is NOT a singleton
//| Version 1.001
//+---------------------------------

#ifndef _VECTOR_
#include <vector>
#endif

#ifndef __TSTRING_H__
#include "TString.h"
#endif

#ifndef __GUIITEM_H__
#include "GuiItem.h"
#endif

#ifndef __TGENERALLIB_H__
#include "TGeneralLib.h"
#endif

#ifndef __TERROR_H__
#include "TError.h"
#endif

//-------------------------------------------------------------------------------
enum GuiEvent
{
	Gui_Press,
	Gui_Release,
};

//-------------------------------------------------------------------------------
class GuiManager
{
public:
	GuiManager();
	~GuiManager();

	bool LoadFile(const char* filename);
	void Render();
	void Clear();
	void AddItem(GuiItem * pItem);
	void RemoveItem(const TString& name);
	void Update(const class GuiCursor& cursor);

	// button access
	GuiItem * GetSelectedItem(const TVector2 & position) const;
	GuiItem * GetButtonByName(const TString& name) const;
	void SetPosition(const TString& name, const TVector2 & position);
	void SetValue(const TString& name, const char *format, ...);
	void SetVisible(const TString& name, bool b);

	// set callback on a button
	template <class ObjectType>
	inline void		RegisterCallback(
						const TString& strItemName,
						GuiEvent eventType,
						ObjectType* object,
						typename GuiCallbackHandler<ObjectType>::FunctionHandler handler
						);

protected:

	std::vector<GuiItem*> m_pItems;
};

#include "GuiManager.inl"

#endif
