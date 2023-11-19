#pragma once
#include "BaseStructures.h"
namespace DTST
{
	template<typename T1, typename T2>
	class UnsafeAvlDict
	{
	protected:
		class AVLEl
		{
		public:
			T1 Key;
			T2 Value;
			AVLEl* Father, * Left, * Right;
			unsigned short Height;
			AVLEl(AVLEl* father, T1 key, T2 value) : Father(father), Key(key), Value(value) {}
			AVLEl(T1 key, T2 value) : Key(key), Value(value) {}
			AVLEl(AVLEl* el, AVLEl* father = NULL)
			{
				Father = father;
				Key = el->Key;
				Value = el->Value;
				Height = el->Height;
			}
			AVLEl* operator ->() { return this; }
		};
		//Father(father), Key(el->Key), Value(el->Value),
		//Height(el->Height)
		unsigned QuantityEl = 0, Height = 0, QuantityLeafs = 1;
		AVLEl* Root;

		static int height(AVLEl* p)
		{
			return p ? p->Height : -1;
		}
		static int equals(AVLEl* p)
		{
			return height(p->Right) - height(p->Left);
		}
		static AVLEl* keyFindEl(AVLEl* p, T1 k)
		{
			if (!p) { return NULL; }
			if (p->Key < k) { return UnsafeAvlDict::keyFindEl(p->Right, k); }
			if (p->Key > k) { return UnsafeAvlDict::keyFindEl(p->Left, k); }
			return p;
		}
		static AVLEl* findMin(AVLEl* p)
		{
			if (!p) { return NULL; }
			return p->Left ? findMin(p->Left) : p;
		}
		static AVLEl* findMax(AVLEl* p)
		{
			if (!p) { return NULL; }
			return p->Right ? findMin(p->Right) : p;
		}
		static void fixheight(AVLEl* p)
		{
			if (!p) { return; }
			int hl = height(p->Left), hr = height(p->Right);
			p->Height = (hl > hr ? hl : hr) + 1;
		}
		AVLEl* rotateLeft(AVLEl* p)
		{
			short before = 0, after = 0;
			AVLEl* q = p->Right;
			if (!q->Left && !q->Right) { ++before; }
			if (!p->Left && !p->Right) { ++before; }
			q->Father = p->Father;
			if (q->Left) { q->Left->Father = p; }
			p->Father = q;
			p->Right = q->Left;
			q->Left = p;
			fixheight(p);
			fixheight(q);
			if (!q->Left && !q->Right) { ++after; }
			if (!p->Left && !p->Right) { ++after; }
			QuantityLeafs += after - before;
			if (p == Root)
			{
				Root = q;
				Root->Father = NULL;
			}
			return q;
		}
		AVLEl* rotateRight(AVLEl* p)
		{
			short before = 0, after = 0;
			AVLEl* q = p->Left;
			if (!q->Left && !q->Right) { ++before; }
			if (!p->Left && !p->Right) { ++before; }
			q->Father = p->Father;
			if (q->Right) { q->Right->Father = p; }
			p->Father = q;
			p->Left = q->Right;
			q->Right = p;
			fixheight(p);
			fixheight(q);
			if (!q->Left && !q->Right) { ++after; }
			if (!p->Left && !p->Right) { ++after; }
			QuantityLeafs += after - before;
			if (p == Root)
			{
				Root = q;
				Root->Father = NULL;
			}
			return q;
		}
		AVLEl* balance(AVLEl* p)
		{
			fixheight(p);
			if (equals(p) == 2)
			{
				if (equals(p->Right) < 0)
					p->Right = rotateRight(p->Right);
				return rotateLeft(p);
			}
			if (equals(p) == -2)
			{
				if (equals(p->Left) > 0)
					p->Left = rotateLeft(p->Left);
				return rotateRight(p);
			}
			return p;
		}
		AVLEl* insertEl(AVLEl* p, T1 k, T2 v)
		{
			static AVLEl* father = NULL;
			if (!p)
			{
				++QuantityEl;
				if (father == NULL) { ++QuantityLeafs; }
				else if (father->Left == father->Right == NULL) { ++QuantityLeafs; }
				return new AVLEl(father, k, v);
			}
			father = p;
			if (p->Key < k) { p->Right = insertEl(p->Right, k, v); }
			else if (p->Key > k) { p->Left = insertEl(p->Left, k, v); }
			else { return NULL; }
			return balance(p);
		}
		static AVLEl* removeMin(AVLEl* p)
		{
			if (!p->Left) { return p->Right; }
			p->Left = removeMin(p->Left);
			return balance(p);
		}
		static AVLEl* removeEl(AVLEl* p)
		{
			if (!p) { return NULL; }
			AVLEl* l = p->Left, r = p->Right, f = p->Father;
			if (!l && !r) { --QuantityLeafs; }
			--QuantityEl;
			delete p;
			if (!r) { return l; }
			AVLEl* min = findMin(r);
			min->Right = removeMin(r);
			min->Left = l;
			min->Father = f;/*
			if (min->Key > f.Key) { f.Right = min; }
			else { f.Left = min; }*/
			return balance(min);
		}
	public:
		UnsafeAvlDict(UnsafeAvlDict& orig)
		{
			*this = orig;
		}
		UnsafeAvlDict(UnsafeAvlDict&& orig)
		{
			*this = orig;
		}
		//UnsafeAvlDict(UnsafeAvlDict &orig, bool iscopy = true) : UnsafeAvlDict(&orig, iscopy) { }
		UnsafeAvlDict() {}
		void delAVLDict()
		{
			QuantityLeafs = Height = 0;
			if (Root == NULL) 
			{ 
				QuantityEl = 0;
				return; 
			}
			AVLEl* temp1 = Root, * temp2;
			while (QuantityEl > 0)
			{
				if (temp1->Left)
				{
					temp1 = temp1->Left;
					temp1->Father->Left = NULL;
				}
				else if (temp1->Right)
				{
					temp1 = temp1->Right;
					temp1->Father->Right = NULL;
				}
				else
				{
					temp2 = temp1->Father;
					--QuantityEl;
					delete temp1;
					temp1 = temp2;
				}
			}
			Root = NULL;
		}
		bool addEl(T1 k, T2 v)
		{
			if (Root == NULL)
			{
				Root = insertEl(Root, k, v);
				if (Root) { return true; }
				return false;
			}
			if (insertEl(Root, k, v)) { return true; }
			return false;
		}
		T2 operator[] (T1 index) const
		{
			return keyFindEl(Root, index)->Value;
		}
		T2& operator[] (T1 index)
		{
			return keyFindEl(Root, index)->Value;
		}
		UnsafeAvlDict &operator = (UnsafeAvlDict& second)
		{
			if (!second.Root) { return *this; }
			delAVLDict();
			Root = new AVLEl(second.Root);
			AVLEl* tempTo = Root, * tempFrom = second.Root;
			for (unsigned i = 1; i <= second.QuantityEl; )
			{
				if (tempFrom->Left && !tempTo->Left)
				{
					tempTo->Left = new AVLEl(tempFrom->Left, tempTo);
					++i;
					tempTo = tempTo->Left;
					tempFrom = tempFrom->Left;
				}
				else if (tempFrom->Right && !tempTo->Right)
				{
					tempTo->Right = new AVLEl(tempFrom->Right, tempTo);
					++i;
					tempTo = tempTo->Right;
					tempFrom = tempFrom->Right;
				}
				else if (tempTo->Father)
				{
					tempTo = tempTo->Father;
					tempFrom = tempFrom->Father;
				}
				else
				{
					break;
				}
			}
			QuantityEl = second.QuantityEl;
			QuantityLeafs = second.QuantityLeafs;
			return *this;
		}
		UnsafeAvlDict& operator = (UnsafeAvlDict&& second)
		{
			if (!second.Root || (&second == this)) { return *this; }
			delAVLDict();
			QuantityEl = second.QuantityEl;
			QuantityLeafs = second.QuantityLeafs;
			Root = second.Root;
			second.Root = NULL;
			second.QuantityEl = second.QuantityLeafs = 0;
			return *this;
		}
		~UnsafeAvlDict() { delAVLDict(); }
	};

