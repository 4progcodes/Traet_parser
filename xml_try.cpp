#include <iostream>
#include <fstream>
#include "Tryet.h"
using namespace std;


//DTST::UnsafeAvlDict<string, string> crt()
//{
//
//}

//to do list
//in every textDump make \n symbol every 100 (?) symbols

using namespace DTST;
using namespace std;

int main()
{
	SuperInputStream is("D:\\document.xml");
	UnsafeXmlParser<SuperInputStream> testParser(is);
	cout << testParser.getDoc()->textDump(true);
	//DTST::SuperInputStream is("D:\\document.xml");
	//DTST::XmlTree b(&is);
	//string a = "", a1 = b.textDump(false);
	//cout << b.textDump(true) << endl << endl;
	//for (int i = 0; i < is.length(); ++i)
	//{
	//	a += is[i];
	//}
	//for (int i = 0; i < is.length(); ++i)
	//{
	//	if (a[i] == a1[i]) { cout << a1[i]; }
	//	else { cout << "\t\t\tHERE (orig : " << a[i] << "copy: " << a1[i] << ")"; }
	//}
	//if (a == a1) { cout << "ass\n"; }
	///*string c = b.textdump(false);
	//int l = a.length();
	//if (l > c.length())
	//{
	//	l = c.length();
	//}
	//for (int i = 0; i < l; ++i)
	//{
	//	if (a[i] == c[i]) { cout << a[i]; }
	//	else { cout << "*HERE-> (orig - '" << a[i] << "' , copy - '" << c[i] << "')*"; }
	//}*/
	////if (a.length() == b.textDump(false).length()) { cout << "ass"; }
	////cout << b.textDump(true);
}