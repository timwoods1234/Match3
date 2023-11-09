//-------------------------------------------------------------------------------
template <class Type, typename Param>
inline void	TEventParamHandler<Type, Param>::Setup(TEventListener* object, Handler handler)
{
	TEventParamHandlerBase<Param>::m_pObject = object;
	m_pHandler = handler;
}

//-------------------------------------------------------------------------------
template <class Type, typename Param>
void TEventParamHandler<Type, Param>::Execute(Param data)
{
	Type* instancePtr = static_cast<Type*>(TEventParamHandlerBase<Param>::m_pObject);
	(instancePtr->*m_pHandler)(data);
}

/***********************************************************************************************************/

template <class Type, typename Param>
inline typename TEventParamHandler<Type, Param>::Handler	
TEventParamHandler<Type, Param>::GetHandler() const
{
	return m_pHandler;
}

/***********************************************************************************************************/