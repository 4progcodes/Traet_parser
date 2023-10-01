#include <iostream>
#include <fstream>
#include "DTST.h"

using namespace std;

template<typename T1, typename T2>
class Pair
{
protected:
	T1 Key;
	T2 Value;
public:
	Pair(T1 key, T2 value) : Key(key), Value(value) {}
	bool operator > (Pair <T1, T2> other) const { return Key > other.Key; }
	bool operator >= (Pair <T1, T2> other) const { return Key >= other.Key; }
	bool operator < (Pair <T1, T2> other) const { return Key < other.Key; }
	bool operator <= (Pair <T1, T2> other) const { return Key <= other.Key; }
	bool operator == (Pair <T1, T2> other) const { return Key == other.Key; }
	bool operator > (T1 other) const { return Key > other; }
	bool operator >= (T1 other) const { return Key >= other; }
	bool operator < (T1 other) const { return Key < other; }
	bool operator <= (T1 other) const { return Key <= other; }
	bool operator == (T1 other) const { return Key == other; }
	operator T1() const { return Key; }
	operator T2() const { return Value; }
};

//template<typename T>
//T getEdge(T first, T second, bool mode) // return bigest value if mode - true, else return lesser value.
//{
//	mode ^= (first < second);
//	if (mode) { return first; }
//	return second;
//}





class Mark
{
	unsigned Position, MinPosition, MaxPosition;
public:
	Mark(unsigned minPosition, unsigned maxPosition) 
	{
		if (minPosition < maxPosition)
		{
			Position = MinPosition = minPosition;
			MaxPosition = maxPosition;
		}
		else
		{
			cerr << "Attempt to create mark with start position after end position\n";
		}
	}
	Mark &operator ++ ()
	{
		if (Position < MaxPosition) { ++Position; }
		return *this;
	}
	Mark operator ++ (int)
	{
		if (Position >= MaxPosition) 
		{ 
			return *this;
		}
		Mark OldPosition = *this;
		++Position;
		return OldPosition;
	}
	Mark &operator -- ()
	{
		if (Position > MinPosition) { --Position; }
		return *this;
	}
	Mark operator -- (int)
	{
		if (Position <= MinPosition)
		{
			return *this;
		}
		Mark OldPosition(*this);
		--Position;
		return OldPosition;
	}
	Mark& operator = (Mark &second)
	{
		this->MaxPosition = second.MaxPosition;
		this->MinPosition = second.MinPosition;
		this->Position = second.Position;
		return *this;
	}
	unsigned getPosition() { return Position; }
	bool setPosition(unsigned NewPosition) 
	{
		if (NewPosition > MaxPosition || NewPosition < MinPosition) { return false; }
		Position = NewPosition;
		return true;
	}
	unsigned getMaxPosition() { return MaxPosition; }
	bool setMaxPosition(unsigned NewPosition)
	{
		if (NewPosition < MinPosition)
		{
			return false;
		}
		if (NewPosition < Position)
		{
			Position = NewPosition;
		}
		MaxPosition = NewPosition;
		return true;
	}
	unsigned getMinPosition() { return MinPosition; }
	bool setMinPosition(unsigned NewPosition)
	{
		if (NewPosition > MaxPosition)
		{
			return false;
		}
		if (NewPosition > Position)
		{
			Position = NewPosition;
		}
		MinPosition = NewPosition;
		return true;
	}
};

//class Textline
//{
//	char* Text;
//	unsigned Length;
//public:
//	Textline(string* text)
//	{
//		Length = text->length();
//		Text = (char*)malloc(sizeof(char) * Length);
//		for (int i = 0; i < Length; ++i)
//		{
//			Text[i] = text[0][i];
//		}
//	}
//
//};

class Attribute
{
protected:
	string Namespace = "", Name = "", Value = "";
public:
	Attribute(string Ns, string N, string V) : Namespace(Ns), Name(N), Value(V) {}
	Attribute(string N, string V) : Name(N), Value(V) {}
	Attribute() {}
	string nameSpace() { return Namespace; }
	string name() { return Name; }
	string value() { return Value; }
	void setNameSpace(string Ns) { Namespace = Ns; }
	void setName(string N) { Name = N; }
	void setValue(string V) { Value = V; }
	string textDump()
	{
		string Ret = Name + '=' + '"' + Value + '"';
		return Namespace.length() == 0 ? Ret : Namespace + ':' + Ret;
	}
};

