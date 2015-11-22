#include "stdafx.h"
#include "head.h"

void Platform::openDB()
{
	int result;

	result = sqlite3_open("platData.db", &platDb);
}

void Platform::closeDB()
{
	sqlite3_close(platDb);
}

bool Platform::judgeExistClientId(string logId)
{
	openDB();
	int result;
	char * errmsg = NULL;
	char **dbResult; //是 char ** 类型，两个*号
	int nRow, nColumn;

	string SQLCode = "select * from client where logId = \"" + logId + "\";";
	result = sqlite3_get_table(platDb, SQLCode.c_str(), &dbResult, &nRow, &nColumn, &errmsg);

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
	result = sqlite3_get_table(platDb, SQLCode.c_str(), &dbResult, &nRow, &nColumn, &errmsg);

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

	result = sqlite3_exec(platDb, SQLCode.c_str(), 0, 0, &errmsg);

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
		char tmpBuf[BUF_LEN];
		cout << "bankName:";
		string bankName;
		cin >> tmpBuf;
		bankName = tmpBuf;

		cout << "cardNum:";
		string cardNum;
		cin >> tmpBuf;
		cardNum = tmpBuf;

		cout << "bank card password:";
		string bankCardPassword;
		cin >> tmpBuf;
		bankCardPassword = tmpBuf;

		//验证银行密码
		//格式"ICBC\1!00000006\1!123\1!10"
		string sendMsg1 = bankName + "\1!" + cardNum + "\1!" + bankCardPassword + "\1!0\0";
		char sendMsg[MSG_MAX_LEN];
		strcpy(sendMsg, sendMsg1.c_str());
		string res = this->ec_connect(this->argc, this->argv, sendMsg);
		if (res != "YES")
		{
			cout << "银行卡信息验证失败" << endl;
			return;
		}

		createCardRecord(logId, bankName, cardNum);
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
	result = sqlite3_exec(platDb, SQLCode.c_str(), 0, 0, &errmsg);

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
	delete product;
}

void Platform::showClothes()
{
	Product *product = new Clothes;	//应该“委托”(delegation)给product去做		
	product->showAllThisType();		//本质上是一种组合（虽然不是其成员） OO原则：少用继承，多用组合
	delete product;
}

void Platform::showBooks()
{
	Product *product = new Books;	//应该“委托”(delegation)给product去做		
	product->showAllThisType();		//本质上是一种组合（虽然不是其成员） OO原则：少用继承，多用组合
	delete product;
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
	result = sqlite3_get_table(platDb, SQLCode.c_str(), &dbResult, &nRow, &nColumn, &errmsg);

	sqlite3_close(db);

	if (nRow > 0)
		return (string)dbResult[8];
	else
		return (string)"none";

}

