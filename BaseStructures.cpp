#include "BaseStructures.h"

namespace DTST
{

	Mark::Mark(unsigned minPosition, unsigned maxPosition)
	{
		if (minPosition < maxPosition)
		{
			Position = MinPosition = minPosition;
			MaxPosition = maxPosition;
		}
		else
		{
			std::cerr << "Attempt to create mark with start position after end position\n";
		}
	}
	Mark& Mark::operator ++ ()
	{
		if (Position < MaxPosition) { ++Position; }
		return *this;
	}
	Mark Mark::operator ++ (int)
	{
		if (Position >= MaxPosition)
		{
			return *this;
		}
		Mark OldPosition = *this;
		++Position;
		return OldPosition;
	}
	Mark& Mark::operator -- ()
	{
		if (Position > MinPosition) { --Position; }
		return *this;
	}
	Mark Mark::operator -- (int)
	{
		if (Position <= MinPosition)
		{
			return *this;
		}
		Mark OldPosition(*this);
		--Position;
		return OldPosition;
	}
	Mark& Mark::operator = (Mark& second)
	{
		this->MaxPosition = second.MaxPosition;
		this->MinPosition = second.MinPosition;
		this->Position = second.Position;
		return *this;
	}
	unsigned Mark::getPos() { return Position; }
	bool Mark::setPos(unsigned NewPosition)
	{
		if (NewPosition > MaxPosition || NewPosition < MinPosition) { return false; }
		Position = NewPosition;
		return true;
	}
	unsigned Mark::getMaxPos() { return MaxPosition; }
	bool Mark::setMaxPos(unsigned NewPosition)
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
	unsigned Mark::getMinPos() { return MinPosition; }
	bool Mark::setMinPos(unsigned NewPosition)
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
	Attribute::Attribute(std::string Ns, std::string N, std::string V) : Namespace(Ns), Name(N), Value(V) {}
	Attribute::Attribute(std::string N, std::string V) : Name(N), Value(V) {}
	Attribute::Attribute() {}
	std::string Attribute::nameSpace() { return Namespace; }
	std::string Attribute::name() { return Name; }
	std::string Attribute::value() { return Value; }
	void Attribute::setNameSpace(std::string Ns) { Namespace = Ns; }
	void Attribute::setName(std::string N) { Name = N; }
	void Attribute::setValue(std::string V) { Value = V; }
	std::string Attribute::textDump()
	{
		std::string Ret = Name + '=' + '"' + Value + '"';
		return Namespace.length() == 0 ? Ret : Namespace + ':' + Ret;
	}

	SuperInputStream::SuperInputStream(std::string filePath) : In(filePath, std::ios::in | std::ios::ate)
	{
		if (In.is_open())
		{
			Length = In.tellg();
			return;
		}
		else { std::cerr << "Unable to open file\n"; }
	}
	char SuperInputStream::operator [] (unsigned index)
	{
		if (index >= Length) { std::cout << "Index out of File's bounds\n"; }
		else 
		{
			In.seekg(index);
			return In.get();
		}
	}
	unsigned SuperInputStream::length() { return Length; }

};
