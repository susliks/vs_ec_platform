#include "stdafx.h"
#include "head.h"

double Clothes::discount = 1;	//���о�̬�����ĳ�ʼ�� ��ʽ��΢��Ѱ��

double Clothes::getPrice(string productId)
{
	double result = Product::getPrice(productId);


	result = result * this->discount;
	return result;
}

void Clothes::setDiscount(double newDiscount)
{
	discount = newDiscount;
}

void Clothes::showAllThisType()
{
	openDB();
	int result;
	char * errmsg = NULL;
	char **dbResult; //�� char ** ���ͣ�����*��
	int nRow, nColumn;

	string SQLCode = "select * from product where type = \"clothes\";";
	result = sqlite3_get_table(Product::db, SQLCode.c_str(), &dbResult, &nRow, &nColumn, &errmsg);


	cout << "-----------------------------------------------" << endl;
	for (int i = 1; i <= nRow; i++)
	{
		cout << "product ID:" << dbResult[i * 7 + 0] << endl;
		cout << "type:" << dbResult[i * 7 + 1] << endl;
		cout << "name:" << dbResult[i * 7 + 2] << endl;
		cout << "price:" << dbResult[i * 7 + 3] << endl;
		cout << "sales volumn:" << dbResult[i * 7 + 4] << endl;
		cout << "left:" << dbResult[i * 7 + 5] << endl;
		cout << "description:" << dbResult[i * 7 + 6] << endl;
		cout << "-----------------------------------------------" << endl;
	}

	closeDB();
}