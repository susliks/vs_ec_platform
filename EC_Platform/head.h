#pragma once

#include "stdafx.h"
#include "sqlite3.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <list>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const int MAX_CARD_NUM = 5;
const int BUF_LEN = 20;
const int MSG_MAX_LEN = 1000;

string int2string(int num, int len);

struct PlatClientInfo
{
	string logId;
	//string bankCard[MAX_CARD_NUM][2];
};

struct CartProduct
{
	string productId;
	int num;
};

struct Bonus
{
	double level;
	double sub;
};

//最多绑定五张银行卡
struct ProductInfo
{
	string productId;
	string name;
	double price;
	int salesVolume;
	int	left;
	string description;
};

class Product;

class Platform
{
private:
	sqlite3 * platDb;
	int argc;
	char **argv;

	bool logFlag;
	PlatClientInfo curClient;
	vector<CartProduct> cart;	//购物车
	vector<Bonus> bonus;

	void openDB();
	void closeDB();
	bool createRecord(string logId, string password);
	bool setPassword(string logId, string newPassword);
	string getProductType(string productId);
	Product* createProduct(string type);

public:
	Platform(int argc, char **argv);

	int getArgc();
	char** getArgv();

	bool addClient();
	void clientInfoUpdate();
	bool judgeExistClientId(string logId);
	string getPassword(string logId);
	bool verifyPassword(string logId, string inputPassword);
	void showFood();
	void showClothes();
	void showBooks();
	bool addToCart();
	//两种付款方式
	bool payForProduct();	//直接购买
	bool payForCart();		//购物车结算
	bool logInOrOut();
	bool checkLeft();
	bool updateDiscount();		//品类打折
	double bonusReduction(double curMoney);	//满减优惠
	bool updateBonus();

	string ec_connect(int argc, char **argv, char* sendbuf);
	bool createCardRecord(string logId, string bankName, string cardNum);
	bool boundCardPay(string logId, double cost);
};

class Product
{
private:
	string productId;
	string name;
	double price;
	int salesVolume;
	int	left;
	string description;

protected:
	sqlite3 * db;
	void openDB();
	void closeDB();
public:
	Product();
	~Product();
	virtual double getPrice(string productId);
	virtual void setDiscount(double newDiscount) = 0;
	string getName(string productId);
	int getSalesVolume(string productId);
	bool setSalesVolume(string productId, int num);
	int getLeft(string productId);
	bool setLeft(string productId, int num);
	virtual string getDescription(string productId);
	virtual void showAllThisType() = 0;
	bool judgeExistProductId(string productId);
	double calcCost(Product *product, string productId, int num);
	double buy(Product *product, string productId, int num);

	//set操作在数据库端完成
	//TODO:这里有一些已经不是virtual
};

class Food : public Product
{
private:
	static double discount;	//类的静态变量 只有一份 对所有实例可见

public:
	double getPrice(string productId);
	void setDiscount(double newDiscount);
	void showAllThisType();

};

class Clothes : public Product
{
private:
	static double discount;	//类的静态变量 只有一份 对所有实例可见

public:
	double getPrice(string productId);
	void setDiscount(double newDiscount);
	void showAllThisType();
};

class Books : public Product
{
private:
	static double discount;	//类的静态变量 只有一份 对所有实例可见

public:
	double getPrice(string productId);
	void setDiscount(double newDiscount);
	void showAllThisType();
};

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