	template <typename T1, typename T2>
	class SafeAvlDict : public UnsafeAvlDict<T1, T2>
	{
	protected:
		T2 DefaultValue;
	public:
		SafeAvlDict(T2 defaultValue) : DefaultValue(defaultValue) {}
		SafeAvlDict(SafeAvlDict& orig)
		{
			DefaultValue = orig.DefaultValue;
			UnsafeAvlDict<T1, T2>::operator = (orig);
		}
		SafeAvlDict(SafeAvlDict&& orig)
		{
			DefaultValue = orig.DefaultValue;
			UnsafeAvlDict<T1, T2>::operator = (orig);
		}
		SafeAvlDict& operator = (SafeAvlDict& second)
		{
			DefaultValue = second.DefaultValue;
			UnsafeAvlDict<T1, T2>::operator = (second);
		}
		SafeAvlDict& operator = (SafeAvlDict&& second)
		{
			DefaultValue = second.DefaultValue;
			UnsafeAvlDict<T1, T2>::operator = (second);
		}
		T2 operator[] (T1 index) const
		{
			typename UnsafeAvlDict<T1, T2>::AVLEl *temp(this->keyFindEl(this->Root, index));
			if (temp) { return temp->Value; }
			return DefaultValue;
		}
		T2& operator[] (T1 index)
		{
			typename UnsafeAvlDict<T1, T2>::AVLEl* temp(this->keyFindEl(this->Root, index));
			if (temp) { return temp->Value; }
			return DefaultValue;
		}
	};

