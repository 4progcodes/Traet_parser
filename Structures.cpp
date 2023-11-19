#include "Structures.h"

namespace DTST
{
	//A_XmlEl
	//protected
	A_XmlEl::A_XmlEl()
	{
		ClassNameHierarchy.addEl(A_XML_EL);
	}
	//public
	bool A_XmlEl::isClassInBranch(unsigned short className)
	{
		ClassNameHierarchy.setCountFirst();
		return ClassNameHierarchy.isInList(className);
	}
	SafeList<unsigned short> A_XmlEl::getClassBranch()
	{
		return ClassNameHierarchy;
	}
	//A_BaseXmlEl
	//protected
	A_BaseXmlEl::A_BaseXmlEl() :Base("") 
	{
		ClassNameHierarchy.addEl(A_BASE_XML_EL);
	}
	A_BaseXmlEl::A_BaseXmlEl(std::string base) : Base(base) 
	{
		ClassNameHierarchy.addEl(A_BASE_XML_EL);
	}

	//XmlDtd
	//public
	XmlDtd::XmlDtd()
	{
		ClassNameHierarchy.addEl(XML_DTD);
	}
	XmlDtd::XmlDtd(std::string base, std::string markingSourse) : MarkingSourse(markingSourse) 
	{
		ClassNameHierarchy.addEl(XML_DTD);
	}
	std::string XmlDtd::textDump(bool showFormat) { return Base; }

	//A_XmlMoveable
	//protected
	A_XmlMoveable::A_XmlMoveable(std::string base, void* father) : Father(father) 
	{
		ClassNameHierarchy.addEl(A_XML_MOVEABLE);
	}

	//public
	A_XmlMoveable::A_XmlMoveable() 
	{
		ClassNameHierarchy.addEl(A_XML_MOVEABLE);
	}
	A_XmlMoveable::A_XmlMoveable(std::string base) : A_BaseXmlEl(base) 
	{
		ClassNameHierarchy.addEl(A_XML_MOVEABLE);
	}
	unsigned short A_XmlMoveable::depth() { return 0; }

	//A_XmlNonTreeEl
	//protected
	A_XmlNonTreeEl::A_XmlNonTreeEl(std::string base, void* father) : A_XmlMoveable(base, father) 
	{
		ClassNameHierarchy.addEl(A_XML_NON_TREE_EL);
	}
	//public
	A_XmlNonTreeEl::A_XmlNonTreeEl() 
	{
		ClassNameHierarchy.addEl(A_XML_NON_TREE_EL);
	}
	A_XmlNonTreeEl::A_XmlNonTreeEl(std::string base) : A_XmlMoveable(base) 
	{
		ClassNameHierarchy.addEl(A_XML_NON_TREE_EL);
	}

	//A_XmlTreeEl
	//protected
	A_XmlTreeEl::A_XmlTreeEl(std::string base, void* father, unsigned short depth) : A_XmlMoveable(base, father), Depth(depth) 
	{
		ClassNameHierarchy.addEl(A_XML_TREE_EL);
	}
	//public
	unsigned short A_XmlTreeEl::depth() { return Depth; }
	A_XmlTreeEl::A_XmlTreeEl() : A_XmlMoveable() 
	{
		ClassNameHierarchy.addEl(A_XML_TREE_EL);
	}
	A_XmlTreeEl::A_XmlTreeEl(std::string base, unsigned short depth) : A_XmlMoveable(base), Depth(depth) 
	{
		ClassNameHierarchy.addEl(A_XML_TREE_EL);
	}

