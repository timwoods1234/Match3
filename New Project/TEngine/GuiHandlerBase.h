#ifndef __GUIHANDLERBASE_H__
#define __GUIHANDLERBASE_H__

class GuiHandlerBase
{
public:
	virtual ~GuiHandlerBase() {}

	virtual void Execute(const class GuiItem* pItem) const = 0;

protected:
	class TEventListener* m_pObject;
};

#endif
