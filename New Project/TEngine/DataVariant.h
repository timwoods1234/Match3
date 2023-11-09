#ifndef __DATAVARIANT_H__
#define __DATAVARIANT_H__

#include <string>

class DataVariant
{
public:
	DataVariant();
	~DataVariant();

	DataVariant(const char* data);
	DataVariant(int data);
	DataVariant(float data);
	DataVariant(bool data);

	enum VariantType
	{
		DATA_STRING,
		DATA_INT,
		DATA_FLOAT,
		DATA_BOOL
	};

	VariantType GetType();

	const char* GetStrData() const;
	int			GetIntData() const;
	float		GetFloatData() const;
	bool		GetBoolData() const;

	void		Print(std::string & buffer) const;

private:
	VariantType m_type;

	union
	{
		char	m_strData[64];
		int		m_nData;
		float	m_fData;
		bool	m_bData;
	};
};

#endif
