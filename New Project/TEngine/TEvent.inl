//-------------------------------------------------------------------------------
template <class ObjectType>
inline void	TEvent::Register(ObjectType* object, typename TEventHandler<ObjectType>::Handler handler)
{
	TEventHandler<ObjectType>* objectHandler = TNew TEventHandler<ObjectType>();
	objectHandler->Setup(object, handler);

	m_pListeners.push_back(objectHandler);
}

//-------------------------------------------------------------------------------
inline void	TEvent::Clear()
{
	for (unsigned int i=0; i<m_pListeners.size(); ++i)
	{
		TSafeDelete(m_pListeners[i]);
	}

	m_pListeners.clear();
}

//-------------------------------------------------------------------------------
template <class ObjectType>
inline void	TEvent::Unregister(ObjectType* object, typename TEventHandler<ObjectType>::Handler handler)
{
	for (unsigned int i=0; i<m_pListeners.size(); ++i)
	{
		TEventHandlerBase* ph = m_pListeners[i];
		TEventListener* pObj = ph->GetListener();
		if (pObj == object)
		{
			TEventHandler<ObjectType>* handlerType = 
				static_cast<TEventHandler<ObjectType>*>(m_pListeners[i]);

			if (handlerType->GetHandler() == handler)
			{
				TSafeDelete(m_pListeners[i]);
				m_pListeners.erase(m_pListeners.begin() + i);
			}
		}
	}
}
