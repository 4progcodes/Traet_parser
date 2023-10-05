#pragma once
#include <iostream>
namespace DTST
{
	class Mark
	{
		unsigned Position, MinPosition, MaxPosition;
	public:
		Mark(unsigned minPosition, unsigned maxPosition);
		Mark& operator ++ ();
		Mark operator ++ (int);
		Mark& operator -- ();
		Mark operator -- (int);
		Mark& operator = (Mark& second);
		unsigned getPosition();
		bool setPosition(unsigned NewPosition);
		unsigned getMaxPosition();
		bool setMaxPosition(unsigned NewPosition);
		unsigned getMinPosition();
		bool setMinPosition(unsigned NewPosition);
	};

	class Attribute
	{
	protected:
		std::string Namespace = "", Name = "", Value = "";
	public:
		Attribute(std::string Ns, std::string N, std::string V);
		Attribute(std::string N, std::string V);
		Attribute();
		std::string nameSpace();
		std::string name();
		std::string value();
		void setNameSpace(std::string Ns);
		void setName(std::string N);
		void setValue(std::string V);
		std::string textDump();
	};
}