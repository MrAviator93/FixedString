/***********************************************************************
FILE FIXEDSTRING.H CONTAINS FIXED STRING IMPLEMENTATION

FILE UTMOST REVIEW DONE ON (28.04.2020) BY ARTUR K. 
***********************************************************************/

#ifndef FIXED_STRING_H
#define FIXED_STRING_H

typedef unsigned char		byte;
typedef unsigned int		uint32;
typedef long long int		int64;
typedef unsigned long long	uint64;

//-----------------------------------------------------------------------
//FIXED STRING CLASS
//
//Stack based string class implementation.
//Note: keep uint32 everywhere, we don't want to recompile header for
//32 bit and 64 bit separately due to type changes!
//
// ****** NOTE: MAX_CHAR_COUNT also includes delimiting char.
//Compiles even on Arduino!
//-----------------------------------------------------------------------

template<uint32 FS_MAX_CHAR_COUNT>
class CFixedString
{
public:
	CFixedString() noexcept;
	CFixedString(const char* pString);
	CFixedString(const int value);

	CFixedString(const uint32 value)
	{


	}

	CFixedString(const int64 value)
	{

	}

	CFixedString(const uint64 value)
	{

	}

	CFixedString(const float value)
	{

	}

	CFixedString(const double value)
	{

	}

	explicit CFixedString(const char c, uint32 repeat) noexcept;

	// ****** TODO: To be implemented
//	CFixedString(const CFixedString& fixedString);

	// ****** TODO: To be implemented
// 	template<uint32 FS_MAX_CHAR_COUNT_STR_2>
// 	CFixedString(const CFixedString<FS_MAX_CHAR_COUNT_STR_2>& fixedString);

	~CFixedString();

	uint32 getSize() const { return m_stringSize; }
	uint32 getLength() const { return m_stringSize; }
	uint32 getReservedSize() const { return FS_MAX_CHAR_COUNT; }

	const char* data() { return m_fixedString; }
	const char* buffer() const { return m_fixedString; }

	//For Range-based for Statement
	char* begin() { return &m_fixedString[0]; }
	char* end() { return &m_fixedString[m_stringSize]; }

	void lower();
	void upper();

	CFixedString& clear();

	bool isEmpty();
	bool isNumeric();

	CFixedString<FS_MAX_CHAR_COUNT> substr(size_t pos, size_t len);

	int		asInt32() const;
	uint32	asUint32() const;
	int64	asInt64() const;
	uint64	asUint64() const;
	float	asFloat() const;
	double	asDouble() const;

private:
	void zeroMemory()
	{
		memset(m_fixedString, 0x00, FS_MAX_CHAR_COUNT);
	}

private:
	uint32 m_stringSize;
	char m_fixedString[FS_MAX_CHAR_COUNT];
};

//-----------------------------------------------------------------------
//IMPLEMENTATION
//-----------------------------------------------------------------------

template<uint32 FS_MAX_CHAR_COUNT>
inline CFixedString<FS_MAX_CHAR_COUNT>::CFixedString() noexcept
{
	this->zeroMemory();
}

template<uint32 FS_MAX_CHAR_COUNT>
CFixedString<FS_MAX_CHAR_COUNT>::CFixedString(const char* pString)
{
	this->zeroMemory();
	if (pString != nullptr)
	{
		m_stringSize = static_cast<uint32>(strlen(pString));
		if (m_stringSize < FS_MAX_CHAR_COUNT)
		{
			memcpy(m_fixedString, pString, m_stringSize);
		}
		else
		{
			memcpy(m_fixedString, pString, FS_MAX_CHAR_COUNT - 1);
			m_stringSize = FS_MAX_CHAR_COUNT - 1;
		}
	}
}

template<uint32 FS_MAX_CHAR_COUNT>
inline CFixedString<FS_MAX_CHAR_COUNT>::CFixedString(const int value)
{
	this->zeroMemory();
	_itoa(value, m_fixedString, 10);
	m_stringSize = strlen(m_fixedString) + 1;
}

