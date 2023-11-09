//-------------------------------------------------------------------------------
template <class Type>
inline void	TEventHandler<Type>::Setup(class TEventListener* pObject, Handler handler)
{
	m_pObject = pObject;
	m_pHandler = handler;
}

//-------------------------------------------------------------------------------
template <class Type>
void TEventHandler<Type>::Execute()
{
	Type* instancePtr = static_cast<Type*>(m_pObject);
	(instancePtr->*m_pHandler)();
}

//-------------------------------------------------------------------------------
template <class Type>
inline typename TEventHandler<Type>::Handler	
TEventHandler<Type>::GetHandler() const
{
	return m_pHandler;
}
