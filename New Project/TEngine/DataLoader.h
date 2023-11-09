#ifndef __DATALOADER_H__
#define __DATALOADER_H__

//+----------------------------------------
//| Loads data from a file
//| contains a map of values held by name
//| i.e. <type> <id> <value>
//| Version 1.005
//+----------------------------------------

#include <unordered_map>

#ifndef __DATAVARIANT_H__
#include "DataVariant.h"
#endif

#ifndef __TSTRING_H__
#include "TString.h"
#endif

class DataLoader;

#ifndef __LOADERCALLBACKDEFINED__
#define __LOADERCALLBACKDEFINED__
	typedef void (* LoaderCallbackFunc)(DataLoader*);
#endif

class DataLoader
{
public:
	static bool LoadFolder(
					std::string strPath, 
					LoaderCallbackFunc pCBFunc, 
					std::string strExtension="*.txt"
					);

	DataLoader();
	~DataLoader();

	bool		LoadFile(const char* filename);

	bool		GetFloat(std::string value, float& fIn, bool bCanFail = false);
	bool		GetInt(std::string value, int& nIn, bool bCanFail = false);
	bool		GetString(std::string value, TString& strIn, bool bCanFail = false);
	bool		GetBool(std::string value, bool& bIn, bool bCanFail = false);

	const char* GetFilename() const;

	void		EnumerateValues(std::string & buffer);
private:
	TString m_strFilename;
	std::unordered_map<std::string, DataVariant> m_values;
};

#endif
