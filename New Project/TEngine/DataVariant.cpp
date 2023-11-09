#include "DataVariant.h"

//-------------------------------------------------------------------------------
DataVariant::DataVariant()
:	m_nData(0)
{

}

//-------------------------------------------------------------------------------
DataVariant::~DataVariant()
{

}

//-------------------------------------------------------------------------------
DataVariant::DataVariant(const char* data)
{
	strcpy_s(m_strData, 64, data);
	m_type = DATA_STRING;
}

//-------------------------------------------------------------------------------
DataVariant::DataVariant(int data)
{
	m_nData = data;
	m_type = DATA_INT;
}

//-------------------------------------------------------------------------------
DataVariant::DataVariant(float data)
{
	m_fData = data;
	m_type = DATA_FLOAT;
}

//-------------------------------------------------------------------------------
DataVariant::DataVariant(bool data)
{
	m_bData = data;
	m_type = DATA_BOOL;
}

//-------------------------------------------------------------------------------
DataVariant::VariantType DataVariant::GetType()
{
	return m_type;
}

//-------------------------------------------------------------------------------
const char* DataVariant::GetStrData() const
{
	return &m_strData[0];
}

//-------------------------------------------------------------------------------
int DataVariant::GetIntData() const
{
	return m_nData;
}

//-------------------------------------------------------------------------------
float DataVariant::GetFloatData() const
{
	return m_fData;
}

//-------------------------------------------------------------------------------
bool DataVariant::GetBoolData() const
{
	return m_bData;
}

//-------------------------------------------------------------------------------
void DataVariant::Print(std::string & buffer) const
{
	static char data[64];

	if (m_type == DATA_STRING)
	{
		buffer.append(m_strData);
	}
	else if (m_type == DATA_INT)
	{
		sprintf_s(data, 64, "%d", m_nData);
		buffer.append(data);
	}
	else if (m_type == DATA_FLOAT)
	{
		sprintf_s(data, 64, "%.4f", m_fData);
		buffer.append(data);
	}
	else if (m_type == DATA_BOOL)
	{
		if (m_bData == true)
		{
			buffer.append("true");
		}
		else
		{
			buffer.append("false");
		}
	}
}