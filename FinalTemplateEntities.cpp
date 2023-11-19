#pragma once
#include "TemplateEntities.cpp"
#include "Structures.h"
#include "DTST.h"
#include "BaseStructures.h"

namespace DTST
{
	template <typename T1>
	class UnsafeXmlParser : public Parser<T1>
	{

	protected:
		bool IsDoc;
		//XmlTag createXmlTree(bool isDoc);
	public:
		UnsafeXmlParser(T1* text);
		UnsafeXmlParser(T1& text, unsigned start, unsigned end);
		UnsafeXmlParser(T1& text);
		UnsafeXmlParser(T1* text, unsigned start, unsigned end);

		unsigned skipNoTag(bool once);
		Attribute getAttribute(bool valueless);
		UnsafeList<unsigned>* declarationToList();
		

		XmlText* getText(short depth, bool isCdata, unsigned char toCdata);
		XmlPi* getPi();
		XmlComm* getComm();
		XmlDoc* getDoc();
		XmlTag* getTag(short depth = 0);

		//static
		static XmlText* formatToXmlText(std::string& in, bool fromCdata);
		static UnsafeList<unsigned> getCdataToReplaceList(std::string& in);

	};





	//Definition 
	//UnsafeXmlParser
	template <typename T1>
	UnsafeXmlParser<T1>::UnsafeXmlParser(T1* text) : Parser<T1>(text) {}
	template <typename T1>
	UnsafeXmlParser<T1>::UnsafeXmlParser(T1* text, unsigned start, unsigned end) : Parser<T1>(text, start, end) {}
	template <typename T1>
	UnsafeXmlParser<T1>::UnsafeXmlParser(T1& text, unsigned start, unsigned end) : Parser<T1>(text, start, end) {}
	template <typename T1>
	UnsafeXmlParser<T1>::UnsafeXmlParser(T1& text) : Parser<T1>(text) {}
	template <typename T1>
	unsigned UnsafeXmlParser<T1>::skipNoTag(bool once)
	{
		Parser<T1>::saveOptions();
		if (this->Options.shift == false) { this->savePoint(); }
		this->Options.shift = true;
		this->Options.stepout = true;
		do
		{
			this->compSubStr("<");
			switch (this->Text[this->ReadPos.getPos()])
			{
			case '?':
			{
				this->compSubStr("?>");
				break;
			}
			case '!':
			{
				switch (this->Text[this->ReadPos.getPos() + 1])
				{
				case '-':
				{
					this->compSubStr("-->");
					break;
				}
				case 'D':
				{
					this->compSubStr("]>");
					break;
				}
				case '[':
				{
					this->compSubStr("]]>");
					break;
				}
				default:
					break;
				}
			}
			default:
			{
				once = true;
				break;
			}
			}
		} while (once != true);
		unsigned temp = this->Text[this->ReadPos.getPos()];
		this->loadOptions();
		if (this->Options.shift == false) { this->loadPoint(); }
		else if (this->Options.stepout == false) {--this->ReadPos; }
		return temp;
	}

	template <typename T1>
	XmlPi* UnsafeXmlParser<T1>::getPi()
	{
		XmlPi* ret = new XmlPi();
		this->saveOptions();
		if (this->Options.shift == false) { this->savePoint(); }
		this->Options.shift = true;
		this->Options.stepout = true;
		this->skipSyms("<?");
		ret->Base = this->takeSubStr("", " ", true, false);
		ret->Data = this->takeSubStr("", "?", true, false);
		this->loadOptions();
		if (this->Options.shift == false) { this->loadPoint(); }
		else if (this->Options.stepout == true) { ++this->ReadPos; }
		return ret;
	}
	template <typename T1>
	XmlComm* UnsafeXmlParser<T1>::getComm()
	{
		XmlComm* ret = new XmlComm();
		this->saveOptions();
		if (this->Options.shift == false) { this->savePoint(); }
		this->Options.shift = true;
		this->Options.stepout = true;
		this->skipSyms("<!-");
		ret->Base = this->takeSubStr("", "--", true, false);
		this->loadOptions();
		if (this->Options.shift == false) { this->loadPoint(); }
		else if (this->Options.stepout == true) { ++this->ReadPos; }
		return ret;
	}