template<uint32 FS_MAX_CHAR_COUNT>
inline CFixedString<FS_MAX_CHAR_COUNT>::CFixedString(const char c, uint32 repeat) noexcept
{
	this->zeroMemory();
	// ****** TODO: This requires a fix!
// 	if (repeat >= 0 && repeat < FS_MAX_CHAR_COUNT)
// 	{
// 		memcpy(m_fixedString, 'x', repeat);
// 	}
// 	else
// 	{
// 		memcpy(m_fixedString, pChar, FS_MAX_CHAR_COUNT - 1);
// 	}
}

template<uint32 FS_MAX_CHAR_COUNT>
inline CFixedString<FS_MAX_CHAR_COUNT>::~CFixedString()
{
	this->zeroMemory();
}

template<uint32 FS_MAX_CHAR_COUNT>
inline void CFixedString<FS_MAX_CHAR_COUNT>::lower()
{
	char* pStrBuff = m_fixedString;
	while (*pStrBuff)
	{
		*pStrBuff = tolower(*pStrBuff);
		*pStrBuff++;
	}
}

template<uint32 FS_MAX_CHAR_COUNT>
inline void CFixedString<FS_MAX_CHAR_COUNT>::upper()
{
	char* pStrBuff = m_fixedString;
	while (*pStrBuff)
	{
		*pStrBuff = toupper(*pStrBuff);
		*pStrBuff++;
	}
}

template<uint32 FS_MAX_CHAR_COUNT>
inline CFixedString<FS_MAX_CHAR_COUNT>& CFixedString<FS_MAX_CHAR_COUNT>::clear()
{
	this->zeroMemory();
	return *this;
}

template<uint32 FS_MAX_CHAR_COUNT>
bool CFixedString<FS_MAX_CHAR_COUNT>::isEmpty()
{
	if (m_fixedString[0] == 0x00)
	{
		return true;
	}
	return false;
}

template<uint32 FS_MAX_CHAR_COUNT>
bool CFixedString<FS_MAX_CHAR_COUNT>::isNumeric()
{
	if (strcmp(m_fixedString, "0") == 0 || strcmp(m_fixedString, "0.0") == 0)
	{
		return true;
	}
	else
	{
		if (this->asDouble() == 0)
		{
			return false;
		}
	}
	return true;
}

template<uint32 FS_MAX_CHAR_COUNT>
inline CFixedString<FS_MAX_CHAR_COUNT> CFixedString<FS_MAX_CHAR_COUNT>::substr(size_t pos, size_t len)
{
	if ((pos + len) < FS_MAX_CHAR_COUNT)
	{
		// ****** TODO: Needs Implementation
	}

	return *this;
}

template<uint32 FS_MAX_CHAR_COUNT>
int CFixedString<FS_MAX_CHAR_COUNT>::asInt32() const
{
	return static_cast<int>(strtod(m_fixedString, 0));
}

template<uint32 FS_MAX_CHAR_COUNT>
uint32 CFixedString<FS_MAX_CHAR_COUNT>::asUint32() const
{
	return static_cast<uint32>(strtod(m_fixedString, 0));
}

template<uint32 FS_MAX_CHAR_COUNT>
int64 CFixedString<FS_MAX_CHAR_COUNT>::asInt64() const
{
	return static_cast<int64>(strtod(m_fixedString, 0));
}

template<uint32 FS_MAX_CHAR_COUNT>
uint64 CFixedString<FS_MAX_CHAR_COUNT>::asUint64() const
{
	return static_cast<uint64>(strtod(m_fixedString, 0));
}

template<uint32 FS_MAX_CHAR_COUNT>
float CFixedString<FS_MAX_CHAR_COUNT>::asFloat() const
{
	return static_cast<float>(strtod(m_fixedString, 0));
}

template<uint32 FS_MAX_CHAR_COUNT>
double CFixedString<FS_MAX_CHAR_COUNT>::asDouble() const
{
	return static_cast<double>(strtod(m_fixedString, 0));
}

using CFixedString16 = CFixedString<16>;
using CFixedString32 = CFixedString<32>;
using CFixedString64 = CFixedString<64>;

#endif FIXED_STRING_H
