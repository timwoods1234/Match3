#ifndef __TEVENTPARAM_H__
#define __TEVENTPARAM_H__

//+----------------------------------------
//| Event that takes a single param argument.
//| Version 1.000
//+----------------------------------------

#include <vector>

#ifndef __TEVENTPARAMHANDLER_H__
#include "TEventParamHandler.h"
#endif

#ifndef __TMEMORY_H__
#include "TMemory.h"
#endif

template <typename Param>
class TEventParam
{
public:
	TEventParam();
	virtual ~TEventParam();

	inline virtual void Execute(Param data);

	template <class ObjectType>
	inline void		Register(
						ObjectType* object, 
						typename TEventParamHandler<ObjectType, Param>::Handler handler
						);

	template <class ObjectType>
	inline void		Unregister(
						ObjectType* object, 
						typename TEventParamHandler<ObjectType, Param>::Handler handler
						);

protected:
	inline void Clear();

	std::vector< TEventParamHandlerBase<Param>* > m_pListeners;
};

#include "TEventParam.inl"

#endif
