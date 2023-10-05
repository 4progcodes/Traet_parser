#include <iostream>
#include <fstream>
#include "Tryet.h"

using namespace std;


//DTST::AVLDict<string, string> crt()
//{
//
//}

int main()
{
	string a = "<w:p w14:paraId=\"055D6CC6\" w14:textId=\"3080C9E9\" w:rsidR=\"00C41096\" w:rsidRPr=\"00AB730C\" w:rsidRDefault=\"00745B3E\"><w:pPr><w:rPr><w:lang w:val=\"en-US\"/></w:rPr></w:pPr></w:p>";
	DTST::XmlTree b(&a);
	cout << b.textDump(true);
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
	//if (a.length() == b.textDump(false).length()) { cout << "ass"; }
	//cout << b.textDump(true);
}