	//XmlTag
	//protected
	XmlTag::XmlTag(std::string base, XmlTag* father, unsigned short depth, std::string nameSpace,
		UnsafeList<A_XmlMoveable*>& childrens, UnsafeList<Attribute>& attributes) :
		A_XmlTreeEl(base, father, depth), NameSpace(nameSpace), Childrens(childrens), Attributes(attributes) 
	{
		ClassNameHierarchy.addEl(XML_TAG);
	}
	//public
	XmlTag::XmlTag() 
	{
		ClassNameHierarchy.addEl(XML_TAG);
	}
	XmlTag::XmlTag(std::string base, unsigned short depth, std::string nameSpace,
		UnsafeList<A_XmlMoveable*>& childrens, UnsafeList<Attribute>& attributes) :
		A_XmlTreeEl(base, depth), NameSpace(nameSpace), Childrens(childrens), Attributes(attributes) 
	{
		ClassNameHierarchy.addEl(XML_TAG);
	}
	//std::string XmlTag::textDump(bool showFormat);
	void XmlTag::addAttribute(Attribute& source)
	{
		Attributes.addEl(source);
	}
	void XmlTag::addAttribute(UnsafeList<Attribute>& source)
	{
		Attributes += source;
	}
	void XmlTag::addAttribute(UnsafeList<Attribute>&& source)
	{
		Attributes += source;
	}
	void XmlTag::convertToCdata(bool toCdata)
	{
		Childrens.setCountFirst();
		for (unsigned i = 0; i < Childrens.length(); ++i)
		{
			A_XmlMoveable* temp = Childrens.countList(true);
			if (temp->isClassInBranch(A_XML_TREE_EL))
			{
				((A_XmlTreeEl*)temp)->convertToCdata(toCdata);
			}
		}
	}
	std::string XmlTag::textDump(bool showFormat)
	{
		std::string Ret = '<' + NameSpace;
		if (showFormat)
		{
			Ret = Parser<std::string>::makeTabulation(Depth) + Ret;
		}
		if (NameSpace.length() != 0) { Ret += ':'; }
		Ret += Base;
		Attributes.setCountFirst();
		for (unsigned i = 0; i < Attributes.length(); ++i)
		{
			Attribute temp = Attributes.countList(true);
			Ret += ' ' + temp.textDump();
		}
		if (Childrens.length() == 0)
		{
			Ret += "/";
		}
		Ret += '>';
		if (showFormat) { Ret += '\n'; }
		Childrens.setCountFirst();
		for (unsigned i = 0; i < Childrens.length(); ++i)
		{
			A_XmlMoveable *tmp = Childrens.countList(true);
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
			Ret += '/' + NameSpace;
			if (NameSpace.length() != 0) { Ret += ':'; }
			Ret += Base + '>';
			if (showFormat) { Ret += '\n'; }
		}

		return Ret;
	}
	//XmlText

	//protected
	XmlText::XmlText(std::string base, XmlTag* father, unsigned short depth, unsigned char cdataStatus,
		UnsafeList<unsigned>* toReplace) : A_XmlTreeEl(base, father, depth), CdataStatus(cdataStatus),
		ToReplace(toReplace)
	{
		ClassNameHierarchy.addEl(XML_TEXT);
	}
	//public
	XmlText::XmlText() 
	{
		ClassNameHierarchy.addEl(XML_TEXT);
	}
	XmlText::XmlText(std::string base, unsigned short depth, unsigned char cdataStatus,
		UnsafeList<unsigned>* toReplace) : A_XmlTreeEl(base, depth), ToReplace(toReplace)
	{
		ClassNameHierarchy.addEl(XML_TEXT);
		if (CHECK_CDATA_STATUS(cdataStatus)) 
		{ 
			CdataStatus = cdataStatus;
			return;
		}
		std::cerr << "Invalid CDATA status in XmlText constructor\n";
	}
	void XmlText::convertToCdata(bool toCdata)
	{
		if (toCdata)
		{
			if (CdataStatus != CDATA_ALLOWED || ToReplace->length() == 0) { return; }
			delete ToReplace;
			CdataStatus = CDATA_ONLY;
			return;
		}
		if (CdataStatus == CDATA_ONLY) 
		{
			ToReplace = XmlText::getCdataToReplaceList(Base);
		}
	}
	//textRaw return text without ampersand entities and tabs
	std::string XmlText::textRaw() 
	{ 
		if (CdataStatus != CDATA_ONLY && ToReplace)
		{
			if (ToReplace->length() == 0) { return Base; }
			std::string ret;
			unsigned j = 0;
			unsigned i = ToReplace->length() - 1;
			ToReplace->setCountFirst();
			unsigned ReplaceMark = ToReplace->countList(true);
			if (ToReplace->length() == 1) { ToReplace->setCountFirst(); }
			do
			{
				if (j == ReplaceMark)
				{
					std::string temp;
					temp += Base[j];
					ret += entityToCode[temp];
					--i;
					ReplaceMark = ToReplace->countList(true);
				}
				else
				{
					ret += Base[j];
				}
				++j;
			} while (i > 0);
			while (j < Base.length())
			{
				ret += Base[i];
				++j;
			}
			return ret;
		}
		return Base; 
	}
	//textCdata return text in cdata format
	std::string XmlText::textCdata() { return "<![CDATA[" + Base + "]]>"; }
	std::string XmlText::textDump(bool showFormat) 
	{
		if (CdataStatus == CDATA_ONLY)
		{
			return Parser<std::string>::makeTabulation(Depth) + textCdata() + '\n';
		}
		if (showFormat)
		{
			return Parser<std::string>::makeTabulation(Depth) + Base + '\n';
		}
		return Parser<std::string>::makeTabulation(Depth) + textRaw() + '\n';
	}