	template<typename T>
	class UnsafeList
	{
	protected:
		class ListEl
		{
		public:
			T content;
			ListEl* Prev, * Next;
			ListEl(T In) : content(In) {}
			ListEl() {}
		};
		unsigned Length;
		ListEl* First = NULL, * Last = NULL;
		mutable ListEl* Current = NULL;
		void insertEl(ListEl* el, ListEl* key, bool order);
		ListEl* findEl(T key, unsigned num = 1);
		void delEl(ListEl* del);
		void delAll();
		void resetCounter(bool edge);
	public:
		UnsafeList();
		UnsafeList(UnsafeList& orig);
		UnsafeList(UnsafeList&& orig);
		//void* getLastElID() { return (void*)Last; }
		//void* getFirstElID() { return (void*)First; }
		void addEl(T el);
		T popEl();
		T dumpEl();
		T& getLastEl();
		T& getFirstEl();
		unsigned length();
		T& operator [] (long index);
		UnsafeList& operator = (const UnsafeList& second);
		UnsafeList& operator = (const UnsafeList&& second);
		UnsafeList operator += (UnsafeList& second);
		UnsafeList& operator += (UnsafeList&& second);
		UnsafeList operator + (const UnsafeList& second);
		UnsafeList operator + (const UnsafeList&& second);
		bool isInList(T key);
		T& countList(bool direction);
		void setCountFirst();
		void setCountLast();
		~UnsafeList();
	};

	template <typename T>
	class SafeList : public UnsafeList<T>
	{
	protected:
		T DefaultValue;
	public:
		SafeList(T defaultValue);
		T popEl();
		T dumpEl();
		T& getLastEl();
		T& getFirstEl();
		T& operator [] (long index);
		SafeList& operator = (const UnsafeList<T>& second);
		SafeList& operator = (const UnsafeList<T>&& second);
		SafeList operator + (const SafeList<T>& second);
		SafeList operator + (const SafeList<T>&& second);
		SafeList operator + (const UnsafeList<T>& second);
		SafeList operator + (const UnsafeList<T>&& second);
		T& countList(bool direction);
	};

