#pragma once
#include "DTST.h"
#include "BaseTemplateEntities.h"
namespace DTST
{
//Declaration of entities
	
	//XmlText
	class XmlText
	{
	protected:
		List<unsigned>* ToReplace = NULL;
		std::string Name;
		void* Father;
		short Depth;
		XmlText(short depth, std::string name, void* father = NULL);
	public:
		virtual std::string textDump(bool showFormat);
		XmlText(short depth, List<unsigned>* toReplace, std::string name = "", void* father = NULL);
		~XmlText();
	};


template<typename T1>
class Parser
{
	struct settings
	{
		unsigned shift : 1;
		unsigned stepout : 1;
	};
	List <settings> OptionSaves;
	List <unsigned> PosSaves;
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
	Attribute takeAttribute(bool valueless);
	DTST::XmlText getAmpFormattedText(short depth);

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
	class XmlTree : public DTST::XmlText
	{
		DTST::List<DTST::XmlText*> Childrens;
		std::string Namespace = "";
		DTST::List<DTST::Attribute> Attributes;
		unsigned AttributeNumber;
		unsigned ChildNumber;
	public:
		static bool xmlTreeCheck(std::string* text)
		{
			DTST::Parser <std::string> parse(text);
			if (parse.compSubStr("'", 0) + 1 % 2 == 0) { return false; }
			if (parse.compSubStr("\"", 0) + 1 % 2 == 0) { return false; }
			if (parse.compSubStr("<", 0) != parse.compSubStr(">", 0)) { return false; }
			return true;
		}
		~XmlTree()
		{
			//free(Childrens);
			//free(Attributes);
		}
		template <typename T>
		XmlTree(T* Text, unsigned* edge = NULL, short depth = 0) : DTST::XmlText(depth, NULL)
		{
			long IndexTemp = 0;
			bool AddNamespace;
			if (edge) { IndexTemp = *edge; }
			DTST::Parser <T> parse(Text, IndexTemp, Text->length() - 1);
			parse.setShift(true);
			parse.setStepout(false);
			parse.compSubStr("<");
			DTST::Attribute Temp = parse.takeAttribute(true);
			Name = Temp.name();
			Namespace = Temp.nameSpace();
			parse.setShift(false);
			IndexTemp = parse.compSubStr(">");
			AttributeNumber = parse.compSubStr("'", 0, IndexTemp);
			AttributeNumber += parse.compSubStr("\"", 0, IndexTemp);
			AttributeNumber /= 2;
			parse.setShift(true);
			for (int i = 0; i < AttributeNumber; ++i)
			{
				Attributes.addEl(parse.takeAttribute(false));
			}
			if (Text[0][parse.getCurrentPos()] != '/')
			{
				unsigned NextPos;
				while (1)
				{
					if (Text[0][parse.skipSyms("		\n\r>")] == '<')
					{
						if (Text[0][parse.skipSyms("		\n\r") + 1] == '/')
						{
							Temp = parse.takeAttribute(true);
							if (Temp.name() == Name && Temp.nameSpace() == Namespace)
							{
								break;
							}
							else
							{
								std::cerr << "Invalid XML structure\n";
							}
						}
						NextPos = parse.getCurrentPos();
						XmlTree* xTmp = new XmlTree(Text, &NextPos, Depth + 1);
						parse.setCurentPos(NextPos);
						Childrens.addEl(xTmp);
						parse.skipSyms("/");
					}
					else
					{
						++ChildNumber;
						DTST::XmlText* tmp = new DTST::XmlText(parse.getAmpFormattedText(Depth + 1));
						Childrens.addEl(tmp);
					}
				}
			}
			if (edge) { *edge = parse.getCurrentPos(); }
			return;
		}
		std::string textDump(bool showFormat) override
		{
			std::string Ret = '<' + Namespace;
			if (showFormat)
			{
				for (int i = 0; i < Depth; ++i)
				{
					Ret = '\t' + Ret;
				}
			}
			if (Namespace.length() != 0) { Ret += ':'; }
			Ret += Name;
			Attributes.setCountFirst();
			for (unsigned i = 0; i < Attributes.length(); ++i)
			{
				DTST::Attribute temp = Attributes.countList(true);
				Ret += ' ' + temp.textDump();
			}
			if (Childrens.length() == 0) 
			{ Ret += "/"; }
			Ret += '>';
			if (showFormat) { Ret += '\n'; }
			Childrens.setCountFirst();
			for (unsigned i = 0; i < Childrens.length(); ++i)
			{
				XmlText* tmp = Childrens.countList(true);
				Ret += tmp->textDump(showFormat);
			}
			if (Childrens.length() != 0)
			{
				if (showFormat)
				{
					for (int i = 0; i < Depth; ++i)
					{
						Ret += '\t';
					}
				}
				Ret += '<';
				Ret += '/' + Namespace;
				if (Namespace.length() != 0) { Ret += ':'; }
				Ret += Name + '>';
				if (showFormat) { Ret += '\n'; }
			}

			return Ret;
		}
	};




//Definition of entities
	

