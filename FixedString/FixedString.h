/***********************************************************************
FILE FIXEDSTRING.H CONTAINS FIXED STRING IMPLEMENTATION

FILE UTMOST REVIEW DONE ON (14.05.2020) BY ARTUR K. 
***********************************************************************/

#ifndef FIXED_STRING_H
#define FIXED_STRING_H

typedef unsigned int		uint32;
typedef long long int		int64;
typedef unsigned long long	uint64;

enum class StrBaseEnum
{
	FS_DEC = 10,
	FS_HEX = 16,
	FS_BIN = 2
};

//-----------------------------------------------------------------------
// FIXED STRING CLASS
//
// Stack based string class implementation.
// Note: keep uint32 everywhere, we don't want to recompile header for
// 32 bit and 64 bit separately due to type changes!
//
// ****** NOTE: MAX_CHAR_COUNT also includes delimiting char.
// Compiles even on Arduino!
//-----------------------------------------------------------------------

template<uint32 FS_MAX_CHAR_COUNT>
class CFixedString
{
public:
	CFixedString() noexcept;
	CFixedString(const char* pString);
	CFixedString(const int value);
	explicit CFixedString(const int value, StrBaseEnum type);

	// ****** TODO: Constructors to consider to implement
// 	CFixedString(const uint32 value);
// 	CFixedString(const int64 value);
// 	CFixedString(const uint64 value)
// 	CFixedString(const float value)
// 	CFixedString(const double value)

	explicit CFixedString(const char singleChar, uint32 numToRepeat) noexcept;

	// ****** TODO: Consider implementing
//	CFixedString(const CFixedString& fixedString);

	// ****** TODO: Consider implementing
// 	template<uint32 FS_MAX_CHAR_COUNT_STR_2>
// 	CFixedString(const CFixedString<FS_MAX_CHAR_COUNT_STR_2>& fixedString);

	~CFixedString();

	uint32 getSize() const { return m_stringSize; }
	uint32 getLength() const { return m_stringSize; }
	uint32 getReservedSize() const { return FS_MAX_CHAR_COUNT; }

	const char* getData() { return m_fixedString; }
	const char* data() { return m_fixedString; }
	const char* buffer() const { return m_fixedString; }

	// For Range-based for Statement
	char* begin() { return &m_fixedString[0]; }
	char* end() { return &m_fixedString[m_stringSize]; }

	void lower();
	void upper();

	CFixedString& clear();

	bool isEmpty();
	bool isNumeric();
	bool isHexDecimal();
	bool isBinary(); 

	bool startsWithNoCase(const char* pString);
	bool startsWith(const char* pString);
	bool endsWith(const char* pString);
	bool contains(const char* pString);

	int countNoOccurances(const char& ch);
	int countNoOccurances(const char* pString);

	CFixedString<FS_MAX_CHAR_COUNT> substr(uint32 pos, uint32 len);

	bool append(const char ch);
	bool append(const char* pString);
	bool appendPrefix(const char* pString);

	int compare(const char* pString);
	int compareNoCase(const char* pString);

 	int find(char ch, uint32 startPos = 0);
 	int find(const char* pStrSub, uint32 startPos = 0);
 	int findLastOccurance(char ch);
	int findLastOccurance(const char* pStrSub);
 	int reverseFind(char ch);
 	int findOneOf(const char* pStrCharSet, uint32 startPos = 0);

	bool replace(uint32 pos, uint32 size, const char* pString);

 	void trimLeft(const int trimCutSet);
 	void trimRight(const int trimCutSet);
 	void trim(const int trimCutSet);

	// Assignment operators "="
	const CFixedString<FS_MAX_CHAR_COUNT>& operator=(char ch);
	const CFixedString<FS_MAX_CHAR_COUNT>& operator=(const char* pString);

	// Append operators "+="
	const CFixedString<FS_MAX_CHAR_COUNT>& operator+=(char ch);
	const CFixedString<FS_MAX_CHAR_COUNT>& operator+=(const char* pString);

