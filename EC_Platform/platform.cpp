#include "stdafx.h"
#include "head.h"

void Platform::openDB()
{
	int result;

	result = sqlite3_open("platData.db", &clientDb);
}

void Platform::closeDB()
{
	sqlite3_close(clientDb);
}

bool Platform::judgeExistClientId(string logId)
{
	openDB();
	int result;
	char * errmsg = NULL;
	char **dbResult; //是 char ** 类型，两个*号
	int nRow, nColumn;

	string SQLCode = "select * from client where logId = \"" + logId + "\";";
	result = sqlite3_get_table(clientDb, SQLCode.c_str(), &dbResult, &nRow, &nColumn, &errmsg);

	closeDB();
	if (nRow > 0)
		return true;
	else
		return false;
}

string Platform::getPassword(string logId)
{
	openDB();	//打开数据库 另指针得到一个值

	int result;
	char * errmsg = NULL;
	char **dbResult; //是 char ** 类型，两个*号
	int nRow, nColumn;

	string SQLCode = "select * from client where logId = \"" + logId + "\";";
	result = sqlite3_get_table(clientDb, SQLCode.c_str(), &dbResult, &nRow, &nColumn, &errmsg);

	closeDB();	//释放指针

	if (nRow > 0)
		return (string)dbResult[3];
	else
		return (string)"";
}

bool Platform::addClient()
{

	string logId;
	while (true)
	{	
		char tmpBuf[BUF_LEN];
		cout << "input the logId you want:";
		cin >> tmpBuf;
		logId = tmpBuf;
		if (logId.length() > 0 && this->judgeExistClientId(logId) == false)
			break;
	}


	//TODO:输入两次密码的验证
	string password;
	char tmpBuf[BUF_LEN];
	cout << "password:";
	cin >> tmpBuf;
	password = tmpBuf;

	if (createRecord(logId, password))
		return true;
	else
		return false;

}

bool Platform::createRecord(string logId, string password)
{
	openDB();	//打开数据库 另指针得到一个值

	int result;
	char * errmsg = NULL;

	string SQLCode = "insert into client ";
	SQLCode = SQLCode + "(logId, password)";
	SQLCode = SQLCode + "values(\"" + logId + "\", \"" + password +  + "\");";

	result = sqlite3_exec(clientDb, SQLCode.c_str(), 0, 0, &errmsg);

	closeDB();	//释放指针

	if (result == SQLITE_OK)
		return true;
	else
	{
		cout << errmsg;
		return false;
	}
}

void Platform::clientInfoUpdate()
{
	char tmpBuf[BUF_LEN];
	cout << "logId:";
	string logId;
	cin >> tmpBuf;
	logId = tmpBuf;

	cout << "password:";
	string inputPassword;
	cin >> tmpBuf;
	inputPassword = tmpBuf;
	if (!verifyPassword(logId, inputPassword))	//若密码错误，直接返回
	{
		return;
	}

	int n;
	cout << "1.bind bank card 2.change password 0.nothing" << endl;
	cin >> n;
	if (n == 1)
	{
		//第三部分
	}
	else
	{
		char tmpBuf[BUF_LEN];
		string newPassword;
		cout << "new password:" << endl;
		cin >> tmpBuf;
		newPassword = tmpBuf;
		setPassword(logId, newPassword);
	}
}

bool Platform::setPassword(string logId, string newPassword)
{
	int result;
	char * errmsg = NULL;

	string SQLCode = "update client set password = " + newPassword + " where logId = \"" + logId + "\"";
	result = sqlite3_exec(clientDb, SQLCode.c_str(), 0, 0, &errmsg);

	if (result == SQLITE_OK)
		return true;
	else
		return false;
}

bool Platform::verifyPassword(string logId, string inputPassword)
{
	string password = this->getPassword(logId);

	if (password == inputPassword)
		return true;
	else
		return false;
}

void Platform::showFood()
{
	Product *product = new Food;	//应该“委托”(delegation)给product去做		
	product->showAllThisType();		//本质上是一种组合（虽然不是其成员） OO原则：少用继承，多用组合
}

void Platform::showClothes()
{

}

void Platform::showBooks()
{

}

string Platform::getProductType(string productId)
{
	sqlite3 *db;

	int result;
	char * errmsg = NULL;
	char **dbResult; //是 char ** 类型，两个*号
	int nRow, nColumn;

	result = sqlite3_open("productData.db", &db);

	string SQLCode = "select * from product where productId = \"" + productId + "\";";
	result = sqlite3_get_table(clientDb, SQLCode.c_str(), &dbResult, &nRow, &nColumn, &errmsg);

	sqlite3_close(db);

	if (nRow > 0)
		return (string)dbResult[8];
	else
		return (string)"none";

}

bool Platform::addToCart()
{
	cout << "productId:";
	char tmpBuf[BUF_LEN];
	cin >> tmpBuf;
	string productId = tmpBuf;

	string type = getProductType(productId);
	if (type == "food")
	{
		Product *product = new Food;
	}
	else if (type == "clothes")
	{
		//Product *product = new Clothes;
	}
	else if (type == "books")
	{
		//Product *product = new Books;
	}
	else
	{
		cout << "no such productId" << endl;
		return false;
	}
	
	cart.push(productId);
	return true;
	
}

bool Platform::payForProduct()
{
	//先登录才能结账
	if (logFlag == false)
	{
		cout << "Please log in" << endl;
		return false;
	}

	cout << "productId:";
	char tmpBuf[BUF_LEN];
	cin >> tmpBuf;
	string productId = tmpBuf;

	cout << "num:";
	int num;
	cin >> num;

	string type = getProductType(productId);

	Product *product;
	if (type == "food")
	{
		product = new Food;
	}
	/*else if (type == "clothes")
	{
		Product *product = new Clothes;
	}
	else if (type == "books")
	{
		Product *product = new Books;
	}*/
	else
	{
		cout << "no such productId" << endl;
		return false;
	}

	//TODO:第三部分
	//付款
	//if付款成功再执行下面的
	//具体的可能要交给product类去执行

	double cost = 0;
	cost += product->buy(product, productId, num);
	if (cost == -1)
		return false;

	cout << "buy successfully(3rd part)" << endl;	
	return true;
}

bool Platform::payForCart()
{
	//先登陆才能结账
	if (logFlag == false)
	{
		cout << "Please log in" << endl;
		return false;
	}

	//TODO:待完成

}

bool Platform::logInOrOut()
{
	if (logFlag == false)	//未登录状态
	{
		cout << "logId:";
		char tmpBuf[BUF_LEN];
		cin >> tmpBuf;
		string logId = tmpBuf;

		if (judgeExistClientId(logId) == false)
		{
			cout << "no such id" << endl;
			return false;
		}

		cout << "password:";
		cin >> tmpBuf;
		string password = tmpBuf;

		if (verifyPassword(logId, password) == false)
		{
			cout << "incorrect password" << endl;
			return false;
		}

		curClient.logId = logId;
		logFlag = true;
		return true;
	}


	else if(logFlag = true)	//登陆状态，则退出登陆
	{
		logFlag = false;
		cout << "log out successfully" << endl;
		return true;
	}
}