//-------------------------------------------------------------------------------
template <class ObjectType>
inline void	GuiItem::RegisterReleaseCallback(ObjectType* object, typename GuiCallbackHandler<ObjectType>::FunctionHandler handler)
{
	if (m_pReleaseCallback != NULL)
	{
		UnregisterReleaseCallback();
	}

	GuiCallbackHandler<ObjectType>* objectHandler = TNew GuiCallbackHandler<ObjectType>();
	objectHandler->Setup(object, handler);

	m_pReleaseCallback = objectHandler;
}

//-------------------------------------------------------------------------------
template <class ObjectType>
inline void	GuiItem::RegisterPressCallback(ObjectType* object, typename GuiCallbackHandler<ObjectType>::FunctionHandler handler)
{
	if (m_pPressCallback != NULL)
	{
		UnregisterPressCallback();
	}

	GuiCallbackHandler<ObjectType>* objectHandler = TNew GuiCallbackHandler<ObjectType>();
	objectHandler->Setup(object, handler);

	m_pPressCallback = objectHandler;
}