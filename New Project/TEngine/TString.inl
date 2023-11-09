//-------------------------------------------------------------------------------
inline const char* TString::operator *() const
{
	return m_strData.c_str();
}

//-------------------------------------------------------------------------------
inline bool TString::operator==(const TString& other) const
{
	return (m_nStringHash == other.m_nStringHash);
}

//-------------------------------------------------------------------------------
inline bool TString::operator==(const char* data) const
{
	return (m_strData.compare(data) == 0);
}

//-------------------------------------------------------------------------------
inline TString& TString::operator=(const char* data)
{
	m_strData = data;
	m_nStringHash = GetStringHash(data);

	return *this;
}

//-------------------------------------------------------------------------------
inline TString& TString::operator=(const TString& other)
{
	m_strData = other.m_strData;
	m_nStringHash = other.m_nStringHash;

	return *this;
}

//-------------------------------------------------------------------------------
inline TString& TString::operator+=(const char* data)
{
	m_strData.append(data);
	m_nStringHash = GetStringHash(m_strData.c_str());

	return *this;
}

//-------------------------------------------------------------------------------
inline TString& TString::operator+=(const TString& other)
{
	m_strData.append(other.m_strData);
	m_nStringHash = GetStringHash(m_strData.c_str());

	return *this;
}

//-------------------------------------------------------------------------------
inline const TString TString::operator+(const TString& other)
{
	TString result = *this;
	result += other;
	return result;
}

//-------------------------------------------------------------------------------
inline const TString TString::operator+(const char* data)
{
	TString result = *this;
	result += data;
	return result;
}

//-------------------------------------------------------------------------------
inline bool TString::operator!=(const TString& other) const
{
	return !(*this == other);
}

//-------------------------------------------------------------------------------
inline bool TString::operator!=(const char* data) const
{
	return !(*this == data);
}

//-------------------------------------------------------------------------------
inline bool TString::IsEmpty() const
{
	return (m_strData.size() == 0);
}
