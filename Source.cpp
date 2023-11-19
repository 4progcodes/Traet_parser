#include "DTST.h"
namespace DTST
{
	SafeAvlDict<std::string, std::string> specSymbolDict()
	{
		SafeAvlDict<std::string, std::string> ret = SafeAvlDict<std::string, std::string>("#NO_TO_CDATA_KEY");
		ret.addEl("&lt;", "<");
		ret.addEl("&gt;", ">");
		ret.addEl("&amp;", "&");
		ret.addEl("&quot;", "\"");
		ret.addEl("&apos;", "'");
		return ret;
	}

	SafeAvlDict<std::string, std::string> specCodeDict()
	{
		SafeAvlDict<std::string, std::string> ret = SafeAvlDict<std::string, std::string>("#NO_TO_AMP_KEY");
		ret.addEl("<", "&lt;");
		ret.addEl(">", "&gt;");
		ret.addEl("&", "&amp;");
		ret.addEl("\"", "&quot;");
		ret.addEl("'", "&apos;");
		return ret;
	}

	SafeAvlDict<unsigned short, std::string> codeToEncodingDict()
	{
		SafeAvlDict<unsigned short, std::string> ret("#NO_TO_ENCODING_KEY");
		ret.addEl(UTF_8, "UTF-8");
		ret.addEl(UTF_16, "UTF-16");
		ret.addEl(ISO_10646_UCS_2, "ISO-10646-UCS-2");
		ret.addEl(ISO_10646_UCS_4, "ISO-10646-UCS-4");
		ret.addEl(ISO_8859_1, "ISO-8859-1");
		ret.addEl(ISO_8859_2, "ISO-8859-2");
		ret.addEl(ISO_8859_3, "ISO-8859-3");
		ret.addEl(ISO_8859_4, "ISO-8859-4");
		ret.addEl(ISO_8859_5, "ISO-8859-5");
		ret.addEl(ISO_8859_6, "ISO-8859-6");
		ret.addEl(ISO_8859_7, "ISO-8859-7");
		ret.addEl(ISO_8859_8, "ISO-8859-8");
		ret.addEl(ISO_8859_9, "ISO-8859-9");
		ret.addEl(ISO_2022_JP, "ISO-2022-JP");
		ret.addEl(Shift_JIS, "Shift_JIS");
		ret.addEl(EUC_JP, "EUC-JP");	
		return ret;
	}
	SafeAvlDict<std::string, unsigned short> encodingToCodeDict()
	{
		SafeAvlDict<std::string, unsigned short> ret(255);
		ret.addEl("UTF-8", UTF_8);
		ret.addEl("UTF-16", UTF_16);
		ret.addEl("ISO-10646-UCS-2", ISO_10646_UCS_2);
		ret.addEl("ISO-10646-UCS-4", ISO_10646_UCS_4);
		ret.addEl("ISO-8859-1", ISO_8859_1);
		ret.addEl("ISO-8859-2", ISO_8859_2);
		ret.addEl("ISO-8859-3", ISO_8859_3);
		ret.addEl("ISO-8859-4", ISO_8859_4);
		ret.addEl("ISO-8859-5", ISO_8859_5);
		ret.addEl("ISO-8859-6", ISO_8859_6);
		ret.addEl("ISO-8859-7", ISO_8859_7);
		ret.addEl("ISO-8859-8", ISO_8859_8);
		ret.addEl("ISO-8859-9", ISO_8859_9);
		ret.addEl("ISO-2022-JP", ISO_2022_JP);
		ret.addEl("Shift-JIS", Shift_JIS);
		ret.addEl("EUC-JP", EUC_JP);
		return ret;
	}
}
