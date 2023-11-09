#include <vector>

#include "DataLoader.h"
#include "TGeneralLib.h"
#include "TError.h"

//-------------------------------------------------------------------------------
DataLoader::DataLoader()
{

}

//-------------------------------------------------------------------------------
DataLoader::~DataLoader()
{

}

//-------------------------------------------------------------------------------
bool DataLoader::LoadFile(const char* filename)
{
	// open file
	FILE* f;
	fopen_s(&f, filename, "r");

	if (!f)
	{
		TDebugPrint(("[DataLoader::LoadFile] Couldn't open file %s", filename));
		return false;
	}

	// read lines until EOF
	char str[256];
	unsigned int nLine = 0;

	while (fgets(str, 256, f))
	{
		// fetch next line
		++nLine;
		std::string string = str;		// convert to std::string (easier)
		string.erase( string.end()-1 );	// cut newline
		std::vector<std::string> parts;	// split into parts
		StringTokenize(string, parts, " ");

		if (!(parts.size() == 3))
		{
			TDebugPrint(("[DataLoader::LoadFile] %s:%d bad format", filename, nLine));
		}

		// add value
		if (parts[0] == "int")
		{
			int data = atoi(parts[2].c_str());
			DataVariant variant(data);
			m_values[parts[1]] = variant;
		}

		else if (parts[0] == "float")
		{
			float data = (float)atof(parts[2].c_str());
			DataVariant variant(data);
			m_values[parts[1]] = variant;
		}

		else if (parts[0] == "string")
		{
			DataVariant variant(parts[2].c_str());
			m_values[parts[1]] = variant;
		}

		else if (parts[0] == "bool")
		{
			if (parts[2] == "true")
			{
				DataVariant variant(true);
				m_values[parts[1]] = variant;
			}
			else
			{
				DataVariant variant(false);
				m_values[parts[1]] = variant;
			}
		}
	}

	fclose(f);
	f = NULL;

	m_strFilename = filename;
	return true;
}

//-------------------------------------------------------------------------------
const char* DataLoader::GetFilename() const
{
	return *m_strFilename;
}

//-------------------------------------------------------------------------------
bool DataLoader::GetString(std::string value, TString& strIn, bool bCanFail)
{
	if((m_values[value].GetType() == DataVariant::DATA_STRING))
	{
		strIn = m_values[value].GetStrData();
		return true;
	}

	if (bCanFail)
	{
		return true;
	}
	
	TDebugPrint(("[DataLoader::GetString] String %s:%s not found", *m_strFilename, value.c_str()));
	return false;
}

//-------------------------------------------------------------------------------
bool DataLoader::GetFloat(std::string value, float& fIn, bool bCanFail)
{
	if( (m_values[value].GetType() == DataVariant::DATA_FLOAT) )
	{
		fIn = m_values[value].GetFloatData();
		return true;
	}

	if (bCanFail)
	{
		return true;
	}

	TDebugPrint(("[DataLoader::GetString] Float %s:%s not found", *m_strFilename, value.c_str()));
	fIn = 0.0f;
	return false;
}

//-------------------------------------------------------------------------------
bool DataLoader::GetInt(std::string value, int& nIn, bool bCanFail)
{
	if ( (m_values[value].GetType() == DataVariant::DATA_INT))
	{
		nIn = m_values[value].GetIntData();		
		return true;
	}

	if (bCanFail)
	{
		return true;
	}
	
	TDebugPrint(("[DataLoader::GetString] Int %s:%s not found", *m_strFilename, value.c_str()));
	nIn = 0;
	return false;
}

//-------------------------------------------------------------------------------
bool DataLoader::GetBool(std::string value, bool& bIn, bool bCanFail)
{
	if ( (m_values[value].GetType() == DataVariant::DATA_BOOL) )
	{
		bIn = m_values[value].GetBoolData();
		return true;
	}

	if (bCanFail)
	{
		return true;
	}

	TDebugPrint(("[DataLoader::GetBool] Bool %s:%s not found", *m_strFilename, value.c_str()));
	bIn = true;
	return false;
}

//-------------------------------------------------------------------------------
void DataLoader::EnumerateValues(std::string & buffer)
{
	buffer.append("Values for ");
	buffer.append(*m_strFilename);

	std::unordered_map<std::string, DataVariant>::iterator iter;

	for (iter = m_values.begin(); iter != m_values.end(); iter++)
	{
		buffer.append("\n");
		buffer.append(iter->first);
		buffer.append(" : ");
		iter->second.Print(buffer);
	}
}

//-------------------------------------------------------------------------------
bool DataLoader::LoadFolder(std::string strPath, LoaderCallbackFunc pCBFunc, std::string strExtension)
{
	std::string strFindParam = strPath;
	strFindParam.append(strExtension);

	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA ffd;
	hFind = FindFirstFile(strFindParam.c_str(), &ffd);

	do
	{
		char filename[1024];
		sprintf_s(filename, 1024, "%s%s", strPath.c_str(), ffd.cFileName);

		DataLoader loader;
		loader.LoadFile(filename);

		pCBFunc(&loader);
	}
	while(FindNextFile(hFind, &ffd));

	return true;
}
