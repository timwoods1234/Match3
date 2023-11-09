#ifndef __TSTRING_H__
#define __TSTRING_H__

//+----------------------------------------
//| Wrapper class for std::string and its hash
//| Use the hash for fast comparison
//| Version 1.000
//+----------------------------------------

#include <vector>
#include <string>

class TString
{
public:
	TString();
	TString(const char* data);
	TString(const TString& other);

	~TString();

	inline TString& operator=(const char* data);
	inline TString& operator=(const TString& other);

	// append functions
	inline TString& operator+=(const char* data);
	inline TString& operator+=(const TString& other);
	inline const TString operator+(const TString& other);
	inline const TString operator+(const char* data);

	// de-reference to obtain char data
	inline const char* operator*() const;

	// fast, compares hashes (ints)
	inline bool operator==(const TString& other) const;
	inline bool operator!=(const TString& other) const;

	// slower, uses std::string compare
	inline bool operator==(const char* data) const;
	inline bool operator!=(const char* data) const;

	inline bool IsEmpty() const;

	// varg format function
	void Format(const char* chString, ...);

private:
	static unsigned int GetStringHash(const char* data);

	std::string m_strData;
	unsigned int m_nStringHash;
};

#include "TString.inl"

#endif