bool Platform::addToCart()
{
	//先登录才能添加到购物车
	if (logFlag == false)
	{
		cout << "Please log in" << endl;
		return false;
	}

	cout << "productId:";
	char tmpBuf[BUF_LEN];
	cin >> tmpBuf;
	string productId = tmpBuf;

	string type = getProductType(productId);
	if (type == "food")
	{
		//Product *product = new Food;
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
	cout << "num:";
	int num;
	cin >> num;

	CartProduct tmp = { productId, num };
	cart.push_back(tmp);
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

	Product *product = NULL;
	if (type == "food")
	{
		product = new Food;
	}
	else if (type == "clothes")
	{
		product = new Clothes;
	}
	else if (type == "books")
	{
		product = new Books;
	}
	else
	{
		cout << "no such productId" << endl;
		return false;
	}


	//TODO:第三部分
	//付款
	//if付款成功再执行下面的
	//具体的可能要交给product类去执行
	updateDiscount();
	updateBonus();

	double cost = 0;
	cost += product->calcCost(product, productId, num);
	if (cost == -1)
		return false;

	

	cost = bonusReduction(cost);	//满减优惠
	cout << "money:" << cost << endl;

	if (boundCardPay(curClient.logId, cost) == false)
	{
		cout << "银行卡支付失败" << endl;
		return false;
	}

	cost = 0;
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
	//
	if (checkLeft() == false)
	{
		cout << "购物车清空" << endl;
		cart.clear();
		return false;
	}

	//计算钱数
	updateDiscount();
	updateBonus();
	double totalMoney = 0; 
	
	int cnt = cart.size();
	Product *p;
	for (int i = 0; i < cnt; i++)
	{
		p = createProduct(getProductType(cart[i].productId));
		totalMoney = totalMoney + p->getPrice(cart[i].productId) * cart[i].num;
		delete p;
	}

	totalMoney = bonusReduction(totalMoney);	//满减优惠
	cout << "total money:" << totalMoney << endl;
	
	char tmpCost[100];
	sprintf(tmpCost, "%f", totalMoney);
	string cost = tmpCost;

	//付款
	//若失败 返回
	if (boundCardPay(curClient.logId, totalMoney) == false)
	{
		cout << "银行卡支付失败" << endl;
		return false;
	}

	for (int i = 0; i < cnt; i++)
	{
		p = createProduct(getProductType(cart[i].productId));
		p->buy(p, cart[i].productId, cart[i].num);
		delete p;
	}
	
	cout << "购物车清空" << endl;
	cart.clear();
	return true;
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

bool Platform::checkLeft()
{
	bool Flag = true;

	int cnt = cart.size();
	for (int i = 0; i < cnt; i++)
	{
		string type = getProductType(cart[i].productId);
		Product *p = createProduct(type);
		int curLeft = p->getLeft(cart[i].productId);
		if (curLeft < cart[i].num)
		{
			Flag = false;
			cout << "no enough product:" << cart[i].productId.c_str() << endl;
		}
	}

	return Flag;
}

Product* Platform::createProduct(string type)
{
	Product *p = NULL;
	if (type == "food")
	{
		p = new Food;
	}
	else if (type == "clothes")
	{
		p = new Clothes;
	}
	else if (type == "books")
	{
		p = new Books;
	}
	return p;
}

bool Platform::updateDiscount()
{
	openDB();
	int result;
	char * errmsg = NULL;
	char **dbResult; //是 char ** 类型，两个*号
	int nRow, nColumn;

	Product *p = NULL;

	//更新FOOD的打折
	string SQLCode = "select * from setting where item = \"foodDiscount\";";
	result = sqlite3_get_table(platDb, SQLCode.c_str(), &dbResult, &nRow, &nColumn, &errmsg);
	p = new Food;
	p->setDiscount(atof(dbResult[3]));
	delete p;
	
	//更新CLOTHES的打折
	SQLCode = "select * from setting where item = \"clothesDiscount\";";
	result = sqlite3_get_table(platDb, SQLCode.c_str(), &dbResult, &nRow, &nColumn, &errmsg);
	p = new Clothes;
	p->setDiscount(atof(dbResult[3]));
	delete p;

	//更新BOOKS的打折
	SQLCode = "select * from setting where item = \"booksDiscount\";";
	result = sqlite3_get_table(platDb, SQLCode.c_str(), &dbResult, &nRow, &nColumn, &errmsg);
	p = new Books;
	p->setDiscount(atof(dbResult[3]));
	delete p;
	
	closeDB();

	return true;
}

double Platform::bonusReduction(double curMoney)
{
	int bonusCnt = bonus.size();
	for (int i = bonusCnt-1; i >= 0; i--)
	{
		if (curMoney > bonus[i].level)
		{
			cout << "原价：" << curMoney << endl;
			cout << "满" << bonus[i].level << "减" << bonus[i].sub << endl;
			return curMoney - bonus[i].sub;
		}
	}
	return curMoney;
}

bool Platform::updateBonus()
{
	bonus.clear();

	openDB();
	int result;
	char * errmsg = NULL;
	char **dbResult; //是 char ** 类型，两个*号
	int nRow, nColumn;
	
	string SQLCode = "select * from setting where item = \"bonusCnt\";";
	result = sqlite3_get_table(platDb, SQLCode.c_str(), &dbResult, &nRow, &nColumn, &errmsg);
	int bonusCnt = atoi(dbResult[3]);

	SQLCode = "select * from bonus;";
	result = sqlite3_get_table(platDb, SQLCode.c_str(), &dbResult, &nRow, &nColumn, &errmsg);
	
	closeDB();

	for (int i = 1; i <= bonusCnt; i++)
	{
		if (dbResult[2 * i + 1] == NULL)	//容错处理
			return false;

		Bonus newBonus;
		newBonus.level = atof(dbResult[2 * i + 0]);
		newBonus.sub = atof(dbResult[2 * i + 1]);
		bonus.push_back(newBonus);
	}
	
	//sort(bonus.begin(), bonus.end());
	return true;
}

string Platform::ec_connect(int argc, char **argv, char* sendbuf)
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	//char *sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;

	// Validate the parameters
	if (argc != 2) {
		printf("usage: %s server-name\n", argv[0]);
		return (string)"1";
	}

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return (string)"1";
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return (string)"1";
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return (string)"1";
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return (string)"1";
	}

	// Send an initial buffer
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return (string)"1";
	}

	printf("Bytes Sent: %ld\n", iResult);

	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return (string)"1";
	}

	string recvMsg;//用来存放结果

	// Receive until the peer closes the connection
	do {

		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			printf("Bytes received: %d\n", iResult);
			recvbuf[iResult] = '\0';//强行添加结尾
			recvMsg = recvbuf;
		}
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

	} while (iResult > 0);

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();

	
	
	return recvMsg;
}

