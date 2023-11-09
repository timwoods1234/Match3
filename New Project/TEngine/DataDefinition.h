#ifndef __DATADEFINITION_H__
#define __DATADEFINITION_H__

//+----------------------------------------
//| Macro definitions.
//| Used to simplify class definitions and accessors.
//| Version 1.001
//+----------------------------------------

#include <vector>

#ifndef __TERROR_H__
#include "TError.h"
#endif

#ifndef __TMEMORY_H__
#include "TMemory.h"
#endif

#ifndef __TSTRING_H__
#include "TString.h"
#endif

// define a named object, with static std::vector
// access using name or index
// add & clear methods included
#define DECLARE_DEFINITION(className) \
	private: \
		static std::vector<className*> ms_pDefinitions; \
		TString m_strName; \
	public: \
		static void AddDefinition(className* pNewInstance); \
		static void Clear(); \
		static unsigned int GetNumDefinitions(); \
		static className* GetDefinition(unsigned int nIndex); \
		static className* GetDefinition(const TString& name); \
		const char* GetName() const;

// implementation
#define DEFINE_DEFINITION(className) \
	std::vector<className*> className::ms_pDefinitions; \
	void className::AddDefinition(className* pNewInstance) \
	{ \
		ms_pDefinitions.push_back(pNewInstance); \
	} \
	void className::Clear() \
	{ \
		for (unsigned int i=0; i<ms_pDefinitions.size(); ++i) \
		{ \
			TSafeDelete(ms_pDefinitions[i]); \
		} \
		ms_pDefinitions.clear(); \
	} \
	unsigned int className::GetNumDefinitions() \
	{ \
		return ms_pDefinitions.size(); \
	} \
	className* className::GetDefinition(unsigned int nIndex) \
	{ \
		if (nIndex < GetNumDefinitions()) \
		{ \
			return ms_pDefinitions[nIndex]; \
		} \
		TDebugPrint(("[DataDefinition::GetDefinition] Invalid Index %d (%s, %s)", nIndex, __FILE__, __LINE__)); \
		return NULL; \
	} \
	className* className::GetDefinition(const TString& name) \
	{ \
		for (unsigned int i=0; i<ms_pDefinitions.size(); ++i) \
		{ \
			if (ms_pDefinitions[i]->m_strName == name) \
			{ \
				return ms_pDefinitions[i]; \
			} \
		} \
		if (name != "None") \
			TDebugPrint(("[DataDefinition::GetDefinition] %s Not Found (%s, %s)", *name, __FILE__, __LINE__)); \
		return NULL; \
	} \
	const char* className::GetName() const \
	{ \
		return *m_strName; \
	}

// expose a simple type
// get & set by value
#define EXPOSE_VAR(type, var, getfunc, setfunc) \
	protected: \
		type var; \
	public: \
		type getfunc() const { return var; } \
		void setfunc(type NewVar) { var = NewVar; }

// expose a simple type, get function only
#define EXPOSE_VAR_READONLY(type, var, getfunc) \
	protected: \
		type var; \
	public: \
		type getfunc() const { return var; }

// expose a string, return const char*
#define EXPOSE_STRING(name, getfunc) \
	protected: \
		TString name; \
	public: \
		const char* getfunc() const { return *name; }

#endif
