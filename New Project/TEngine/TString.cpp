#include <cstring>
#include "TString.h"

//-------------------------------------------------------------------------------
unsigned int TString::GetStringHash( const char* data )
{
	unsigned int j   = 0;
	unsigned int r   = 0;
	unsigned int len = strlen( data );
	for( unsigned int i = 0; i < len; ++i )
	{
		unsigned int x = data[ i ] - ' ';
		j += ( ( ( i & 4 ) >> 2 ) + ( i & 1 ) ) >> 1;
		j &= 3;
        r += x << ( ( ( i & 3 ) << 3 ) + j );
	}
	return r;
}

//-------------------------------------------------------------------------------
TString::TString()
:	m_nStringHash(0)
{
	m_strData = "";
}

//-------------------------------------------------------------------------------
TString::TString(const char* data)
{
	m_strData		= data;
	m_nStringHash	= GetStringHash(data);
}

//-------------------------------------------------------------------------------
TString::TString(const TString& other)
{
	m_strData		= other.m_strData;
	m_nStringHash	= other.m_nStringHash;
}

//-------------------------------------------------------------------------------
TString::~TString()
{
}

//-------------------------------------------------------------------------------
void TString::Format(const char *chString, ...)
{
	static char	buffer[1024];
	char		*pArgs;

	pArgs = (char*) &chString + sizeof(chString);
	vsprintf_s(buffer, 1024, chString, pArgs);

	m_strData = buffer;
	m_nStringHash = GetStringHash(m_strData.c_str());
}