	//UnsafeList
	//protected
	template<typename T>
	void UnsafeList<T>::insertEl(ListEl* el, ListEl* key, bool order)
	{
		++Length;
		if (order)
		{
			if (key->Next != NULL)
			{
				key->Next->Prev = el;
				el->Next = key->Next;
			}
			else
			{
				Last = el;
			}
			key->Next = el;
			el->Prev = key;
		}
		else
		{
			if (key->Prev != NULL)
			{
				key->Prev->Next = el;
				el->Prev = key->Prev;
			}
			else
			{
				First = el;
			}
			key->Prev = el;
			el->Next = key;
		}
	}
	template<typename T>
	typename UnsafeList<T>::ListEl* UnsafeList<T>::findEl(T key, unsigned num)
	{
		unsigned i = 0;
		ListEl* temp = Current;
		while (i < Length)
		{
			if (Current->content == key)
			{
				if (--num == 0) { return Current; }
			}
			if (Current == Last) { break; }
			Current = Current->Next;
			++i;
		}
		Current = temp;
		return NULL;
	}
	template<typename T>
	void UnsafeList<T>::delEl(ListEl* del)
	{
		if (del == Current)
		{
			if (Current->Prev) { Current = Current->Prev; }
			else { Current = Current->Next; }
		}
		if (del == Last)
		{
			Last = Last->Prev;
		}
		if (del == First)
		{
			First = First->Next;
		}
		if (del == NULL)
		{
			return;
		}
		if (del->Next == NULL)
		{
			if (del->Prev == NULL) {}
			else { del->Prev->Next = NULL; }
		}
		else
		{
			if (del->Prev == NULL) { del->Next->Prev = NULL; }
			else
			{
				del->Prev->Next = del->Next;
				del->Next->Prev = del->Prev;
			}
		}
		--Length;
		delete del;
		del = NULL;
	}
	template<typename T>
	void  UnsafeList<T>::delAll()
	{
		while (Length > 0)
		{
			delEl(Last);
		}
	}
	template<typename T>
	void UnsafeList<T>::resetCounter(bool edge)
	{
		if (edge) { Current = First; }
		else { Current = Last; }
	}
	
	//public
	template<typename T>
	UnsafeList<T>::UnsafeList() {}
	template<typename T>
	UnsafeList<T>::UnsafeList(UnsafeList& orig)
	{
		*this = orig;
	}
	template<typename T>
	UnsafeList<T>::UnsafeList(UnsafeList&& orig)
	{
		*this = orig;
	}
	//void* getLastElID() { return (void*)Last; }
	//void* getFirstElID() { return (void*)First; }
	template<typename T>
	void UnsafeList<T>::addEl(T el)
	{
		ListEl* temp = new ListEl(el);
		if (Length == 0)
		{
			Last = First = Current = temp;
			++Length;
		}
		else { insertEl(temp, Last, true); }
	}
	template<typename T>
	T UnsafeList<T>::popEl()
	{
		T temp = Last->content;
		delEl(Last);
		return temp;
	}
	template<typename T>
	T UnsafeList<T>::dumpEl()
	{
		T temp = First->content;
		delEl(First);
		return temp;
	}
	template<typename T>
	T& UnsafeList<T>::getLastEl() { return Last->content; }
	template<typename T>
	T& UnsafeList<T>::getFirstEl() { return First->content; }
	template<typename T>
	unsigned UnsafeList<T>::length() { return Length; }
	template<typename T>
	T& UnsafeList<T>::operator [] (long index)
	{
		if (index >= Length && index < (Length * -1))
		{
			std::cerr << "Index out of List's bounds";
			return NULL;
		}
		ListEl* temp;
		if (index < 0)
		{
			temp = Last;
			while (index != -1)
			{
				temp = temp->Prev;
				++index;
			}
		}
		else
		{
			temp = First;
			while (index != 0)
			{
				temp = temp->Next;
				--index;
			}
		}
		return temp->content;
	}
	template<typename T>
	UnsafeList<T>& UnsafeList<T>::operator = (const UnsafeList& second)
	{
		if (Length != 0)
		{
			delAll();
		}
		ListEl* temp = second.Current;
		second.Current = second.First;
		while (Length != second.Length)
		{
			addEl(second.Current->content);
			if (temp == second.Current) { Current = Last; }
			second.Current = second.Current->Next;
		}
		second.Current = temp;
		return *this;
	}
	template<typename T>
	UnsafeList<T>& UnsafeList<T>::operator = (const UnsafeList&& second)
	{
		if (&second == this) { return *this; }
		if (Length != 0) { delAll(); }
		First = second.First;
		Last = second.Last;
		Current = second.Current;
		Length = second.Length;
		second.First = second.Last = second.Current = NULL;
		second.Length = 0;
		return *this;
	}
	template<typename T>
	UnsafeList<T> UnsafeList<T>::operator += (UnsafeList& second)
	{
		ListEl* temp = second.Current;
		second.Current = second.First;
		for (unsigned i = 0; i < second.Length; ++i)
		{
			addEl(second.countList(true));
		}
		second.Current = temp;
		return *this;
	}
	template<typename T>
	UnsafeList<T>& UnsafeList<T>::operator += (UnsafeList&& second)
	{
		if (this->length == 0)
		{
			*this = second;
			return *this;
		}
		if (second.Length != 0)
		{
			this->Length += second.Length;
			this->Last->Next = second.First;
			second.First->Prev = this->Last;
			this->Last = second.Last;
			second.First = second.Last = second.Current = NULL;
			second.Length = 0;
		}
		return *this;
	}
	template<typename T>
	UnsafeList<T> UnsafeList<T>::operator + (const UnsafeList& second)
	{
		UnsafeList ret = *this;
		ret += second;
		return ret;
	}
	template<typename T>
	UnsafeList<T> UnsafeList<T>::operator + (const UnsafeList&& second)
	{
		UnsafeList ret = *this;
		ret += second;
		return ret;
	}
	template<typename T>
	bool UnsafeList<T>::isInList(T key)
	{
		if (UnsafeList<T>::findEl(key)) { return true; }
		return false;
	}
	template<typename T>
	T& UnsafeList<T>::countList(bool direction)
	{
		ListEl* temp = Current;
		if (direction) { Current = Current->Next; }
		else { Current = Current->Prev; }
		return temp->content;
	}
	template<typename T>
	void UnsafeList<T>::setCountFirst() { resetCounter(true); }
	template<typename T>
	void UnsafeList<T>::setCountLast() { resetCounter(false); }
	template<typename T>
	UnsafeList<T>::~UnsafeList()
	{
		delAll();
	}

