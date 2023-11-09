//-------------------------------------------------------------------------------
template <typename Param>
inline TEventParam<Param>::TEventParam()
{

}

//-------------------------------------------------------------------------------
template <typename Param>
inline TEventParam<Param>::~TEventParam()
{
	Clear();
}

//-------------------------------------------------------------------------------
template <typename Param>
inline void TEventParam<Param>::Clear()
{
	for (unsigned int i=0; i<m_pListeners.size(); ++i)
	{
		TSafeDelete(m_pListeners[i]);
	}

	m_pListeners.clear();
}

//-------------------------------------------------------------------------------
template <typename Param>
inline void	TEventParam<Param>::Execute(Param data)
{
	for (unsigned int i=0; i<m_pListeners.size(); ++i)
	{
		m_pListeners[i]->Execute(data);
	}
}

//-------------------------------------------------------------------------------
template <typename Param>
template <class ObjectType>
inline void	TEventParam<Param>::Register(ObjectType* object, typename TEventParamHandler<ObjectType, Param>::Handler handler)
{
	TEventParamHandler<ObjectType, Param>* objectHandler = TNew TEventParamHandler<ObjectType, Param>();
	objectHandler->Setup(object, handler);

	m_pListeners.push_back(objectHandler);
}

//-------------------------------------------------------------------------------
template <typename Param>
template <class ObjectType>
inline void	TEventParam<Param>::Unregister(ObjectType* object, typename TEventParamHandler<ObjectType, Param>::Handler handler)
{
	for (unsigned int i=0; i<m_pListeners.size(); ++i)
	{
		TEventParamHandlerBase<Param>* ph = m_pListeners[i];
		TEventListener* pObj = ph->GetListener();
		if (pObj == object)
		{
			TEventParamHandler<ObjectType, Param>* handlerType = 
				static_cast<TEventParamHandler<ObjectType, Param>*>(m_pListeners[i]);

			if (handlerType->GetHandler() == handler)
			{
				TSafeDelete(m_pListeners[i]);
				m_pListeners.erase(m_pListeners.begin() + i);
			}
		}
	}
}
