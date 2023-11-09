#ifndef __GUIITEM_H__
#define __GUIITEM_H__

//+---------------------------------
//| Base class for a UI item
//| Version 1.001
//+---------------------------------

#ifndef __TMathClasses_H__
#include "TMathClasses.h"
#endif

#ifndef __TSTRING_H__
#include "TString.h"
#endif

#ifndef __TMEMORY_H__
#include "TMemory.h"
#endif

#ifndef __TEVENTPARAMHANDLER_H__
#include "TEventParamHandler.h"
#endif

#ifndef __GUICALLBACKHANDLER_H__
#include "GuiCallbackHandler.h"
#endif

class GuiItem
{
public:
	GuiItem();
	virtual ~GuiItem();

	virtual void Render() = 0;
	virtual bool IsSelected(const TVector2 & position) const;
	virtual void SetValue(const char* value);

	virtual void OnPress(const TVector2& pressPosition);
	virtual void OnRelease(const TVector2& releasePosition);
	virtual void OnDrag(const TVector2& dragPosition);

	template <class ObjectType>
	inline void		RegisterPressCallback(
						ObjectType* object,
						typename GuiCallbackHandler<ObjectType>::FunctionHandler handler
						);

	template <class ObjectType>
	inline void		RegisterReleaseCallback(
						ObjectType* object,
						typename GuiCallbackHandler<ObjectType>::FunctionHandler handler
						);

	void UnregisterPressCallback();
	void UnregisterReleaseCallback();

	// member access
	const TString& GetName() const;

	void SetVisible(bool visible);
	bool IsVisible() const;

	void SetSelectable(bool selectable);
	bool IsSelectable() const;

	void SetPosition(const TVector2 & position);
	const TVector2 & GetPosition() const;

protected:
	TString m_name;

	bool m_bVisible;
	bool m_bSelectable;	// can be clicked

	TVector2 m_position;	// position

	class GuiHandlerBase* m_pPressCallback;
	class GuiHandlerBase* m_pReleaseCallback;
};

#include "GuiItem.inl"

#endif