	UnsafeList<unsigned>* XmlText::getCdataToReplaceList(std::string& in)
	{
		UnsafeList<unsigned>* ret = new UnsafeList<unsigned>();
		for (unsigned i = 0; i < in.length(); ++i)
		{
			std::string temp;
			temp += in[i];
			if (entityToCode[temp].length() < 8)
			{
				ret->addEl(i);
			}
		}
		return ret;
	}

	//XmlPi
	//protected
	XmlPi::XmlPi(std::string base, std::string data, XmlTag* father) : A_XmlNonTreeEl(base, father), Data(data) 
	{
		ClassNameHierarchy.addEl(XML_PI);
	}
	//public
	XmlPi::XmlPi() 
	{
		ClassNameHierarchy.addEl(XML_PI);
	}
	XmlPi::XmlPi(std::string base, std::string data) : A_XmlNonTreeEl(base) 
	{
		ClassNameHierarchy.addEl(XML_PI);
	}
	std::string XmlPi::textDump(bool showFormat)
	{
		return "<?" + Base + " " + Data + "?>\n";
	}

	//XmlComm
	//protected
	XmlComm::XmlComm(std::string base, XmlTag* father) : A_XmlNonTreeEl(base, father) 
	{
		ClassNameHierarchy.addEl(XML_COMM);
	}
	//public
	XmlComm::XmlComm() 
	{
		ClassNameHierarchy.addEl(XML_COMM);
	}
	XmlComm::XmlComm(std::string base) : A_XmlNonTreeEl(base) 
	{
		ClassNameHierarchy.addEl(XML_COMM);
	}
	std::string XmlComm::textDump(bool showFormat) { return "<!--" + Base + "-->\n"; }

	//XmlDoc
	//public
	XmlDoc::XmlDoc() : Encoding(UTF_8), StandAlone(false), Version(false) 
	{
		ClassNameHierarchy.addEl(XML_DOC);
	}
	std::string XmlDoc::textDump(bool showFormat)
	{
		std::string ret;
		BeforTreeNonTreeEl.setCountFirst();
		for (unsigned i = 0; i < BeforTreeNonTreeEl.length(); ++i)
		{
			ret += BeforTreeNonTreeEl.countList(true)->textDump(showFormat);
		}
		ret += RootEl->textDump(showFormat);
		AfterTreeNonTreeEl.setCountFirst();
		for (unsigned i = 0; i < AfterTreeNonTreeEl.length(); ++i)
		{
			ret += AfterTreeNonTreeEl.countList(true)->textDump(showFormat);
		}
		return ret;
	}
}