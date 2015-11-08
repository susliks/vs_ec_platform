#include "stdafx.h"
#include "head.h"

double Food::discount = 1;	//���о�̬�����ĳ�ʼ�� ��ʽ��΢��Ѱ��

double Food::getPrice(string productId)
{
	double result = Product::getPrice(productId);
	result = result * this->discount;
	return result;
}

void Food::setDiscount(double newDiscount)
{
	discount = newDiscount;
}

void Food::showAllThisType()
{
	openDB();
	int result;
	char * errmsg = NULL;
	char **dbResult; //�� char ** ���ͣ�����*��
	int nRow, nColumn;

	string SQLCode = "select * from product where type = \"food\";";
	result = sqlite3_get_table(Product::db, SQLCode.c_str(), &dbResult, &nRow, &nColumn, &errmsg);


	cout << "-----------------------------------------------";
	for (int i = 1; i <= nRow; i++)
	{
		cout << "product ID:" << dbResult[i * 7 + 0] << endl;
		cout << "type:" << dbResult[i * 7 + 1] << endl;
		cout << "name:" << dbResult[i * 7 + 2] << endl;
		cout << "price:" << dbResult[i * 7 + 3] << endl;
		cout << "sales volumn:" << dbResult[i * 7 + 4] << endl;
		cout << "left:" << dbResult[i * 7 + 5] << endl;
		cout << "description:" << dbResult[i * 7 + 6] << endl;
	}

	closeDB();
}