#pragma once
#include "BaseTemplateEntities.cpp"
#include "Enums.h"
namespace DTST
{
	SafeAvlDict<std::string, std::string> specSymbolDict();
	SafeAvlDict<std::string, std::string> specCodeDict();
	SafeAvlDict<unsigned short, std::string> codeToEncodingDict();
	SafeAvlDict<std::string, unsigned short> encodingToCodeDict();


	const SafeAvlDict<std::string, std::string> entityToCode = specCodeDict();
	const SafeAvlDict<std::string, std::string> entityToSymbol = specSymbolDict();
	const SafeAvlDict<unsigned short, std::string> encodingToSymbol = codeToEncodingDict();
	const SafeAvlDict<std::string, unsigned short> encodingToCode = encodingToCodeDict();

	
}
