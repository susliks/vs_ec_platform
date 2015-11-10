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

string int2string(int num, int len);

struct PlatClientInfo
{
	string logId;
	string bankCard[MAX_CARD_NUM][2];
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

//�����������п�
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

	bool logFlag;
	PlatClientInfo curClient;
	vector<CartProduct> cart;	//���ﳵ
	vector<Bonus> bonus;

	void openDB();
	void closeDB();
	bool createRecord(string logId, string password);
	bool setPassword(string logId, string newPassword);
	string getProductType(string productId);
	Product* createProduct(string type);

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
	//���ָ��ʽ
	bool payForProduct();	//ֱ�ӹ���
	bool payForCart();		//���ﳵ����
	bool logInOrOut();
	bool checkLeft();
	bool updateDiscount();		//Ʒ�����
	double bonusReduction(double curMoney);	//�����Ż�
	bool updateBonus();
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
	double buy(Product *product, string productId, int num);

	//set���������ݿ�����
	//TODO:������һЩ�Ѿ�����virtual
};

class Food : public Product
{
private:
	static double discount;	//��ľ�̬���� ֻ��һ�� ������ʵ���ɼ�

public:
	double getPrice(string productId);
	void setDiscount(double newDiscount);
	void showAllThisType();

};

class Clothes : public Product
{
private:
	static double discount;	//��ľ�̬���� ֻ��һ�� ������ʵ���ɼ�

public:
	double getPrice(string productId);
	void setDiscount(double newDiscount);
	void showAllThisType();
};

class Books : public Product
{
private:
	static double discount;	//��ľ�̬���� ֻ��һ�� ������ʵ���ɼ�

public:
	double getPrice(string productId);
	void setDiscount(double newDiscount);
	void showAllThisType();
};


