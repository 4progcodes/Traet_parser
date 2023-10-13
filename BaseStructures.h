#pragma once
#include <iostream>
#include <fstream>
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
		unsigned getPos();
		bool setPos(unsigned NewPosition);
		unsigned getMaxPos();
		bool setMaxPos(unsigned NewPosition);
		unsigned getMinPos();
		bool setMinPos(unsigned NewPosition);
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

	class SuperInputStream
	{
	protected:
		unsigned Length;
		std::ifstream In;
	public:
		SuperInputStream(std::string filePath);
		char operator [] (unsigned index);
		unsigned length();
	};
}