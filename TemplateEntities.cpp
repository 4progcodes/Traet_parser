#pragma once
#include <iostream>
#include "BaseTemplateEntities.cpp"
namespace DTST
{
template<typename T1>
class Parser
{
protected:
	struct settings
	{
		unsigned shift : 1;
		unsigned stepout : 1;
	};
	UnsafeList <settings> OptionSaves;
	UnsafeList <unsigned> PosSaves;
	settings Options;
	Mark ReadPos;
	T1& Text;
	void saveOptions();
	void loadOptions();
	void clearOptionSave();
public:
	void setShift(bool Shift);
	void setStepout(bool Stepout);
	void savePoint();
	void loadPoint();
	void clearPointSave();
	void setCurentPos(unsigned pos);
	unsigned getCurrentPos();
	char getCurrentSym();
	Parser(T1& text, unsigned start, unsigned end);
	Parser(T1& text);
	Parser(T1* text, unsigned start, unsigned end);
	Parser(T1* text);
	template<typename T2>
	int compSubStr(T2* less, int n = 1, unsigned end = 0);
	template<typename T2>
	int compSubStr(T2& less, int n = 1, unsigned end = 0);
	int compSubStr(const char* less, int n = 1, unsigned end = 0);
	template<typename T2>
	int skipSyms(T2* skiped, unsigned end = 0);
	template<typename T2>
	int skipSyms(T2 skiped, unsigned end = 0);
	int skipSyms(const char* skiped, unsigned end = 0);
	std::string takeSubStr(std::string sep1 = "", std::string sep2 = "", bool takesep1 = false, bool takesep2 = false);
	
	//static

	static std::string makeTabulation(unsigned i)
	{
		std::string Ret = "";
		for (; i > 0; --i)
		{
			Ret += '\t';
		}
		return Ret;
	}
};

	//Parser
	//protected
	template <typename T1>
	void Parser<T1>::saveOptions() { OptionSaves.addEl(Options); }
	template <typename T1>
	void Parser<T1>::loadOptions() { Options = OptionSaves.popEl(); }
	template <typename T1>
	void Parser<T1>::clearOptionSave() { OptionSaves.popEl(); }

	//public
	template <typename T1>
	void Parser<T1>::setShift(bool Shift) { Options.shift = Shift; }
	template <typename T1>
	void Parser<T1>::setStepout(bool Stepout) { Options.stepout = Stepout; }
	template <typename T1>
	void Parser<T1>::savePoint() { PosSaves.addEl(ReadPos.getPos()); }
	template <typename T1>
	void Parser<T1>::loadPoint() { ReadPos.setPos(PosSaves.popEl()); }
	template <typename T1>
	void Parser<T1>::clearPointSave() { PosSaves.popEl(); }
	template <typename T1>
	void Parser<T1>::setCurentPos(unsigned pos) { ReadPos.setPos(pos); }
	template <typename T1>
	unsigned Parser<T1>::getCurrentPos() { return ReadPos.getPos(); }
	template <typename T1>
	char Parser<T1>::getCurrentSym() 
	{
		return Text[ReadPos.getPos()];
	}
	template <typename T1>
	Parser<T1>::Parser(T1& text, unsigned start, unsigned end) : Text(text), ReadPos(start, end) {}
	template <typename T1>
	Parser<T1>::Parser(T1& text) : Text(text), ReadPos(0, text.length() - 1) {}
	template <typename T1>
	Parser<T1>::Parser(T1* text, unsigned start, unsigned end) : Text(*text), ReadPos(start, end) {}
	template <typename T1>
	Parser<T1>::Parser(T1* text) : Text(*text), ReadPos(0, text->length() - 1) {}
	template<typename T1>
	template<typename T2>
	int Parser<T1>::compSubStr(T2* less, int n, unsigned end)
	{
		savePoint();
		if (end == 0) { end = ReadPos.getMaxPos(); }
		else if (ReadPos.getMaxPos() < end)
		{
			std::cerr << "End of reading lable more than text length\n";
			return -2;
		}
		if (ReadPos.getMaxPos() < less->length())
		{
			std::cerr << "Substring longer than text\n";
			return -3;
		}
		if (n < 0)
		{
			std::cerr << "Invalid number of expected enters\n";
			return -4;
		}
		int i = 0;
		do {
			if (Text[ReadPos.getPos()] == less[0][i])
			{
				if (++i == less->length())
				{
					if (--n == 0)
					{
						int ret = ReadPos.getPos() - --i;
						if (Options.shift)
						{
							if (Options.stepout)
							{
								++ReadPos;
							}
							clearPointSave();
						}
						else
						{
							loadPoint();
						}
						return ret;
					}
					i = 0;
				}
			}
			else { i = 0; }
		} while (ReadPos++.getPos() < end);
		loadPoint();
		if (n < 1)
		{
			return -n;
		}
		else
		{
			return -1;
		}
	}
	template<typename T1>
	template<typename T2>
	int Parser<T1>::compSubStr(T2& less, int n, unsigned end)
	{
		return compSubStr(&less, n, end);
	}
	template<typename T1>
	int Parser<T1>::compSubStr(const char* less, int n, unsigned end)
	{
		std::string Less(less);
		return compSubStr(&Less, n, end);
	}
	template<typename T1>
	template<typename T2>
	int Parser<T1>::skipSyms(T2* skiped, unsigned end)
	{
		if (end == 0) { end = ReadPos.getMaxPos(); }
		else if (ReadPos.getMaxPos() <= end)
		{
			std::cerr << "End of reading lable more than text length\n";
			return -2;
		}
		unsigned i = ReadPos.getPos();
		while (i <= end)
		{
			for (int j = 0; j < skiped->length(); j)
			{
				if (Text[i] == skiped[0][j])
				{
					if (++i > ReadPos.getMaxPos()) { return -1; }
					j = 0;
				}
				else { ++j; }
			}
			break;
		}
		if (Options.shift) { ReadPos.setPos(i); }
		return i;
	}
	template<typename T1>
	template<typename T2>
	int Parser<T1>::skipSyms(T2 skiped, unsigned end)
	{
		return skipSyms(&skiped, end);
	}
	template<typename T1>
	int Parser<T1>::skipSyms(const char* skiped, unsigned end)
	{
		std::string Skiped(skiped);
		return skipSyms(&Skiped, end);
	}
	template <typename T1>
	std::string Parser<T1>::takeSubStr(std::string sep1, std::string sep2, bool takesep1, bool takesep2)
	{
		std::string ret;
		int start, end, oldpos = ReadPos.getPos();
		saveOptions();
		setShift(true);
		setStepout(true);
		if (sep1 == "") { start = ReadPos.getPos(); }
		else { start = compSubStr(&sep1); }
		Options.stepout = OptionSaves.getLastEl().stepout;
		if (sep2 == "") { end = ReadPos.getMaxPos(); }
		else { end = compSubStr(&sep2); }
		loadOptions();
		if (!Options.shift) { ReadPos.setPos(oldpos); }
		if (!takesep1) { ++start; }
		if (!takesep2) { --end; }
		while (start <= end)
		{
			ret += Text[start++];
		}
		return ret;
	}
}