class XmlText
{
protected:
	DTST::List<unsigned>* ToReplace = NULL;
	string Name;
	void* Father;
	XmlText(string name, void* father = NULL) : Name(name), Father(father) {}
public:
	virtual string textDump(bool showFormat)
	{
		if (showFormat) { return Name + "\n"; }
		else
		{
			ToReplace->setCountFirst();
			string Ret;
			unsigned k = 0;
			for (unsigned i = 0; i < Name.length(); ++i)
			{
				if (ToReplace->length() != k)
				{
					if (ToReplace->countList(true) == i)
					{
						string temp;
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
	XmlText(DTST::List<unsigned>* toReplace, string name = "", void* father = NULL) :
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
	DTST::List <settings> OptionSaves;
	DTST::List <unsigned> PosSaves;
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
	int compSubStr(T2 *less, int n = 1, unsigned end = 0)
	{
		savePoint(); 
		if (end == 0) { end = ReadPos.getMaxPosition(); }
		else if (ReadPos.getMaxPosition() < end)
		{
			cerr << "End of reading lable more than text length\n";
			return -2;
		}
			if (ReadPos.getMaxPosition() < less->length())
			{
				cerr << "Substring longer than text\n";
				return -3;
			}
			if (n < 0) 
			{
				cerr << "Invalid number of expected enters\n";
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
		string Less(less);
		return compSubStr(&Less, n, end);
	}
	template<typename T2>
	int skipSyms(T2 *skiped, unsigned end = 0)
	{
		if (end == 0) { end = ReadPos.getMaxPosition(); }
		else if (ReadPos.getMaxPosition() <= end)
		{
			cerr << "End of reading lable more than text length\n";
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
		string Skiped(skiped);
		return skipSyms(&Skiped, end);
	}
	string takeSubStr(string sep1 = "", string sep2 = "", bool takesep1 = false, bool takesep2 = false)
	{
		string ret;
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
		string Stoper = "=", Sep = "\"";
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
		if (IndexTemp1 == IndexTemp2) { cerr << "Invalid separator syntaxis"; }
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
	XmlText getAmpFormattedText() 
	{ 
		DTST::List<unsigned>* ToReplace = new DTST::List<unsigned>();
		string RetText = "";
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





class XmlTree : public XmlText
{
	DTST::List<XmlText*> Childrens;
	string Namespace = "";
	DTST::List<Attribute> Attributes;
	unsigned AttributeNumber;
	unsigned ChildNumber;
	short Depth;
public:
	static bool xmlTreeCheck(string* text)
	{
		parser <string> parse(text);
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
	XmlTree(string *Text, unsigned *edge = NULL, short depth = 0) : XmlText (NULL), Depth(depth)
	{
		//забув, що коли тег закривається, то перед ним ставиться знак /, ну а ще, якщо тег пустий, то він може бути 
		//закритий відразу, якщо там є знак / після атрибутів
		long IndexTemp = 0;
		bool AddNamespace;
		if (edge) { IndexTemp = *edge; }
		parser <string> parse(Text, IndexTemp, Text->length());
		parse.setShift(true);
		parse.setStepout(false);
		parse.compSubStr("<");
		Attribute Temp = parse.takeAttribute(true);
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
							cerr << "Invalid XML structure\n";
						}
					}
					NextPos = parse.getCurrentPos();
					XmlTree *xTmp = new XmlTree(Text, &NextPos, Depth + 1);
					parse.setCurentPos(NextPos);
					Childrens.addEl(xTmp);
					//Childrens.addEl(XmlTree(Text, &NextPos, Depth + 1));
				}
				else
				{
					++ChildNumber;
					XmlText *tmp = new XmlText(parse.getAmpFormattedText());
					Childrens.addEl(tmp);
				}
			}
		}
		if (edge) { *edge = parse.getCurrentPos(); }
		return;
	}
	string textDump(bool showFormat) override
	{
		string Ret = '<' + Namespace;
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
			Attribute temp = Attributes.countList(true);
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

int main()
{
	string a = "<w:p w14:paraId=\"055D6CC6\" w14:textId=\"3080C9E9\" w:rsidR=\"00C41096\" w:rsidRPr=\"00AB730C\" w:rsidRDefault=\"00745B3E\"><w:pPr><w:rPr><w:lang w:val=\"en-US\"/></w:rPr></w:pPr></w:p>";
	XmlTree b(&a);
	/*string c = b.textDump(false);
	int l = a.length();
	if (l > c.length())
	{
		l = c.length();
	}
	for (int i = 0; i < l; ++i)
	{
		if (a[i] == c[i]) { cout << a[i]; }
		else { cout << "*HERE-> (orig - '" << a[i] << "' , copy - '" << c[i] << "')*"; }
	}*/
	if (a.length() == b.textDump(false).length()) { cout << "ass"; }
	cout << b.textDump(true);
	//cout << DTST::toSymbol["&amp"];
}