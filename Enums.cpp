#include "Enums.h"

namespace DTST
{
	bool CHECK_CDATA_STATUS(unsigned char in)
	{
		if (in < 0 || in > 2) { return false; }
		return true;
	}
	bool CHECK_ENCODING(unsigned char in)
	{
		if (in < 0 || in > 15) { return false; }
		return true;
	}
}