#ifndef _SINGLETON_H_
#define _SINGLETON_H_

//+------------------------------------------------------
//| File Singleton.h
//| Base class for singletons
//| Version 1.000
//+------------------------------------------------------

template<class C>
class Singleton
{

public:
	static C* Create()
	{
		Singleton<C>::m_currentInstance = ::new C;
		return Singleton<C>::m_currentInstance;
	}
	static void Destroy()
	{
		if (Singleton<C>::m_currentInstance != NULL)
		{
			delete Singleton<C>::m_currentInstance;
			Singleton<C>::m_currentInstance = 0;
		}
	}
	static C* Get()
	{
		return m_currentInstance;
	}

protected:
	static C* m_currentInstance;
};

#define DefineSingleton(C) C* Singleton<C>::m_currentInstance = 0;

#endif
