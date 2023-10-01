#include "DTST.h"
namespace DTST
{
	AVLDict<std::string, std::string> SpecSymbolDict()
	{
		AVLDict<std::string, std::string> ret = AVLDict<std::string, std::string>();
		ret.addEl("&lt", "<");
		ret.addEl("&gt", ">");
		ret.addEl("&amp", "&");
		ret.addEl("&quot", "\"");
		ret.addEl("&apos", "'");
		return ret;
	}
	AVLDict<std::string, std::string> SpecCodeDict()
	{
		AVLDict<std::string, std::string> ret = AVLDict<std::string, std::string>();
		ret.addEl("<", "&lt");
		ret.addEl(">", "&gt");
		ret.addEl("&", "&amp");
		ret.addEl("\"", "&quot");
		ret.addEl("'", "&apos");
		return ret;
	}
}
