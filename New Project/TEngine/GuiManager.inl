//-------------------------------------------------------------------------------
template <class ObjectType>
inline void	GuiManager::RegisterCallback(const TString& strItemName, 
										 GuiEvent eventType,
										 ObjectType* object,
										 typename GuiCallbackHandler<ObjectType>::FunctionHandler handler)
{
	for (unsigned int i=0; i<m_pItems.size(); ++i)
	{
		if (m_pItems[i]->GetName() == strItemName)
		{
			switch(eventType)
			{
			case Gui_Press:
				m_pItems[i]->RegisterPressCallback(object, handler);
				return;
			case Gui_Release:
				m_pItems[i]->RegisterReleaseCallback(object, handler);
				return;
			default:
				TASSERT(false, "[GuiManager::RegisterCallback] bad event type");
			}
		}
	}

	TDebugPrint(("[GuiManager::RegisterCallback] Failed to find %s", *strItemName));
}
