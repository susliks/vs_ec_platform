#include "stdafx.h"
#include "head.h"

void Product::openDB()
{
	int result;
	
	result = sqlite3_open("productData.db", &db);
}

void Product::closeDB()
{
	sqlite3_close(db);
}

Product::Product()
{

}

Product::~Product()
{

}

double Product::getPrice(string productId)
{
	openDB();
	int result;
	char * errmsg = NULL;
	char **dbResult; //是 char ** 类型，两个*号
	int nRow, nColumn;

	/*string SQLCode = "select * from food as th, books as de where th.productId = \""
		+ productId + "\" or de.productId = \""
		+ productId + "\";";*/
	string SQLCode = "select * from product where productId = \""
				+ productId + "\";";
	result = sqlite3_get_table(db, SQLCode.c_str(), &dbResult, &nRow, &nColumn, &errmsg);

	//cout << nRow << endl;
	//cout << errmsg << endl;
	//cout << dbResult[10] << endl;
	closeDB();

	if (nRow > 0)
		return atof(dbResult[10]);
	else
		return -1;
}

string Product::getName(string productId)
{
	openDB();
	int result;
	char * errmsg = NULL;
	char **dbResult; //是 char ** 类型，两个*号
	int nRow, nColumn;

	string SQLCode = "select * from product where productId = \""
		+ productId + "\";";
	result = sqlite3_get_table(db, SQLCode.c_str(), &dbResult, &nRow, &nColumn, &errmsg);

	closeDB();
	if (nRow > 0)
		return (string)dbResult[9];
	else
		return (string)"no name";
}

int Product::getSalesVolume(string productId)
{
	openDB();
	int result;
	char * errmsg = NULL;
	char **dbResult; //是 char ** 类型，两个*号
	int nRow, nColumn;

	string SQLCode = "select * from product where productId = \""
		+ productId + "\";";
	result = sqlite3_get_table(db, SQLCode.c_str(), &dbResult, &nRow, &nColumn, &errmsg);

	closeDB();
	if (nRow > 0)
		return atoi(dbResult[11]);
	else
		return -1;
}

bool Product::setSalesVolume(string productId, int num)
{
	openDB();

	string snum = int2string(num, 0);

	int result;
	char * errmsg = NULL;

	string SQLCode = "update product set salesVolume = " + snum + " where productId = \"" + productId + "\"";
	result = sqlite3_exec(db, SQLCode.c_str(), 0, 0, &errmsg);

	closeDB();

	if (result == SQLITE_OK)
		return true;
	else
		return false;
}

int Product::getLeft(string productId)
{
	openDB();
	int result;
	char * errmsg = NULL;
	char **dbResult; //是 char ** 类型，两个*号
	int nRow, nColumn;

	string SQLCode = "select * from product where productId = \""
		+ productId + "\";";
	result = sqlite3_get_table(db, SQLCode.c_str(), &dbResult, &nRow, &nColumn, &errmsg);

	closeDB();
	if (nRow > 0)
		return atoi(dbResult[12]);
	else
		return -1;
}

bool Product::setLeft(string productId, int num)
{
	openDB();

	string snum = int2string(num, 0);

	int result;
	char * errmsg = NULL;

	string SQLCode = "update product set left = " + snum + " where productId = \"" + productId + "\"";
	result = sqlite3_exec(db, SQLCode.c_str(), 0, 0, &errmsg);

	closeDB();

	if (result == SQLITE_OK)
		return true;
	else
		return false;
}

string Product::getDescription(string productId)
{
	openDB();
	int result;
	char * errmsg = NULL;
	char **dbResult; //是 char ** 类型，两个*号
	int nRow, nColumn;

	string SQLCode = "select * from product where productId = \""
		+ productId + "\";";
	result = sqlite3_get_table(db, SQLCode.c_str(), &dbResult, &nRow, &nColumn, &errmsg);

	closeDB();
	if (nRow > 0)
		return (string)dbResult[9];
	else
		return (string)"no description";
}

bool Product::judgeExistProductId(string productId)
{
	openDB();
	openDB();
	int result;
	char * errmsg = NULL;
	char **dbResult; //是 char ** 类型，两个*号
	int nRow, nColumn;

	string SQLCode = "select * from product where productId = \""
		+ productId + "\";";
	result = sqlite3_get_table(db, SQLCode.c_str(), &dbResult, &nRow, &nColumn, &errmsg);

	closeDB();
	if (nRow > 0)
		return true;
	else
		return false;
	closeDB();
}

double Product::calcCost(Product *product, string productId, int num)
{
	int preLeft = product->getLeft(productId);
	if (num > preLeft)
	{
		cout << "库存不足" << endl;
		return -1;	//错误返回
	}
	
	return product->getPrice(productId) * num;
}

double Product::buy(Product *product, string productId, int num)
{
	int preLeft = product->getLeft(productId);
	if (num > preLeft)
	{
		cout << "库存不足" << endl;
		return -1;	//错误返回
	}
	int preSalesVolume = product->getSalesVolume(productId);
	int curLeft = preLeft - num;
	int curSalesVolume = preSalesVolume + num;

	product->setSalesVolume(productId, curSalesVolume);
	product->setLeft(productId, curLeft);

	return product->getPrice(productId) * num;
}


/*
string Product::getName(string productId)
{
	openDB();

	closeDB();
}
*/


