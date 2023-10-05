#pragma once
#include "DTST.h"
#include "BaseTemplateEntities.h"
namespace DTST
{
	class XmlText
	{
	protected:
		List<unsigned>* ToReplace = NULL;
		std::string Name;
		void* Father;
		XmlText(std::string name, void* father = NULL) : Name(name), Father(father) {}
	public:
		virtual std::string textDump(bool showFormat)
		{
			if (showFormat) { return Name + "\n"; }
			else
			{
				ToReplace->setCountFirst();
				std::string Ret;
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
				return Ret + "\n";
			}
		}
		XmlText(List<unsigned>* toReplace, std::string name = "", void* father = NULL) :
			XmlText(name, father)
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
		~XmlText()
		{
			delete(ToReplace);
		}
	};


template<typename T1>
class parser
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
	T1* Text;
	void saveOptions() { OptionSaves.addEl(Options); }
	void loadOptions() { Options = OptionSaves.popEl(); }
	void clearOptionSave() { OptionSaves.popEl(); }
	//void 
public:
	void setShift(bool Shift) { Options.shift = Shift; }
	void setStepout(bool Stepout) { Options.stepout = Stepout; }
	void savePoint() { PosSaves.addEl(ReadPos.getPosition()); }
	void loadPoint() { ReadPos.setPosition(PosSaves.popEl()); }
	void clearPointSave() { PosSaves.popEl(); }
	void setCurentPos(unsigned pos) { ReadPos.setPosition(pos); }
	unsigned getCurrentPos() { return ReadPos.getPosition(); }
	parser(T1* text, unsigned start, unsigned end) : Text(text), ReadPos(start, end) {}
	parser(T1* text) : Text(text), ReadPos(0, text->length() - 1) {}
	template<typename T2>
	int compSubStr(T2* less, int n = 1, unsigned end = 0)
	{
		savePoint();
		if (end == 0) { end = ReadPos.getMaxPosition(); }
		else if (ReadPos.getMaxPosition() < end)
		{
			std::cerr << "End of reading lable more than text length\n";
			return -2;
		}
		if (ReadPos.getMaxPosition() < less->length())
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
			if (Text[0][ReadPos.getPosition()] == less[0][i])
			{
				if (++i == less->length())
				{
					if (--n == 0)
					{
						int ret = ReadPos.getPosition() - --i;
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
		} while (ReadPos++.getPosition() < end);
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
	template<typename T2>
	int compSubStr(T2 less, int n = 1, unsigned end = 0)
	{
		return compSubStr(&less, n, end);
	}
	int compSubStr(const char* less, int n = 1, unsigned end = 0)
	{
		std::string Less(less);
		return compSubStr(&Less, n, end);
	}
	template<typename T2>
	int skipSyms(T2* skiped, unsigned end = 0)
	{
		if (end == 0) { end = ReadPos.getMaxPosition(); }
		else if (ReadPos.getMaxPosition() <= end)
		{
			std::cerr << "End of reading lable more than text length\n";
			return -2;
		}
		int i = ReadPos.getPosition();
		while (i <= end)
		{
			for (int j = 0; j < skiped->length(); j)
			{
				if (Text[0][i] == skiped[0][j])
				{
					if (++i > ReadPos.getMaxPosition()) { return -1; }
					j = 0;
				}
				else { ++j; }
			}
			break;
		}
		if (Options.shift) { ReadPos.setPosition(i); }
		return i;
	}
	template<typename T2>
	int skipSyms(T2 skiped, unsigned end = 0)
	{
		return skipSyms(&skiped, end);
	}
	int skipSyms(const char* skiped, unsigned end = 0)
	{
		std::string Skiped(skiped);
		return skipSyms(&Skiped, end);
	}
	std::string takeSubStr(std::string sep1 = "", std::string sep2 = "", bool takesep1 = false, bool takesep2 = false)
	{
		std::string ret;
		int start, end, oldpos = ReadPos.getPosition();
		saveOptions();
		setShift(true);
		setStepout(true);
		if (sep1 == "") { start = ReadPos.getPosition(); }
		else { start = compSubStr(&sep1); }
		Options.stepout = OptionSaves.getLastEl().stepout;
		if (sep2 == "") { end = ReadPos.getMaxPosition(); }
		else { end = compSubStr(&sep2); }
		loadOptions();
		if (!Options.shift) { ReadPos.setPosition(oldpos); }
		if (!takesep1) { ++start; }
		if (!takesep2) { --end; }
		while (start <= end)
		{
			ret += Text[0][start++];
		}
		return ret;
	}
	Attribute takeAttribute(bool valueless)
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
		AddNamespace = (bool)(compSubStr(":", 1, IndexTemp1) != -1);
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
	DTST::XmlText getAmpFormattedText()
	{
		DTST::List<unsigned>* ToReplace = new DTST::List<unsigned>();
		std::string RetText = "";
		char CurrentSym = Text[0][ReadPos.getPosition()];
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
				RetText += DTST::toSymbol[takeSubStr("", " ", true)];
			}
			else
			{
				RetText += CurrentSym;
			}
			CurrentSym = Text[0][(++ReadPos).getPosition()];
		}
		loadOptions();
		if (Options.shift) { loadPoint(); }
		return XmlText(ToReplace, RetText);
	}
};
	class XmlTree : public DTST::XmlText
	{
		DTST::List<DTST::XmlText*> Childrens;
		std::string Namespace = "";
		DTST::List<DTST::Attribute> Attributes;
		unsigned AttributeNumber;
		unsigned ChildNumber;
		short Depth;
	public:
		static bool xmlTreeCheck(std::string* text)
		{
			DTST::parser <std::string> parse(text);
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
		XmlTree(std::string* Text, unsigned* edge = NULL, short depth = 0) : DTST::XmlText(NULL), Depth(depth)
		{
			long IndexTemp = 0;
			bool AddNamespace;
			if (edge) { IndexTemp = *edge; }
			DTST::parser <std::string> parse(Text, IndexTemp, Text->length());
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
					if (Text[0][parse.skipSyms("		\n\r>/")] == '<')
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
						//Childrens.addEl(XmlTree(Text, &NextPos, Depth + 1));
					}
					else
					{
						++ChildNumber;
						DTST::XmlText* tmp = new DTST::XmlText(parse.getAmpFormattedText());
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
			if (Childrens.length() == 0) { Ret += "/"; }
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
}