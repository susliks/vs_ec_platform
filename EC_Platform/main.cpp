// EC_Platform.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "head.h"

string int2string(int num, int len)	//返回一个长度为len的数字num对应的string
{
	char tmpCh[25];
	sprintf(tmpCh, "%d", num);
	string tmpString = tmpCh;
	int deltaLen = len - tmpString.length();
	for (int i = 0; i < deltaLen; i++)
	{
		tmpString = "0" + tmpString;
	}
	return tmpString;
}

int main()
{
	/*Product *p = new Food;
	double res = p->getPrice("00000001");
	cout << res << endl;

	p->setDiscount(0.8);
	res = p->getPrice("00000001");
	cout << res << endl;*/
	Platform plat;
	
	bool endFlag = false;
	while (!endFlag)
	{
		cout << "1.food 2.clothes 3.books 4.buy 5.add to cart" << endl;
		cout << "6.settle the account of cart 7.log in/log out 8.sign in" << endl;
		cout << "9.update info(改密码和银行卡的绑定) 0.exit" << endl;
		int n;
		cin >> n;

		switch (n)
		{
		case 1:
			plat.showFood();
			break;

		case 2:

			break;

		case 3:
			
			break;

		case 4:
			plat.payForProduct();
			break;

		case 5:
			plat.addClient();
			break;

		case 6:
			plat.payForCart();
			break;

		case 7:
			plat.logInOrOut();
			break;

		case 8:
			plat.addClient();
			break;

		case 9:
			plat.clientInfoUpdate();

		case 0:
			endFlag = true;
			break;
		}
	}
    return 0;
}

