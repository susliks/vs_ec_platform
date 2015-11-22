// EC_Platform.cpp : �������̨Ӧ�ó������ڵ㡣
//

//TODO:�������new��open������û�м�ʱ�ر�

#include "stdafx.h"
#include "head.h"

Platform::Platform(int argc, char **argv)
{
	this->argc = argc;
	this->argv = argv;
}

string int2string(int num, int len)	//����һ������Ϊlen������num��Ӧ��string
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

int main(int argc, char **argv)
{
	/*Product *p = new Food;
	double res = p->getPrice("00000001");
	cout << res << endl;

	p->setDiscount(0.8);
	res = p->getPrice("00000001");
	cout << res << endl;*/
	Platform plat(argc, argv);

	//string tmpMsg = "ICBC" + '\0' + "222" + "\0"


	/*char sendMsg[] = "ICBC\1!00000006\1!123\1!10";
	string getStr = plat.ec_connect(plat.getArgc(), plat.getArgv(), sendMsg);
	cout << getStr.c_str() << endl;*/
	
	bool endFlag = false;
	while (!endFlag)
	{
		cout << "1.food 2.clothes 3.books 4.buy 5.add to cart" << endl;
		cout << "6.settle the account of cart 7.log in/log out 8.sign in" << endl;
		cout << "9.update info(����������п��İ�) 0.exit" << endl;
		int n;
		cin >> n;

		switch (n)
		{
		case 1:
			plat.showFood();
			break;

		case 2:
			plat.showClothes();
			break;

		case 3:
			plat.showBooks();
			break;

		case 4:
			plat.payForProduct();
			break;

		case 5:
			plat.addToCart();
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
			break;

		case 0:
			endFlag = true;
			break;
		}
	}
    return 0;
}