bool Platform::createCardRecord(string logId, string bankName, string cardNum)
{
	openDB();	//打开数据库 另指针得到一个值

	int result;
	char * errmsg = NULL;

	string SQLCode = "insert into boundCard ";
	SQLCode = SQLCode + "(logId, bankName, cardNum)";
	SQLCode = SQLCode + "values(\"" + logId + "\", \"" + bankName + "\", \"" + cardNum + "\");";

	result = sqlite3_exec(platDb, SQLCode.c_str(), 0, 0, &errmsg);

	closeDB();	//释放指针

	if (result == SQLITE_OK)
		return true;
	else
	{
		cout << errmsg;
		return false;
	}
}

int Platform::getArgc()
{
	return this->argc;
}

char** Platform::getArgv()
{
	return this->argv;
}

bool Platform::boundCardPay(string logId, double cost)
{
	openDB();
	int result;
	char * errmsg = NULL;
	char **dbResult; //是 char ** 类型，两个*号
	int nRow, nColumn;

	string SQLCode = "select * from boundCard where logId = \"" + logId + "\";";
	result = sqlite3_get_table(platDb, SQLCode.c_str(), &dbResult, &nRow, &nColumn, &errmsg);

	closeDB();

	if (nRow == 0)
	{
		cout << "no bound card, please bind it first." << endl;
		return false;
	}

	cout << "this logId have bound " << nRow << " card(s)." << endl;
	cout << "Please choose one to pay the bill." << endl;
	cout << "----------------" << endl;
	for (int i = 1; i <= nRow; i++)
	{
		cout << "input " << i << " to choose this one";
		cout << "bank name:" << dbResult[2 * i + 2] << endl;
		cout << "card number:" << dbResult[2 * i + 3] << endl;
		cout << "----------------" << endl;
	}

	int n;
	cin >> n;
	if (n > nRow)
	{
		cout << "no such card" << endl;
		return false;
	}

	char tmpBuf[BUF_LEN];
	cout << "bank card password:";
	string bankCardPassword;
	cin >> tmpBuf;
	bankCardPassword = tmpBuf;

	string bankName = dbResult[2 * n + 2];
	string cardNum = dbResult[2 * n + 3];
	char tmpCost[100];
	sprintf(tmpCost, "%f", cost);
	string _cost = tmpCost;
	string sendMsg1 = bankName + "\1!" + cardNum + "\1!" + bankCardPassword + "\1!" + _cost + "\0";
	char sendMsg[MSG_MAX_LEN];
	strcpy(sendMsg, sendMsg1.c_str());
	string res = this->ec_connect(this->argc, this->argv, sendMsg);
	if (res != "YES")
	{
		//cout << "银行卡信息验证失败" << endl;
		cout << res.c_str() << endl;
		return false;
	}

	cout << "银行扣款成功" << endl;
	return true;
}