#pragma once
#include "BaseTemplateEntities.h"
namespace DTST
{
	AVLDict<std::string, std::string> SpecSymbolDict();
	AVLDict<std::string, std::string> SpecCodeDict();

	const AVLDict<std::string, std::string> toCode = SpecCodeDict();
	const AVLDict<std::string, std::string> toSymbol = SpecSymbolDict();

	
}