	// Access by reference.
	char& operator[](int index);

	// Access by copy.
	char operator[](int index) const;

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
		m_stringSize = 0;
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

int fsipow(const int& value, const uint32& power)
{
	if (power == 0)
	{
		return 1;
	}

	if (power == 1)
	{
		return value;
	}

	int outValue = value;
	for (int i = 1; i < power ; i++)
	{
		outValue *= value;
	}

	return outValue;
}

template<uint32 FS_MAX_CHAR_COUNT>
inline CFixedString<FS_MAX_CHAR_COUNT>::CFixedString(const int value)
{
	this->zeroMemory();
	int p = fsipow(10, FS_MAX_CHAR_COUNT - 2);
	if ((float)(value / p) < 10.0f)
	{
		_itoa_s(value, m_fixedString, FS_MAX_CHAR_COUNT, 10);
		m_stringSize = strlen(m_fixedString);
	}	
}

template<uint32 FS_MAX_CHAR_COUNT>
inline CFixedString<FS_MAX_CHAR_COUNT>::CFixedString(const int value, StrBaseEnum type)
{
	this->zeroMemory();

	switch (type)
	{
	case StrBaseEnum::FS_DEC:
	{
		int p = fsipow(10, FS_MAX_CHAR_COUNT - 2);
		if ((float)(value / p) < 10.0f)
		{
			_itoa_s(value, m_fixedString, FS_MAX_CHAR_COUNT, 16);
			m_stringSize = strlen(m_fixedString);
		}
		break;
	}
	case StrBaseEnum::FS_HEX:
	{
		int p = fsipow(10, FS_MAX_CHAR_COUNT - 4); //We require 2 additional chars for "0x"
		if ((float)(value / p) < 10.0f)
		{
			m_fixedString[0] = '0';
			m_fixedString[1] = 'x';
			_itoa_s(value, &m_fixedString[2], FS_MAX_CHAR_COUNT, 16);
			m_stringSize = strlen(m_fixedString);
		}
		break;
	}
	case StrBaseEnum::FS_BIN:
	{
		int p = fsipow(10, FS_MAX_CHAR_COUNT - 4); //We require 2 additional chars for "0b"
		/*if ((float)(value / p) < 10.0f)*/  // ****** TODO:  for binary this does not work.
		{
			m_fixedString[0] = '0';
			m_fixedString[1] = 'b';
			_itoa_s(value, &m_fixedString[2], FS_MAX_CHAR_COUNT, 2);
			m_stringSize = strlen(m_fixedString);
		}
		break;
	}
	default:
		break;
	}
}

template<uint32 FS_MAX_CHAR_COUNT>
inline CFixedString<FS_MAX_CHAR_COUNT>::CFixedString(const char singleChar, uint32 numToRepeat) noexcept
{
	this->zeroMemory();
	if (numToRepeat >= 0 && numToRepeat < FS_MAX_CHAR_COUNT)
	{
		memset(m_fixedString, singleChar, numToRepeat);
		m_stringSize = numToRepeat;
	}
	else
	{
		memset(m_fixedString, singleChar, FS_MAX_CHAR_COUNT - 1);
		m_stringSize = FS_MAX_CHAR_COUNT - 1;
	}
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
inline bool CFixedString<FS_MAX_CHAR_COUNT>::isHexDecimal()
{
	if (m_stringSize > 4)
	{
		if (m_fixedString[0] == '0' && m_fixedString[1] == 'x' || m_fixedString[0] == '0' && m_fixedString[1] == 'X')
		{
			return true;
		}
	}
	return false;
}

template<uint32 FS_MAX_CHAR_COUNT>
inline bool CFixedString<FS_MAX_CHAR_COUNT>::isBinary()
{
	if (m_stringSize > 4)
	{
		if (m_fixedString[0] == '0' && m_fixedString[1] == 'b')
		{
			return true;
		}
	}
	return false;
}

template<uint32 FS_MAX_CHAR_COUNT>
inline bool CFixedString<FS_MAX_CHAR_COUNT>::startsWithNoCase(const char* pString)
{
	const uint32 stringLen = strlen(pString);
	if (stringLen > (FS_MAX_CHAR_COUNT - 1))
	{
		stringLen = FS_MAX_CHAR_COUNT - 1;
	}

	if (strnicmp(m_fixedString, pString, stringLen) == 0)
	{
		return true;
	}
	return false;
}

template<uint32 FS_MAX_CHAR_COUNT>
inline bool CFixedString<FS_MAX_CHAR_COUNT>::startsWith(const char* pString)
{
	const uint32 stringLen = strlen(pString);
	if (stringLen > (FS_MAX_CHAR_COUNT - 1))
	{
		stringLen = FS_MAX_CHAR_COUNT - 1;
	}

	if (strncmp(m_fixedString, pString,  stringLen) == 0)
	{
		return true;
	}
	return false;
}

template<uint32 FS_MAX_CHAR_COUNT>
inline bool CFixedString<FS_MAX_CHAR_COUNT>::endsWith(const char* pString)
{
	const uint32 stringLen = strlen(pString);
	if (m_stringSize > stringLen && strncmp(m_fixedString + (m_stringSize - stringLen), pString, stringLen) == 0)
	{
		return true;
	}
	return false;
}

template<uint32 FS_MAX_CHAR_COUNT>
inline bool CFixedString<FS_MAX_CHAR_COUNT>::contains(const char* pString)
{
	if (strstr(m_fixedString, pString) != nullptr)
	{
		return true;
	}
	return false;
}

template<uint32 FS_MAX_CHAR_COUNT>
inline int CFixedString<FS_MAX_CHAR_COUNT>::countNoOccurances(const char& ch)
{
	int occurances(0);
	for (uint32 i = 0; i < m_stringSize; i++)
	{
		if (m_fixedString[i] == ch)
		{
			occurances++;
		}
	}
	return occurances;
}

template<uint32 FS_MAX_CHAR_COUNT>
inline int CFixedString<FS_MAX_CHAR_COUNT>::countNoOccurances(const char* pString)
{
	int occurances(0);
	const char* pOccurance = m_fixedString;
	while (pOccurance != nullptr || (((size_t)(m_fixedString - pOccurance)) >= FS_MAX_CHAR_COUNT))
	{
		pOccurance = strstr(pOccurance, pString);
		if (pOccurance != nullptr)
		{
			*pOccurance++;
			occurances++;
		}
		else
		{
			break;
		}
	}
	return occurances;
}

template<uint32 FS_MAX_CHAR_COUNT>
inline CFixedString<FS_MAX_CHAR_COUNT> CFixedString<FS_MAX_CHAR_COUNT>::substr(uint32 pos, uint32 len)
{
	if ((pos + len) < FS_MAX_CHAR_COUNT)
	{
		CFixedString<FS_MAX_CHAR_COUNT> fixedStr;
		memcpy(fixedStr.m_fixedString, m_fixedString + pos, len);
		return fixedStr;
	}
	return *this;
}

template<uint32 FS_MAX_CHAR_COUNT>
inline bool CFixedString<FS_MAX_CHAR_COUNT>::append(const char ch)
{
	if ((m_stringSize + 1) < FS_MAX_CHAR_COUNT)
	{
		m_fixedString[m_stringSize + 1] = ch;
		m_stringSize += 1;
		return true;
	}
	return false;
}

template<uint32 FS_MAX_CHAR_COUNT>
inline bool CFixedString<FS_MAX_CHAR_COUNT>::append(const char* pString)
{
	const uint32 stringLen = strlen(pString);
	if ((m_stringSize + stringLen) < FS_MAX_CHAR_COUNT)
	{
		memcpy(m_fixedString + m_stringSize, pString, stringLen);
		m_stringSize += stringLen;
		return true;
	}
	return false;
}

template<uint32 FS_MAX_CHAR_COUNT>
inline bool CFixedString<FS_MAX_CHAR_COUNT>::appendPrefix(const char* pString)
{
	const uint32 stringLen = strlen(pString);
	if ((m_stringSize + stringLen) < FS_MAX_CHAR_COUNT)
	{
		for (int i = m_stringSize; i >= 0; i--)
		{
			m_fixedString[i + stringLen] = m_fixedString[i];
		}
		memcpy(m_fixedString, pString, stringLen);
		m_stringSize += stringLen;
		return true;
	}
	return false;
}

template<uint32 FS_MAX_CHAR_COUNT>
inline int CFixedString<FS_MAX_CHAR_COUNT>::compare(const char* pString)
{
	return strcmp(m_fixedString, pString);
}

template<uint32 FS_MAX_CHAR_COUNT>
inline int CFixedString<FS_MAX_CHAR_COUNT>::compareNoCase(const char* pString)
{
	return stricmp(m_fixedString, pString);
}

template<uint32 FS_MAX_CHAR_COUNT>
inline int CFixedString<FS_MAX_CHAR_COUNT>::find(char ch, uint32 startPos)
{
	if (startPos < m_stringSize)
	{
		const char* pFirstOccurance = strchr(m_fixedString + startPos, static_cast<int>(ch));
		if (pFirstOccurance != nullptr)
		{
			return (pFirstOccurance - m_fixedString);
		}
	}
	return -1;
}

template<uint32 FS_MAX_CHAR_COUNT>
inline int CFixedString<FS_MAX_CHAR_COUNT>::find(const char* pStrSub, uint32 startPos)
{
	if (startPos < m_stringSize)
	{
		const char* pFirstOccurance = strstr(m_fixedString + startPos, pStrSub);
		if (pFirstOccurance != nullptr)
		{
			return (pFirstOccurance - m_fixedString);
		}
	}
	return -1;
}

template<uint32 FS_MAX_CHAR_COUNT>
inline int CFixedString<FS_MAX_CHAR_COUNT>::findLastOccurance(char ch)
{
	const char* pLastOccurance = strrchr(m_fixedString, static_cast<int>(ch));
	if (pLastOccurance != nullptr)
	{
		return static_cast<int>(pLastOccurance - m_fixedString);
	}
	return -1;
}

template<uint32 FS_MAX_CHAR_COUNT>
inline int CFixedString<FS_MAX_CHAR_COUNT>::findLastOccurance(const char* pStrSub)
{
	const uint32 strSize = strlen(pStrSub);
	if (strSize < m_stringSize)
	{
		int offset = 0;
		while (offset < m_stringSize)
		{
			const char* pLastOccurance = strstr(m_fixedString + offset, pStrSub);
			if (pLastOccurance != nullptr)
			{
				offset = static_cast<int>(pLastOccurance - m_fixedString) + strSize;
			}

			if (pLastOccurance == nullptr)
			{
				break;
			}
		}
		return offset - strSize;
	}
	return -1;
}

template<uint32 FS_MAX_CHAR_COUNT>
inline int CFixedString<FS_MAX_CHAR_COUNT>::reverseFind(char ch)
{
	return this->findLastOccurance(ch);
}

template<uint32 FS_MAX_CHAR_COUNT>
inline int CFixedString<FS_MAX_CHAR_COUNT>::findOneOf(const char* pStrCharSet, uint32 startPos)
{
	const uint32 charSetLen = strlen(pStrCharSet);
	if ((charSetLen + startPos) < m_stringSize)
	{
		const char* singleInstMatch = strpbrk(m_fixedString, pStrCharSet);
		if (singleInstMatch != nullptr)
		{
			return static_cast<int>(singleInstMatch - m_fixedString);
		}
	}
	return -1;
}

template<uint32 FS_MAX_CHAR_COUNT>
inline bool CFixedString<FS_MAX_CHAR_COUNT>::replace(uint32 pos, uint32 size, const char* pString)
{
	if (pos + size < FS_MAX_CHAR_COUNT)
	{
		memcpy(m_fixedString + pos, pString, size);
		return true;
	}
	return false;
}

template<uint32 FS_MAX_CHAR_COUNT>
inline void CFixedString<FS_MAX_CHAR_COUNT>::trimLeft(const int trimCutSet)
{
	if (trimCutSet > 0 && static_cast<uint32>(trimCutSet) < m_stringSize)
	{
		for (int i = 0; i < (m_stringSize - static_cast<uint32>(trimCutSet)); i++)
		{
			m_fixedString[i] = m_fixedString[i + trimCutSet];
		}
		memset(m_fixedString + m_stringSize - trimCutSet, 0x00, trimCutSet);
		m_stringSize -= trimCutSet;
	}
}

template<uint32 FS_MAX_CHAR_COUNT>
inline void CFixedString<FS_MAX_CHAR_COUNT>::trimRight(const int trimCutSet)
{
	if (trimCutSet > 0 && static_cast<uint32>(trimCutSet) < m_stringSize)
	{
		for (int i = m_stringSize; i >= (m_stringSize - static_cast<uint32>(trimCutSet)); i--)
		{
			m_fixedString[i] = 0x00;
		}
		m_stringSize -= trimCutSet;
	}
}

template<uint32 FS_MAX_CHAR_COUNT>
inline void CFixedString<FS_MAX_CHAR_COUNT>::trim(const int trimCutSet)
{
	this->trimLeft(trimCutSet);
	this->trimRight(trimCutSet);
}

template<uint32 FS_MAX_CHAR_COUNT>
inline const CFixedString<FS_MAX_CHAR_COUNT>& CFixedString<FS_MAX_CHAR_COUNT>::operator=(char ch)
{
	this->zeroMemory();
	m_fixedString[0] = ch;
	m_stringSize = 1;
	return *this;
}

template<uint32 FS_MAX_CHAR_COUNT>
inline const CFixedString<FS_MAX_CHAR_COUNT>& CFixedString<FS_MAX_CHAR_COUNT>::operator=(const char* pString)
{
	const uint32 stringLen = strlen(pString);
	if (stringLen < FS_MAX_CHAR_COUNT)
	{
		this->zeroMemory();
		memcpy(m_fixedString, pString, stringLen);
		m_stringSize = stringLen;
	}
	else
	{
		this->zeroMemory();
		memcpy(m_fixedString, pString, FS_MAX_CHAR_COUNT - 1);
		m_stringSize = FS_MAX_CHAR_COUNT - 1;
	}
	return *this;
}

template<uint32 FS_MAX_CHAR_COUNT>
inline const CFixedString<FS_MAX_CHAR_COUNT>& CFixedString<FS_MAX_CHAR_COUNT>::operator+=(char ch)
{
	this->append(ch);
	return *this;
}

template<uint32 FS_MAX_CHAR_COUNT>
inline const CFixedString<FS_MAX_CHAR_COUNT>& CFixedString<FS_MAX_CHAR_COUNT>::operator+=(const char* pString)
{
	this->append(pString);
	return *this;
}

template<uint32 FS_MAX_CHAR_COUNT>
inline char & CFixedString<FS_MAX_CHAR_COUNT>::operator[](int index)
{
	if (m_stringSize > static_cast<size_t>(index) && index > -1)
	{
		return m_fixedString[index];
	}
	return m_fixedString[0];
}

template<uint32 FS_MAX_CHAR_COUNT>
inline char CFixedString<FS_MAX_CHAR_COUNT>::operator[](int index) const
{
	if (m_stringSize > static_cast<size_t>(index) && index > -1)
	{
		return m_fixedString[index];
	}
	return m_fixedString[0];
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
