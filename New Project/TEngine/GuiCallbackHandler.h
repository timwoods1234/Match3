#ifndef __GUICALLBACKHANDLER_H__
#define __GUICALLBACKHANDLER_H__

#ifndef __GUIHANDLERBASE_H__
#include "GuiHandlerBase.h"
#endif

template <class Type>
class GuiCallbackHandler : public GuiHandlerBase
{
public:
	typedef void (Type::*FunctionHandler)(const class GuiItem* pItem);

	~GuiCallbackHandler() {}

	inline void Setup(TEventListener* pObject, FunctionHandler handler);

	void Execute(const class GuiItem* pItem) const;

protected:
	FunctionHandler m_pFunction;
};

//-------------------------------------------------------------------------------
template <class Type>
inline void GuiCallbackHandler<Type>::Setup(TEventListener* pObject, FunctionHandler handler)
{
	m_pObject = pObject;
	m_pFunction = handler;
}

//-------------------------------------------------------------------------------
template <class Type>
void GuiCallbackHandler<Type>::Execute(const class GuiItem* pItem) const
{
	Type* pInstance = static_cast<Type*>(m_pObject);
	(pInstance->*m_pFunction)(pItem);
}

#endif