	//SafeList
	//public
	template<typename T>
	SafeList<T>::SafeList(T defaultValue) : DefaultValue(defaultValue) {}
	template<typename T>
	T SafeList<T>::popEl()
	{
		if (this->Last == NULL) { return DefaultValue; }
		T temp = this->Last->content;
		delEl(this->Last);
		return temp;
	}
	template<typename T>
	T SafeList<T>::dumpEl()
	{
		if (this->First == NULL) { return DefaultValue; }
		T temp = this->First->content;
		delEl(this->First);
		return temp;
	}
	template<typename T>
	T& SafeList<T>::getLastEl()
	{
		if (this->Last) { return this->Last->content; }
		return this->DefaultValue;
	}
	template<typename T>
	T& SafeList<T>::getFirstEl()
	{
		if (this->First) { return this->First->content; }
		return this->DefaultValue;
	}
	template<typename T>
	T& SafeList<T>::operator [] (long index)
	{
		if (index >= this->Length && index < (this->Length * -1))
		{
			std::cerr << "Index out of List's bounds";
			return DefaultValue;
		}
		return UnsafeList<T>::operator [] (index);
	}
	template<typename T>
	SafeList<T>& SafeList<T>::operator = (const UnsafeList<T>& second)
	{
		UnsafeList<T>::operator = (second);
		DefaultValue = second.DefaultValue;
		return *this;
	}
	template<typename T>
	SafeList<T>& SafeList<T>::operator = (const UnsafeList<T>&& second)
	{
		UnsafeList<T>::operator = (second);
		DefaultValue = second.DefaultValue;
		return *this;
	}
	template<typename T>
	SafeList<T> SafeList<T>::operator + (const SafeList<T>& second)
	{
		SafeList<T> ret = *this;
		ret += second;
		return ret;
	}
	template<typename T>
	SafeList<T> SafeList<T>::operator + (const SafeList<T>&& second) 
	{
		SafeList<T> ret = *this;
		ret += second;
		return ret;
	}
	template<typename T>
	SafeList<T> SafeList<T>::operator + (const UnsafeList<T>& second)
	{
		SafeList<T> ret = *this;
		ret += second;
		return ret;
	}
	template<typename T>
	SafeList<T> SafeList<T>::operator + (const UnsafeList<T>&& second)
	{
		SafeList<T> ret = *this;
		ret += second;
		return ret;
	}

	template<typename T>
	T& SafeList<T>::countList(bool direction)
	{
		if (this->Current == NULL) { return DefaultValue; }
		typename UnsafeList::ListEl* temp = this->Current;
		if (direction && this->Current->Next) { this->Current = this->Current->Next; }
		else if (direction == false && this->Current->Prev) { this->Current = this->Current->Prev; }
		return temp->content;
	}
}


