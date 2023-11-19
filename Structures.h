#pragma once
#include "TemplateEntities.cpp"
#include "DTST.h"

namespace DTST
{
	// A_XmlEl - base abstract class for any XML element (document, tag, PI, DTD, comment, text).
	class A_XmlEl
	{
		template <typename T1>
		friend class UnsafeXmlParser;
	protected:
		SafeList<unsigned short> ClassNameHierarchy = SafeList<unsigned short> (XML_CLASS_ERROR);
		A_XmlEl();
	public:
		bool isClassInBranch(unsigned short className);
		SafeList<unsigned short> getClassBranch();
		virtual std::string textDump(bool showFormat) = 0;
	};

	// A_BaseXmlEl - base abstract class for XML element with some base string(tag, PI, DTD, comment, text).
	class A_BaseXmlEl : public A_XmlEl
	{
		template <typename T1>
		friend class UnsafeXmlParser;
	protected:
		std::string Base;
		A_BaseXmlEl();
		A_BaseXmlEl(std::string base);
	};

	//XmlDtd using for DTD only. It is not handle DTD - only save DTD of XML document.
	class XmlDtd : public A_BaseXmlEl
	{
		template <typename T1>
		friend class UnsafeXmlParser;
	protected:
		std::string MarkingSourse;
	public:
		XmlDtd();
		XmlDtd(std::string base, std::string MarkingSourse);
		std::string textDump(bool showFormat) override;
	};

	//A_XmlTreeEl - abstract class for moveable XML elements (comment, tag, text, PI)
	class A_XmlMoveable : public A_BaseXmlEl
	{
		template <typename T1>
		friend class UnsafeXmlParser;
	protected:
		void* Father = NULL;
		A_XmlMoveable(std::string base, void* father);
		A_XmlMoveable();
		A_XmlMoveable(std::string base);
	public:
		virtual unsigned short depth();
	};

	class A_XmlTreeEl : public A_XmlMoveable
	{
		template <typename T1>
		friend class UnsafeXmlParser;
	protected:
		unsigned short Depth;
		A_XmlTreeEl(std::string base, void* father, unsigned short depth);
		A_XmlTreeEl();
		A_XmlTreeEl(std::string base, unsigned short depth);

	public:
		unsigned short depth() override;
		virtual void convertToCdata(bool toCdata) = 0;
	};

	class A_XmlNonTreeEl : public A_XmlMoveable
	{
		template <typename T1>
		friend class UnsafeXmlParser;
	protected:
		A_XmlNonTreeEl();
		A_XmlNonTreeEl(std::string base, void* father);
		A_XmlNonTreeEl(std::string base);
	};
	//XmlTag using for tags in XML. It can contain children tags, text, or attributes
	class XmlTag : public A_XmlTreeEl
	{
		template <typename T1>
		friend class UnsafeXmlParser;
	protected:
		UnsafeList<A_XmlMoveable*> Childrens;
		std::string NameSpace = "";
		UnsafeList<Attribute> Attributes;
		XmlTag(std::string base, XmlTag* father, unsigned short depth, std::string nameSpace, 
			UnsafeList<A_XmlMoveable*>& childrens, UnsafeList<Attribute>& attributes);
	public:
		XmlTag();
		XmlTag(std::string base, unsigned short depth, std::string nameSpace, 
			UnsafeList<A_XmlMoveable*>& childrens, UnsafeList<Attribute>& attributes);
		void addAttribute(Attribute& source);
		void addAttribute(UnsafeList<Attribute>& source);
		void addAttribute(UnsafeList<Attribute>&& source);
		void convertToCdata(bool toCdata) override;
		std::string textDump(bool showFormat) override;
	};

	class XmlText : public A_XmlTreeEl
	{
		template <typename T1>
		friend class UnsafeXmlParser;
	protected:
		UnsafeList<unsigned>* ToReplace = NULL;
		unsigned char CdataStatus;
		XmlText(std::string base, XmlTag* father, unsigned short depth, unsigned char cdataStatus, UnsafeList<unsigned>* toReplace = NULL);
	public:
		XmlText();
		XmlText(std::string base, unsigned short depth, unsigned char cdataStatus, UnsafeList<unsigned>* toReplace = NULL);
		void convertToCdata(bool toCdata) override;
		//textRaw return text without ampersand entities and tabs
		std::string textRaw();
		//textCdata return text in cdata format
		std::string textCdata();
		//textDump return text without ampersand entities with tabs (if showFormat == true) 
		// or without tabs with ampersand entities (if showFormat == false) 
		std::string textDump(bool showFormat) override;
		//static
		UnsafeList<unsigned>* getCdataToReplaceList(std::string& in);
	};

	//XmpPi using for any PI except XML declaration. XML declaration will handle in constructor of class XmlDoc.
	class XmlPi : public A_XmlNonTreeEl
	{
		template <typename T1>
		friend class UnsafeXmlParser;
	protected:
		std::string Data;
		XmlPi(std::string base, std::string data, XmlTag* father);
	public:
		XmlPi();
		XmlPi(std::string base, std::string data);
		std::string textDump(bool showFormat) override;
	};

	class XmlComm : public A_XmlNonTreeEl
	{
		template <typename T1>
		friend class UnsafeXmlParser;
	protected:
		XmlComm(std::string base, XmlTag* father);
	public:
		XmlComm();
		XmlComm(std::string base);
		std::string textDump(bool showFormat) override;
	};

	class XmlDoc : public A_XmlEl
	{
		template <typename T1>
		friend class UnsafeXmlParser;
	protected:
		unsigned short Encoding;
		bool StandAlone, Version; //version 1.0 = false
		XmlDtd* DocDTD;
		UnsafeList<A_XmlNonTreeEl*> BeforTreeNonTreeEl, AfterTreeNonTreeEl;
		XmlTag* RootEl;
	public:
		XmlDoc();
		std::string textDump(bool showFormat) override;
	};
}