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

using namespace std;

const int MAX_CARD_NUM = 5;
const int BUF_LEN = 20;

struct PlatClientInfo
{
	string logId;
	string bankCard[MAX_CARD_NUM][2];
};

//最多绑定五张银行卡
struct productInfo
{
	string productId;
	string name;
	double price;
	int salesVolume;
	int	left;
	string description;
};

class Platform
{
private:
	sqlite3 * clientDb;

	bool logFlag;
	PlatClientInfo curClient;
	queue<string> cart;

	void openDB();
	void closeDB();
	bool createRecord(string logId, string password);
	bool setPassword(string logId, string newPassword);
	
public:
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
	virtual string getName(string productId);
	virtual int getSalesVolume(string productId);
	virtual int getLeft(string productId);
	virtual string getDescription(string productId);
	virtual void showAllThisType() = 0;
	bool judgeExistProductId(string productId);
	//set操作在数据库端完成
	//TODO:这里有一些已经不是virtual
};

class Food : public Product
{
private:
	static double discount;

public:
	double getPrice(string productId);
	void setDiscount(double newDiscount);
	void showAllThisType();

};

class Clothes : public Product
{

};

class Books : public Product
{

};