	//XmlText

	//Protected
	XmlText::XmlText(short depth, std::string name, void* father) : Name(name), Father(father), Depth(depth) {}

	//Public
	std::string XmlText::textDump(bool showFormat)
	{
		std::string Ret;
		if (showFormat)
		{
			Ret += Parser <std::string> ::makeTabulation(Depth);
			unsigned k = 0;
			for (unsigned i = 0; i < Name.length(); ++i)
			{
				if (Name[i] == '\n') { k = 0; }
				else if ((k + 1) % 101 == 0)
				{
					Ret += '\n';
					Ret += Parser <std::string> ::makeTabulation(Depth);
					k = 0;
				}
				Ret += Name[i];
			}
			return Ret + "\n"; 
		}
		else
		{
			ToReplace->setCountFirst();
			unsigned k = 0;
			for (unsigned i = 0; i < Name.length(); ++i)
			{
				if (ToReplace->length() != k)
				{
					if (ToReplace->countList(true) == i)
					{
						std::string temp;
						temp += Name[i];
						Ret += DTST::toCode[temp];
						++k;
						continue;
					}
				}
				Ret += Name[i];
			}
			return Ret;
		}
	}
	XmlText::XmlText(short depth, List<unsigned>* toReplace, std::string name, void* father) :
		XmlText(depth, name, father)
	{
		if (toReplace == NULL)
		{
			ToReplace = new DTST::List<unsigned>();
			//cerr << "Can't create XmlText without repalce list\n";
		}
		else
		{
			ToReplace = toReplace;
		}
	}
	XmlText::~XmlText()
	{
		delete(ToReplace);
	}

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
	Parser<T1>::Parser(T1& text) : Text(text), ReadPos(0, text->length() - 1) {}
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
		int i = ReadPos.getPos();
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
	template <typename T1>
	Attribute Parser<T1>::takeAttribute(bool valueless)
	{
		std::string Stoper = "=", Sep = "\"";
		Attribute Ret;
		int IndexTemp1, IndexTemp2;
		bool AddNamespace;
		if (valueless) { Stoper = " "; }
		savePoint();
		saveOptions();
		setShift(true);
		setStepout(true);
		skipSyms("	 \n\r\"'</");
		setShift(false);
		IndexTemp1 = compSubStr(Stoper);
		IndexTemp2 = compSubStr(">");
		if (IndexTemp1 == IndexTemp2) { std::cerr << "Invalid separator syntaxis"; }
		if (IndexTemp1 > IndexTemp2 || IndexTemp1 == -1) {
			IndexTemp1 = IndexTemp2;
			Stoper = ">";
		}
		AddNamespace = (bool)(compSubStr(":", 1, compSubStr(Stoper)) != -1);
		setShift(true);
		if (AddNamespace)
		{
			Ret.setNameSpace(takeSubStr("", ":", true));
		}
		Ret.setName(takeSubStr("", Stoper, true));
		if (!valueless)
		{
			setShift(false);
			IndexTemp1 = compSubStr("\"");
			IndexTemp2 = compSubStr("'");
			if (IndexTemp1 > IndexTemp2 && (IndexTemp2 >= 0))
			{
				Sep = "'";
			}
			setShift(true);
			Ret.setValue(takeSubStr(Sep, Sep));
		}
		loadOptions();
		if (!Options.shift)
		{
			loadPoint();
		}
		else
		{
			if (!valueless && Options.stepout) { ++ReadPos; }
			if (valueless && !Options.stepout) { --ReadPos; }
			clearPointSave();
		}
		return Ret;
 	}
	template <typename T1>
	DTST::XmlText Parser<T1>::getAmpFormattedText(short depth)
	{
		DTST::List<unsigned>* ToReplace = new DTST::List<unsigned>();
		std::string RetText = "";
		char CurrentSym = Text[ReadPos.getPos()];
		if (Options.shift != true) { savePoint(); }
		saveOptions();
		setShift(true);
		setStepout(false);
		skipSyms("	 \n\r>");
		while (CurrentSym != '<')
		{
			if (CurrentSym == '&')
			{
				ToReplace->addEl(RetText.length());
				RetText += DTST::toSymbol[takeSubStr("", ";", true)];
			}
			else
			{
				RetText += CurrentSym;
			}
			CurrentSym = Text[(++ReadPos).getPos()];
		}
		loadOptions();
		if (Options.shift != true) { loadPoint(); }
		return XmlText(depth, ToReplace, RetText);
	}
}