	template <typename T1>
	XmlDoc* UnsafeXmlParser<T1>::getDoc()
	{
		XmlDoc* ret = new XmlDoc();
		this->saveOptions();
		if (this->Options.shift == false) { this->savePoint(); }
		this->Options.shift = true;
		this->Options.stepout = false;
		switch (this->Text[this->compSubStr("<") + 1])
		{
		case '?':
		{
			if (this->takeSubStr("?", " ") == "xml")
			{
				this->Options.shift = true;
				UnsafeList<unsigned> *temp = declarationToList();
				if (temp == NULL) 
				{ 
					std::cerr << "Invalid Xml declaration\n";
					return NULL;
				}
				else
				{
					for (unsigned short i = 0; i < temp->length(); ++i)
					{
						switch (i)
						{
						case 0:
						{
							if (temp->countList(true) == 1) { ret->Version = true; }
							break;
						}
						case 1:
						{
							ret->Encoding = temp->countList(true);
							break;
						}
						case 2:
						{
							if (temp->countList(true) == 1) { ret->StandAlone = true; }
							break;
						}
						default:
						{
							std::cerr << "This error is impossible, however...\n";
							break;
						}
							
						}
					}
				}
			}
			else
			{
				break;
			}
		}
		case '!':
		{
			if (this->takeSubStr("!", " ") == "DOCTYPE")
			{
				this->Options.shift = true;
				this->compSubStr("]>");
			}
			break;
		}
		default:
			break;
		}
		this->Options.stepout = false;
		char temp;
		if (this->compSubStr("<") < 0) { temp = ')'; }
		else if (this->ReadPos.getPos() != this->ReadPos.getMaxPos()) { temp = this->Text[this->ReadPos.getPos() + 1]; }
		else { std::cerr << "Unexpected end of XML document\n"; }
		while (temp == '?' || temp == '!')
		{
			if (temp == '?') { ret->BeforTreeNonTreeEl.addEl(this->getPi()); }
			else if (temp == '!') { ret->BeforTreeNonTreeEl.addEl(this->getComm()); }
			else { std::cerr << "This error is impossible, however...\n"; }
			if (this->compSubStr("<") < 0) { temp = ')'; }
			else if (this->ReadPos.getPos() != this->ReadPos.getMaxPos()) { temp = this->Text[this->ReadPos.getPos() + 1]; }
			else { std::cerr << "Unexpected end of XML document\n"; }
		}
		ret->RootEl = this->getTag();
		if (this->compSubStr("<") < 0) { temp = ')'; }
		else if (this->ReadPos.getPos() != this->ReadPos.getMaxPos()) { temp = this->Text[this->ReadPos.getPos() + 1]; }
		else { std::cerr << "Unexpected end of XML document\n"; }
		while (temp == '?' || temp == '!')
		{
			if (temp == '?') { ret->AfterTreeNonTreeEl.addEl(this->getPi()); }
			else if (temp == '!') { ret->AfterTreeNonTreeEl.addEl(this->getComm()); }
			else { std::cerr << "This error is impossible, however...\n"; }
			temp = this->Text[this->compSubStr("<") + 1];
		}
		return ret;
	}
	template <typename T1>
	XmlTag* UnsafeXmlParser<T1>::getTag(short depth)
	{
		this->saveOptions();
		if (this->Options.shift == false) { this->savePoint(); }
		XmlTag* ret = new XmlTag();
		ret->Depth = depth;
		unsigned IndexTemp = 0;
		this->Options.shift = true;
		this->Options.stepout = false;
		skipNoTag(false);
		Attribute Temp = getAttribute(true);
		ret->Base = Temp.name();
		ret->NameSpace = Temp.nameSpace();
		while (this->Text[this->ReadPos.getPos()] != '>' && this->Text[this->ReadPos.getPos()] != '/')
		{
			ret->Attributes.addEl(this->getAttribute(false));
		}
		this->Options.stepout = false;
		if (this->Text[this->ReadPos.getPos()] != '/')
		{
			while (1)
			{
				if (this->Text[this->skipSyms("		\n\r>")] == '<')
				{
					switch (this->Text[this->ReadPos.getPos() + 1])
					{
					case '/':
					{
						Temp = this->getAttribute(true);
						if (Temp.name() == ret->Base && Temp.nameSpace() == ret->NameSpace)
						{
							return ret;
						}
						else
						{
							std::cerr << "Invalid XML structure\n";
						}
						break;
					}
					case '?':
					{
						XmlPi* temp = getPi();
						temp->Father = ret;
						ret->Childrens.addEl(temp);
						break;
					}
					case '!':
					{
						if (this->Text[this->ReadPos.getPos() + 1] == '-')
						{
							XmlComm* temp = getComm();
							temp->Father = ret;
							ret->Childrens.addEl(temp);
						}
						else
						{
							XmlText* temp = getText(depth + 1, true, CDATA_ONLY);
							temp->Father = ret;
							ret->Childrens.addEl(temp);
						}
						break;
					}
					default:
					{
						XmlTag* temp = getTag(depth + 1);
						temp->Father = ret;
						ret->Childrens.addEl(temp);

					}
					}
				}
				else
				{
					XmlText* temp = getText(depth + 1, false, CDATA_ALLOWED);
					temp->Father = ret;
					ret->Childrens.addEl(temp);
				}
			}
		}
		else
		{
			++this->ReadPos;
		}
		this->loadOptions();
		if (this->Options.shift == false) { this->loadPoint(); }
		else if (this->Options.stepout) { ++this->ReadPos; }
		return ret;
	}
	template <typename T1>
	Attribute UnsafeXmlParser<T1>::getAttribute(bool valueless)
	{
		std::string Stoper = "=", Sep = "\"";
		Attribute Ret;
		int IndexTemp1, IndexTemp2;
		bool AddNamespace;
		if (valueless) { Stoper = " "; }
		this->saveOptions();
		if (this->Options.shift == false) { this->savePoint(); }
		this->Options.shift = true;
		this->Options.stepout = true;
		this->skipSyms("	 \n\r\"'</");
		this->Options.shift = false;
		IndexTemp1 = this->compSubStr(Stoper);
		IndexTemp2 = this->compSubStr(">");
		if (IndexTemp1 == IndexTemp2) { std::cerr << "Invalid separator syntaxis"; }
		if (IndexTemp1 > IndexTemp2 || IndexTemp1 == -1) {
			IndexTemp1 = IndexTemp2;
			Stoper = ">";
		}
		AddNamespace = (bool)(this->compSubStr(":", 1, this->compSubStr(Stoper)) != -1);
		this->Options.shift = true;
		if (AddNamespace)
		{
			Ret.setNameSpace(this->takeSubStr("", ":", true));
		}
		Ret.setName(this->takeSubStr("", Stoper, true));
		if (!valueless)
		{
			this->Options.shift = false;
			IndexTemp1 = this->compSubStr("\"");
			IndexTemp2 = this->compSubStr("'");
			if (IndexTemp1 > IndexTemp2 && (IndexTemp2 >= 0))
			{
				Sep = "'";
			}
			this->Options.shift = true;
			Ret.setValue(this->takeSubStr(Sep, Sep));
		}
		this->loadOptions();
		if (this->Options.shift == false)
		{
			this->loadPoint();
		}
		else
		{
			if (!valueless && this->Options.stepout) { ++this->ReadPos; }
			if (valueless && (this->Options.stepout == false)) { --this->ReadPos; }
		}
		return Ret;
	}
	template <typename T1>
	DTST::XmlText* UnsafeXmlParser<T1>::getText(short depth, bool isCdata, unsigned char toCdata)
	{
		if (CHECK_CDATA_STATUS(toCdata) == false)
		{
			std::cerr << "Invalid CDATA status in UnsafeParser::getText\n";
			return NULL;
		}
		XmlText* ret = new XmlText();
		ret->Depth = depth;
		ret->CdataStatus = toCdata;
		if (this->Options.shift == false) { this->savePoint(); }
		this->saveOptions();
		this->Options.shift = true;
		this->Options.stepout = true;
		if (isCdata)
		{
			this->skipSyms("<![CDATA");
			ret->Base = this->takeSubStr("", "]]>", true);
			ret->CdataStatus = CDATA_ONLY;
			switch (toCdata)
			{
			case CDATA_ONLY:
			{
				break;
			}
			case CDATA_ALLOWED:
			{
				ret->convertToCdata(false);
				if (ret->ToReplace->length() > 2)
				{
					ret->convertToCdata(true);
				}
				break;
			}
			case CDATA_NOT_ALLOWED:
			{
				ret->convertToCdata(false);
				ret->CdataStatus = CDATA_NOT_ALLOWED;
				break;
			}
			default:
			{
				std::cerr << "Invalid CDATA status in UnsafeParser::getText\n";
				break;
			}
			}
			return ret;
		}
		ret->ToReplace = new UnsafeList<unsigned>();
		unsigned StartPos = this->skipSyms(">");
		this->Options.shift = false;
		unsigned IndexTemp = this->compSubStr("<");
		if (IndexTemp < 0)
		{
			std::cerr << "Warning: Xml text out of tag tree\n";
			IndexTemp = this->Text.length();
		}
		if (this->compSubStr("]]&gt;", 1, IndexTemp) >= 0)
		{
			if (toCdata != CDATA_NOT_ALLOWED)
			{
				std::cerr << "Warning: allowed CDATA status when string can't formated to CDATA\n";
				toCdata = ret->CdataStatus = CDATA_NOT_ALLOWED;
			}
		}
		this->Options.shift = true;
		char CurrentSym = this->Text[this->ReadPos.getPos()];
		while (CurrentSym != '<')
		{
			if (CurrentSym == '&')
			{
				if (toCdata != CDATA_ONLY) { ret->ToReplace->addEl(this->ReadPos.getPos() - StartPos); }
				ret->Base += entityToSymbol[this->takeSubStr("", ";", true, true)];
			}
			else
			{
				ret->Base += CurrentSym;
				++this->ReadPos;
			}
			CurrentSym = this->Text[(this->ReadPos).getPos()];
		}

		switch (toCdata)
		{
		case CDATA_NOT_ALLOWED:
		{
			break;
		}
		case CDATA_ALLOWED:
		{
			if (ret->ToReplace->length() > 2)
			{
				ret->convertToCdata(true);
			}
			break;
		}
		case CDATA_ONLY:
		{
			ret->convertToCdata(true);
			break;
		}
		default:
		{
			std::cerr << "Invalid CDATA status in UnsafeParser::getText\n";
			break;
		}
		}
		return ret;
	}
	template <typename T1>
	UnsafeList<unsigned>* UnsafeXmlParser<T1>::declarationToList()
	{
		std::string parameters[3] = { "version" , "encoding", "standalone" };
		std::string temp;
		UnsafeList<unsigned> falseRet;
		if (this->Options.shift == false) { this->savePoint(); }
		this->saveOptions();
		this->Options.shift = true;
		this->Options.stepout = true;
		this->skipSyms("<?xml ");
		for (unsigned short i = 0; i < 3; ++i)
		{
			if (this->compSubStr(parameters[i], 1, this->ReadPos.getPos() + parameters[i].length() - 1) > -1)
			{
				if (this->Text[this->skipSyms(" ")] == '=')
				{
					++this->ReadPos;
					if (this->Text[this->skipSyms(" ")] != '\"') { return NULL; }
					temp = this->takeSubStr("\"", "\"");
					switch (i)
					{
					case 0:
					{
						if (temp == "1.0") { falseRet.addEl(0); } //add "false" for first parameter
						else if (temp == "1.1") { falseRet.addEl(1); } //add "true" for first parameter
						else 
						{ 
							std::cerr << "Invalid Xml version\n";
							return NULL; //return NULL wich means invalid declaration
						} 
						break;
					}
					case 1:
					{
						unsigned short tempCode = encodingToCode[temp];
						if (tempCode == 255) { std::cerr << "Invalid enkoding\n"; }
						else { falseRet.addEl(tempCode); }//add encoding code to second parameter
						break;
					}
					case 2:
					{
						if (temp == "no") { falseRet.addEl(0); } //add "false" for first parameter
						else if (temp == "yes") { falseRet.addEl(1); } //add "true" for first parameter
						else 
						{ 
							std::cerr << "Invalid standalone\n";
							return NULL; //return NULL wich means invalid declaration
						}
						break;
					}
					default:
					{
						std::cerr << "This error is impossible, however...\n";
						break;
					}
						
					}
					if (this->Text[this->skipSyms(" ")] == '?') { break; }
				}
			}
			else
			{
				std::cerr << "Invalid Xml declaration's attribute\n";
				return NULL;
			}
		}
		this->loadOptions();
		if (this->Options.shift == false) { this->loadPoint(); }
		else if (this->Options.stepout == false) { --this->ReadPos; }
		UnsafeList<unsigned>* trueRet = new UnsafeList<unsigned>(falseRet); //need to add cutFrom function... (Bruh moment)
		return trueRet;
	}

}