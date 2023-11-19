#pragma once
namespace DTST
{
	enum CDATA_STATUS
	{
		CDATA_NOT_ALLOWED,
		CDATA_ALLOWED,
		CDATA_ONLY
	};
	
	bool CHECK_CDATA_STATUS(unsigned char in);

	enum ENCODING
	{
		UTF_8, 
		UTF_16,
		ISO_10646_UCS_2,
		ISO_10646_UCS_4,
		ISO_8859_1,
		ISO_8859_2,
		ISO_8859_3,
		ISO_8859_4,
		ISO_8859_5,
		ISO_8859_6,
		ISO_8859_7,
		ISO_8859_8,
		ISO_8859_9,
		ISO_2022_JP,
		Shift_JIS,
		EUC_JP
	};

	bool CHECK_ENCODING(unsigned char in);

	enum CLASS_NAMES
	{
		A_XML_EL,
		A_BASE_XML_EL,
		XML_DTD,
		A_XML_MOVEABLE,
		A_XML_TREE_EL,
		A_XML_NON_TREE_EL,
		XML_TAG,
		XML_TEXT,
		XML_PI,
		XML_COMM,
		XML_DOC,

		XML_CLASS_ERROR = 255
	};
	
}