// EC_Platform.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "head.h"


int main()
{
	Product *p = new Food;
	double res = p->getPrice("00000001");
	cout << res << endl;

	p->setDiscount(0.8);
	res = p->getPrice("00000001");
	cout << res << endl;

	/*
	bool endFlag = false;
	while (!endFlag)
	{
		cout << "1.food 2.clothes 3.books 4.buy 5.add to cart" << endl;
		cout << "6.settle the account of cart 7.log in/log out 8.sign in << endl;
		cout << 9.update info(改密码和银行卡的绑定) 0.exit" << endl;
		int n;
		cin >> n;

		switch (n)
		{
		case 1:

			break;

		case 2:

			break;

		case 3:
			
			break;

		case 7:

			break;

		case 8:

			break;

		case 0:
			endFlag = true;
			break;
		}
	}*/
    return 0;
}

