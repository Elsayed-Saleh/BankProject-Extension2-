#include <iostream>
#include <windows.h>
#include <ctime>
#include<string>
#include<iomanip>
#include <vector>
#include<fstream>
using namespace std;

void ShowManagesMenue();

const string FileName = "ClintsDetails.txt";
const string UserFile = "Users.txt";

void ShowMainMenue();

enum enMainMenuePermissions
{
	eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4,
	pUpdateClients = 8, pFindClient = 16, pTranactions = 32, pManageUsers = 64
};

struct sUser
{
	string UserName = "";
	string Password = "";
	short Permissions = 0;
	bool MarkForDelete = false;
};

struct sClient
{
	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	double AccounBalance = 0.0;
	bool MarkFoeDelete = false;
};

vector<string>SplitString(string Line, string Delim)
{
	vector<string>vString;
	short Pos;
	string sWord;
	while ((Pos = Line.find(Delim)) != std::string::npos)
	{
		sWord = Line.substr(0, Pos);
		if (sWord != "")
		{
			vString.push_back(sWord);
		}
		Line.erase(0, Pos + Delim.length());
	}
	if (Line != "")
	{
		vString.push_back(Line);
	}
	return vString;
}

sUser CurrentUser;

void ShowAccessDeniedMessage()
{
	cout << "\n------------------------------------\n";
	cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin.";
	cout << "\n------------------------------------\n";
}

bool CheckAccessPermission(enMainMenuePermissions Permission)
{
	if (CurrentUser.Permissions == enMainMenuePermissions::eAll)
		return true;

	if ((Permission & CurrentUser.Permissions) == Permission)
		return true;
	else
		return false;

}


sClient ConvertLineToRecordClient(string Line, string Seperator = "#//#")
{
	sClient Client;
	vector<string>vString = SplitString(Line, Seperator);
	Client.AccountNumber = vString[0];
	Client.PinCode = vString[1];
	Client.Name = vString[2];
	Client.Phone = vString[3];
	Client.AccounBalance = stod(vString[4]);
	return Client;
}

sUser ConvertLineToRecordUser(string Line, string Seperator = "#//#")
{
	sUser Users;
	vector<string>vString = SplitString(Line, Seperator);
	Users.UserName = vString[0];
	Users.Password = vString[1];
	Users.Permissions = stoi(vString[2]);
	return Users;
}

string ConvertRecordToLineClients(sClient Client, string Seperator = "#//#")
{
	string Line = "";
	Line += Client.AccountNumber + Seperator;
	Line += Client.PinCode + Seperator;
	Line += Client.Name + Seperator;
	Line += Client.Phone + Seperator;
	Line += to_string(Client.AccounBalance);

	return Line;
}

string ConvertRecordToLineUsers(sUser Users, string Seperator = "#//#")
{
	string Line = "";
	Line += Users.UserName + Seperator;
	Line += Users.Password + Seperator;
	Line += to_string(Users.Permissions);

	return Line;
}

bool ClientExistByAccountNumber(string AccountNumber, string FileName)
{
	vector<sClient>vClient;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		sClient Client;
		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecordClient(Line);
			if (Client.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
		}
		MyFile.close();

	}
	return false;

}


sClient ReadNewClient()
{
	sClient Client;
	cout << "Enter Account Number : ";
	getline(cin >> ws, Client.AccountNumber);
	while (ClientExistByAccountNumber(Client.AccountNumber, FileName))
	{
		cout << "\nClient with [ " << Client.AccountNumber << " ] already exists, Enter another Account Number? ";
		getline(cin >> ws, Client.AccountNumber);
	}

	cout << "\nEnter PinCode : ";
	getline(cin, Client.PinCode);
	cout << "\nEnter your name : ";
	getline(cin, Client.Name);
	cout << "\nEnter Phone number : ";
	getline(cin, Client.Phone);
	cout << "\nEnter Account balance : ";
	cin >> Client.AccounBalance;

	return Client;
}


vector<sClient>LoadDataFromFileClients(string FileName)
{
	vector<sClient>vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		sClient Client;
		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecordClient(Line);
			vClients.push_back(Client);
		}
		MyFile.close();
	}

	return vClients;
}

vector<sUser>LoadDataFromFileUsers(string UserFile)
{
	vector<sUser>vUsers;
	fstream MyFile;
	MyFile.open(UserFile, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		sUser User;
		while (getline(MyFile, Line))
		{
			User = ConvertLineToRecordUser(Line);
			vUsers.push_back(User);
		}
		MyFile.close();
	}

	return vUsers;
}

void PrintUserRecordLine(sUser User)
{
	cout << "| " << setw(20) << left << User.UserName;
	cout << "| " << setw(20) << left << User.Password;
	cout << "| " << setw(30) << left << User.Permissions << endl;
}

void PrintClientRecordLine(sClient Client)
{
	cout << "| " << setw(20) << left << Client.AccountNumber;
	cout << "| " << setw(20) << left << Client.PinCode;
	cout << "| " << setw(30) << left << Client.Name;
	cout << "| " << setw(20) << left << Client.Phone;
	cout << "| " << setw(20) << left << Client.AccounBalance << endl;
}

void ShowAllClientsScreen()
{

	if (!CheckAccessPermission(enMainMenuePermissions::pListClients))
	{
		ShowAccessDeniedMessage();
		return;
	}

	vector<sClient>vClients = LoadDataFromFileClients(FileName);

	cout << "\n\t\t\t\tClients list ( " << vClients.size() << " ) Client(s) \n";

	cout << "\n------------------------------------------------------------------";
	cout << "----------------------------------------------------\n\n" << endl;

	cout << "| " << left << setw(20) << "Account Number";
	cout << "| " << left << setw(20) << "Pin Code";
	cout << "| " << left << setw(30) << "Name";
	cout << "| " << left << setw(20) << "Phone Number";
	cout << "| " << left << setw(20) << "Account balance";

	cout << "\n------------------------------------------------------------------";
	cout << "----------------------------------------------------\n\n" << endl;
	if (vClients.size() == 0)
	{
		cout << "\t\t\tNo Clients Available in the system!";
	}
	else
	{
		for (sClient& C : vClients)
		{
			PrintClientRecordLine(C);
		}
		cout << endl;
	}

	cout << "\n------------------------------------------------------------------";
	cout << "----------------------------------------------------\n\n" << endl;

}


void ShowAllUsersScreen()
{
	vector<sUser>vUsers = LoadDataFromFileUsers(UserFile);

	cout << "\n\t\t\t\t Users list ( " << vUsers.size() << " ) User(s) \n";

	cout << "\n__________________________________________________________________";
	cout << "____________________________________________________\n\n" << endl;

	cout << "| " << left << setw(20) << "Full name";
	cout << "| " << left << setw(20) << "Password";
	cout << "| " << left << setw(30) << "Permission";

	cout << "\n__________________________________________________________________";
	cout << "____________________________________________________\n\n" << endl;
	if (vUsers.size() == 0)
	{
		cout << "\t\t\tNo Users Available in the system!";
	}
	else
	{
		for (sUser& U : vUsers)
		{
			PrintUserRecordLine(U);
		}
		cout << endl;
	}

	cout << "\n__________________________________________________________________";
	cout << "____________________________________________________\n\n" << endl;

}


void PrintClientCard(sClient C)
{
	cout << "The following are client details : " << endl;
	cout << "_______________________________________________" << endl;
	cout << "Account number : " << C.AccountNumber << endl;
	cout << "Pin Code: " << C.PinCode << endl;
	cout << "Name: " << C.Name << endl;
	cout << "Phone number: " << C.Phone << endl;
	cout << "Account Blalnce: " << C.AccounBalance << endl;
	cout << "_______________________________________________" << endl;

}

bool FindClientByAccountNumber(vector<sClient>& Clients, string AccountNumber, sClient& Client)
{
	for (sClient& C : Clients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}


sClient ChangeClientRecoed(string AccountNumber)
{
	sClient Client;

	Client.AccountNumber = AccountNumber;

	cout << "\nEnter Pin code : ";
	getline(cin >> ws, Client.PinCode);
	cout << "\nEnter Name : ";
	getline(cin, Client.Name);
	cout << "Enter Phone number : ";
	getline(cin, Client.Phone);
	cout << "Enter Account balance : ";
	cin >> Client.AccounBalance;

	return Client;
}

bool MarkAccountNumberToDelet(vector<sClient>& Clients, string AccountNumber)
{

	for (sClient& C : Clients)
	{

		if (C.AccountNumber == AccountNumber)
		{
			C.MarkFoeDelete = true;
			return true;
		}
	}
	return false;
}

vector<sClient> SaveClientDataToFile(vector<sClient>Clients, string FileName)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	string Line;

	if (MyFile.is_open())
	{
		for (sClient& C : Clients)
		{
			if (C.MarkFoeDelete == false)
			{
				Line = ConvertRecordToLineClients(C);
				MyFile << Line << endl;

			}
		}
		MyFile.close();
	}
	return Clients;
}

vector<sUser> SaveUserDataToFile(vector<sUser>Users, string FileName)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	string Line;

	if (MyFile.is_open())
	{
		for (sUser& S : Users)
		{
			if (S.MarkForDelete == false)
			{
				Line = ConvertRecordToLineUsers(S);
				MyFile << Line << endl;

			}
		}
		MyFile.close();
	}
	return Users;
}

void AddDataLineToFile(string FileName, string DataLine)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{

		MyFile << DataLine << endl;

		MyFile.close();

	}

}

void AddNewClient()
{
	sClient Client;
	Client = ReadNewClient();
	AddDataLineToFile(FileName, ConvertRecordToLineClients(Client));

}

void AddNewClients()
{
	char AddMore = 'Y';
	do
	{

		cout << "Adding new Client: \n\n";
		AddNewClient();
		cout << "\nClient Addedsuccessfuly, do you want do Add more clients? Y/N? ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');

}


bool DeleteclientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
	sClient Client;
	char Answer = 'n';
	if (FindClientByAccountNumber(vClients, AccountNumber, Client))
	{

		PrintClientCard(Client);

		cout << "Are you sure you want delete this account ? y/n? ";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y')
		{
			MarkAccountNumberToDelet(vClients, AccountNumber);
			SaveClientDataToFile(vClients, FileName);
			vClients = LoadDataFromFileClients(FileName);

			cout << "\n\nClient Deleted successfuly.";

			return true;

		}

	}
	else
	{

		cout << "\nClient with account number [" << AccountNumber << "] is not found!";

		return false;
	}

}

bool UpdateClientByAccountNumber(vector<sClient>& vClient, string AccountNumber)
{
	sClient Client;

	char Answer = 'n';
	if (FindClientByAccountNumber(vClient, AccountNumber, Client))
	{
		PrintClientCard(Client);

		cout << "Are you sure you want delete this account ? y/n? ";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y')
		{
			for (sClient& C : vClient)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecoed(AccountNumber);
					break;
				}
			}
		}
		SaveClientDataToFile(vClient, FileName);

		cout << "\n\nClient Updated successfuly.";
		return true;

	}
	else
	{
		cout << "\nClient with account number [" << AccountNumber << "] is not found!";

		return false;
	}

}

string ReadClientAccountNumber()
{
	string AccountNumber;
	cout << "\nPlease enter account number? ";
	cin >> AccountNumber;
	return AccountNumber;
}


void ShowDeleteClientScreen()
{

	if (!CheckAccessPermission(enMainMenuePermissions::pDeleteClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

	cout << "\n________________________________________________\n\n";
	cout << "\tDelete Client Screen ";
	cout << "\n________________________________________________\n";

	vector<sClient>vClients = LoadDataFromFileClients(FileName);
	string AccountNumber = ReadClientAccountNumber();
	DeleteclientByAccountNumber(AccountNumber, vClients);

}

void ShowUpdateClientScreen()
{

	if (!CheckAccessPermission(enMainMenuePermissions::pUpdateClients))
	{
		ShowAccessDeniedMessage();
		return;
	}

	cout << "\n________________________________________________\n";
	cout << "\tUpdate Client Screen ";
	cout << "\n________________________________________________\n";

	vector<sClient>vClients = LoadDataFromFileClients(FileName);
	string AccountNumber = ReadClientAccountNumber();
	UpdateClientByAccountNumber(vClients, AccountNumber);
}

void ShowAddNewClientScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pAddNewClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

	cout << "\n________________________________________________\n\n";
	cout << "\tAdd New Client Screen ";
	cout << "\n________________________________________________\n";

	AddNewClients();
}

bool FindUserByUsernameAndPassword(string Username, string Password, sUser& User)
{

	vector <sUser> vUsers = LoadDataFromFileUsers(UserFile);

	for (sUser U : vUsers)
	{

		if (U.UserName == Username && U.Password == Password)
		{
			User = U;
			return true;
		}

	}
	return false;

}

void ShowFindClientScreen()
{

	if (!CheckAccessPermission(enMainMenuePermissions::pFindClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

	cout << "\n________________________________________________\n\n";
	cout << "\tFind Client Screen ";
	cout << "\n________________________________________________\n";

	vector<sClient>vClients = LoadDataFromFileClients(FileName);
	string AccountNumber = ReadClientAccountNumber();
	sClient Client;

	if (FindClientByAccountNumber(vClients, AccountNumber, Client))
	{
		PrintClientCard(Client);
	}
	else
	{
		cout << "\nClient with account number [" << AccountNumber << "] is not found!";
	}
}

void ShowEndScreen()
{
	cout << "\n________________________________________________\n\n";
	cout << "\tProgram Ends :-) ";
	cout << "\n________________________________________________\n";
}

int ReadWithDrawAccount(sClient Client)
{
	int Amount;
	cout << "\nPlease enter Deposit Amount? ";
	cin >> Amount;


	while (Amount > Client.AccounBalance)
	{
		cout << "Amount Exceeds the balance, you can Withdraw up to : " << Client.AccounBalance << endl;
		cout << "Please enter anther amount? ";
		cin >> Amount;
	}

	return Amount;
}

int ReadAmountAccount()
{
	int Amount;
	cout << "\nPlease enter Deposit Amount? ";
	cin >> Amount;
	return Amount;
}

void ChaingAccountBlalnce(int Amount, sClient& C)
{
	C.AccounBalance += Amount;
}

bool ChingDepositClientRecord(vector<sClient>& Clients, string AccountNumber, int DepositAmount)
{
	for (sClient& C : Clients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			ChaingAccountBlalnce(DepositAmount, C);

			cout << "Done Successfuly New balance is [" << C.AccounBalance << "]..." << endl;
			return true;

		}
	}
	return false;
}



void DepositClientByAccountNumber(vector<sClient>& Clients, string AccountNumber)
{
	sClient Client;
	char DoAgain = 'y';
	char Answer = 'n';

	int DepositAmount;
	do
	{

		if (FindClientByAccountNumber(Clients, AccountNumber, Client))
		{
			PrintClientCard(Client);

			DepositAmount = ReadAmountAccount();

			cout << "\n\nAre you sure you want perfrom this transaction? y/n? ";
			cin >> Answer;
			if (toupper(Answer) == 'Y')
			{
				ChingDepositClientRecord(Clients, AccountNumber, DepositAmount);
				Clients = SaveClientDataToFile(Clients, FileName);

			}

			cout << "Prees Any Key To Exit... ";
			system("pause>0");
			DoAgain = 'n';

		}
		else
		{
			cout << "Client with account number [" << AccountNumber << "] dose not Exist.";
			DoAgain = 'y';
		}
	} while (DoAgain == 'y');
}


void PrintClientBalanceRecord(sClient Client)
{

	cout << "| " << setw(30) << left << Client.AccountNumber;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(40) << left << Client.AccounBalance << endl;

}

void ShowTotalBalanceScreen()
{
	vector<sClient>vClients = LoadDataFromFileClients(FileName);

	double TotalAccountBalance = 0;

	cout << "\n\t\t\t\tBalance list ( " << vClients.size() << " ) Client(s) \n";

	cout << "\n__________________________________________________________________";
	cout << "________________________________________________________\n\n" << endl;

	cout << "| " << left << setw(30) << "Account Number";
	cout << "| " << left << setw(40) << "Name";
	cout << "| " << left << setw(40) << "Account balance";

	cout << "\n__________________________________________________________________";
	cout << "________________________________________________________\n\n" << endl;

	if (vClients.size() == 0)
	{
		cout << "\t\t\tNo Clients Available in the system!";
	}
	else
	{
		for (sClient& C : vClients)
		{
			PrintClientBalanceRecord(C);
			TotalAccountBalance += C.AccounBalance;
		}
	}
	cout << "\n__________________________________________________________________";
	cout << "________________________________________________________\n\n" << endl;
	cout << "\t\t\t\t\t\t\t    Total Balance = " << TotalAccountBalance << endl;

	cout << "Prees any key to go back to Transaction Menue...";
	system("pause>0");

}

void WithdrawClientByAccountNumber(vector<sClient>Clients, string AccountNumber)
{

	sClient Client;
	char DoAgin = 'n';
	char Answer = 'n';
	int Withdraw;
	do
	{
		if (FindClientByAccountNumber(Clients, AccountNumber, Client))
		{
			PrintClientCard(Client);
			Withdraw = ReadWithDrawAccount(Client);

			cout << "\n\nAre you sure you want perfrom this transaction? y/n? ";
			cin >> Answer;
			if (toupper(Answer) == 'Y')
			{
				ChingDepositClientRecord(Clients, AccountNumber, Withdraw * -1);
				Clients = SaveClientDataToFile(Clients, FileName);

			}
			cout << "Prees any key to exit ..." << endl;
			system("pause>0");
			DoAgin = 'n';
		}
		else
		{
			cout << "Client with account number [" << AccountNumber << "] dose not Exist.";
			AccountNumber = ReadClientAccountNumber();
			DoAgin = 'Y';
		}
	} while (DoAgin == 'Y');


}


void ShowDepositClientScreen()
{

	cout << "\n________________________________________________\n\n";
	cout << "\tDeposit Client Screen ";
	cout << "\n________________________________________________\n";

	vector<sClient>vClients = LoadDataFromFileClients(FileName);
	string AccountNumber = ReadClientAccountNumber();

	DepositClientByAccountNumber(vClients, AccountNumber);
}


void ShowWithdrawClientScreen()
{

	cout << "\n________________________________________________\n\n";
	cout << "\tWithdraw Client Screen ";
	cout << "\n________________________________________________\n";

	vector<sClient>vClients = LoadDataFromFileClients(FileName);
	string AccountNumber = ReadClientAccountNumber();

	WithdrawClientByAccountNumber(vClients, AccountNumber);
}

enum enMainMenueOption
{
	eListClient = 1, eAddNewClient = 2,
	eDlelteClient = 3, eUpdateClient = 4,
	eFindClient = 5, eTransactionMenue = 6,
	eManagesUsers = 7, eExit = 8
};

enum enTransactionMenueOption
{
	eDeposit = 1, eWithdraw = 2, eTotalBalance = 3, enExit = 4
};

void ShowTransactionMenue();

void GoBackToMainMenue()
{
	cout << "\n\nPress any Key to go back Main Menue...";
	system("pause>0");
	ShowMainMenue();

}

void GoBackToTransactionMenue()
{
	cout << "\n\nPress any Key to go back Transaction Menue...";
	system("pause>0");
	ShowTransactionMenue();
}

short ReadMainMenueOpration()
{
	cout << "Choose do you want to do? [1 to 6]? ";
	short Choice = 0;
	cin >> Choice;
	return Choice;

}

short ReadPermission()
{
	short Permetion = 0;
	char Answer;
	cout << "\nDo you want to give full access ? y / n ? ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
		return -1;


	cout << "\nDo you want to Give access to : " << endl;
	/////////////////
	cout << "\nShow Client List ? y/n? ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
		Permetion += 1;
	/////////////////
	cout << "\nAdd new Client? y/n? ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
		Permetion += 2;
	/////////////////
	cout << "\nDelete Client? y/n? ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
		Permetion += 4;
	/////////////////
	cout << "\nUpdate Client? y/n? ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
		Permetion += 8;
	/////////////////
	cout << "\nFind Client? y/n? ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
		Permetion += 16;
	////////////////
	cout << "\nTransactions? y/n? ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
		Permetion += 32;
	////////////////
	cout << "\nManage Users? y/n? ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
		Permetion += 64;
	return Permetion;

}

bool UserExistByUserName(string UserName, string FileName)
{
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		sUser User;
		while (getline(MyFile, Line))
		{
			User = ConvertLineToRecordUser(Line);
			if (User.UserName == UserName)
			{
				MyFile.close();
				return true;
			}
		}
		MyFile.close();

	}
	return false;
}

sUser ReadNewUser()
{
	sUser User;
	cout << "\nEnter user name: ";
	getline(cin >> ws, User.UserName);
	while (UserExistByUserName(User.UserName, UserFile))
	{
		cout << "\nUser With [" << User.UserName << "] already exists, Enter anther user name? ";
		getline(cin >> ws, User.UserName);
	}
	cout << "\nEnter a password : ";
	cin >> User.Password;
	User.Permissions = ReadPermission();
	return User;
}

void AddUser()
{
	sUser User;
	User = ReadNewUser();
	AddDataLineToFile(UserFile, ConvertRecordToLineUsers(User));
}

void AddNewUsers()
{
	char Answer = 'Y';
	do
	{
		AddUser();
		cout << "\nUser adding successfully, do you want to add more users? y/n? ";
		cin >> Answer;

	} while (toupper(Answer) == 'Y');
}


void ShowAddNewUsersScreen()
{
	cout << "\n________________________________________________\n\n";
	cout << "\tAdd New Client Screen ";
	cout << "\n________________________________________________\n";

	AddNewUsers();
}

bool FindUserByName(vector<sUser>Users, sUser& User, string UserName)
{
	for (sUser& S : Users)
	{
		if (S.UserName == UserName)
		{
			User = S;
			return true;
		}
	}
	return false;
}

void PrintUserCard(sUser User)
{
	cout << "The following are User details : " << endl;
	cout << "_______________________________________________" << endl;
	cout << "User Name : " << User.UserName << endl;
	cout << "Password : " << User.Password << endl;
	cout << "Permitssion : " << User.Permissions << endl;
	cout << "_______________________________________________" << endl;
}

string ReadUserName()
{
	string UserName = "";
	cout << "Enter User Name To search : ";
	getline(cin >> ws, UserName);
	cout << endl;
	return UserName;
}

void ShowFindUserScreen()
{
	cout << "\n________________________________________________\n\n";
	cout << "\tFind User Screen ";
	cout << "\n________________________________________________\n";
	vector<sUser>Users = LoadDataFromFileUsers(UserFile);
	sUser User;
	string UserName = ReadUserName();
	if (FindUserByName(Users, User, UserName))
	{
		PrintUserCard(User);
	}
	else
	{
		cout << "\nClient with account number [" << UserName << "] is not found!";
	}

}


sUser ChaingUserRecord(string UserName)
{
	sUser User;
	User.UserName = UserName;
	cout << "\nEnter new password : ";
	getline(cin >> ws, User.Password);
	cout << "\n";
	User.Permissions = ReadPermission();

	return User;
}

bool UpdateUserByName(vector<sUser> Users, string UserName)
{
	sUser User;
	char Answer = 'y';
	if (FindUserByName(Users, User, UserName))
	{
		PrintUserCard(User);
		cout << "Are you sure you want Update this account ? y/n? ";
		cin >> Answer;
		if (toupper(Answer) == 'Y')
		{
			for (sUser& S : Users)
			{
				if (S.UserName == UserName)
				{
					S = ChaingUserRecord(UserName);
					break;
				}
			}
			Users = SaveUserDataToFile(Users, UserFile);

		}
		cout << "\n\nUser Updated successfuly.";
		return true;

	}
	else
	{
		cout << "\nUser with User NAme  [" << UserName << "] is not found!";

		return false;
	}

}

void ShowUpdateUserScreen()
{
	vector<sUser>Users;

	cout << "\n________________________________________________\n\n";
	cout << "\nUpdate User Screen ";
	cout << "\n________________________________________________\n";

	Users = LoadDataFromFileUsers(UserFile);
	string UserName = ReadUserName();
	UpdateUserByName(Users, UserName);
}

bool MarkUserToDelete(vector<sUser>& Users, string UserName)
{
	for (sUser& S : Users)
	{
		if (S.UserName == UserName)
		{
			S.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

bool DeleteUserByName(vector<sUser>& Users, string UserName)
{

	if (UserName == "Admin")
	{
		cout << "\n\nYou cannot Delete This User.";
		return false;

	}

	sUser User;
	char Answer = 'y';
	if (FindUserByName(Users, User, UserName))
	{
		PrintUserCard(User);
		cout << "Are you sure you want Delete this account ? y/n? ";
		cin >> Answer;
		if (toupper(Answer) == 'Y')
		{
			MarkUserToDelete(Users, UserName);
			SaveUserDataToFile(Users, UserFile);

			Users = LoadDataFromFileUsers(UserFile);
		}
		cout << "\n\nClient Deleted successfuly.";

		return true;
	}
	else
	{
		cout << "\nUser with User Name  [" << UserName << "] is not found!";

		return false;
	}
}

void ShowDeleteUserScreen()
{
	vector<sUser>Users;

	cout << "\n________________________________________________\n";
	cout << "\n\t\tDelete User Screen ";
	cout << "\n________________________________________________\n";

	Users = LoadDataFromFileUsers(UserFile);
	string UserName = ReadUserName();

	DeleteUserByName(Users, UserName);

}
void Login();

enum enManagesMenueOption
{
	enShowAllUsers = 1, enAddUser = 2, enUpdateUser = 3, enDeleteUser = 4, enFindUser = 5, enMainMenu = 6
};



void GoBackMangesMenue()
{
	cout << "\n\nPress any Key to go back Manages Menue...";
	system("pause>0");
	ShowManagesMenue();
}

void PerformMangesMenueScreen(enManagesMenueOption ManagesMenueOption)
{
	switch (ManagesMenueOption)
	{
	case enManagesMenueOption::enShowAllUsers:
		system("cls");
		ShowAllUsersScreen();
		GoBackMangesMenue();
		break;
	case enManagesMenueOption::enAddUser:
		system("cls");
		ShowAddNewUsersScreen();
		GoBackMangesMenue();
		break;

	case enManagesMenueOption::enUpdateUser:

		system("cls");
		ShowUpdateUserScreen();
		GoBackMangesMenue();
		break;

	case enManagesMenueOption::enDeleteUser:

		system("cls");
		ShowDeleteUserScreen();
		GoBackMangesMenue();
		break;

	case enManagesMenueOption::enFindUser:

		system("cls");
		ShowFindUserScreen();

		GoBackMangesMenue();
		break;

	case enManagesMenueOption::enMainMenu:

		system("cls");
		GoBackToMainMenue();
		break;
	}
}

void PerfromTransactionsMenueScreen(enTransactionMenueOption TransactionMenueOption)
{

	switch (TransactionMenueOption)
	{

	case enTransactionMenueOption::eDeposit:
		system("cls");
		ShowDepositClientScreen();
		GoBackToTransactionMenue();
		break;

	case enTransactionMenueOption::eWithdraw:
		system("cls");
		ShowWithdrawClientScreen();
		GoBackToTransactionMenue();
		break;

	case enTransactionMenueOption::eTotalBalance:
		system("cls");
		ShowTotalBalanceScreen();
		GoBackToTransactionMenue();
		break;

	case enTransactionMenueOption::enExit:
		system("cls");
		cout << "\nExit....\nPrees  any key to go Main Menue ";
		system("pause>0");
		GoBackToMainMenue();
	}

}


void PerfromMainMenueOption(enMainMenueOption MainMenueOption)
{

	switch (MainMenueOption)
	{
	case enMainMenueOption::eListClient:
		system("cls");
		ShowAllClientsScreen();
		GoBackToMainMenue();
		break;

	case enMainMenueOption::eAddNewClient:
		system("cls");
		ShowAddNewClientScreen();
		GoBackToMainMenue();
		break;

	case enMainMenueOption::eDlelteClient:
		system("cls");;
		ShowDeleteClientScreen();
		GoBackToMainMenue();
		break;

	case enMainMenueOption::eUpdateClient:
		system("cls");
		ShowUpdateClientScreen();
		GoBackToMainMenue();
		break;

	case enMainMenueOption::eFindClient:
		system("cls");
		ShowFindClientScreen();
		GoBackToMainMenue();
		break;

	case enMainMenueOption::eTransactionMenue:
		system("cls");
		ShowTransactionMenue();
		break;
	case enMainMenueOption::eManagesUsers:
		system("cls");
		ShowManagesMenue();
		GoBackToMainMenue();
		break;

	case enMainMenueOption::eExit:
		system("cls");
		//ShowEndScreen();
		Login();
		break;

	}

}

short ReadTransactionMenueOpration()
{
	cout << "Choose do you want to do? [1 to 4]? ";
	short Choice = 0;
	cin >> Choice;
	return Choice;

}

short ReadManagesMenueOpration()
{
	cout << "Choose do you want to do? [1 to 6]? ";
	short Choice = 0;
	cin >> Choice;
	return Choice;

}


string ReadPassword()
{
	string Password;
	cout << "\nEnter a Password : ";
	getline(cin >> ws, Password);

	return Password;
}

bool IsPassword(sUser User, string Password)
{
	return (User.Password == Password);
}

void ShowManagesMenue()
{

	if (!CheckAccessPermission(enMainMenuePermissions::pManageUsers))
	{
		ShowAccessDeniedMessage();
		return;
	}

	system("cls");
	cout << "====================================================\n";
	cout << "\t\tManages Menue Screen \n";
	cout << "====================================================\n";
	cout << "\t[1] Show all users." << endl;
	cout << "\t[2] Add Users." << endl;
	cout << "\t[3] Update Useers." << endl;
	cout << "\t[4] Dalete Users." << endl;
	cout << "\t[5] Find Users." << endl;
	cout << "\t[6] Main Menue" << endl;
	cout << "====================================================\n";

	PerformMangesMenueScreen((enManagesMenueOption)ReadManagesMenueOpration());
}

void ShowTransactionMenue()
{

	if (!CheckAccessPermission(enMainMenuePermissions::pTranactions))
	{
		ShowAccessDeniedMessage();
		return;
	}

	system("cls");
	cout << "====================================================\n";
	cout << "\t\tTransation Menue Screen \n";
	cout << "====================================================\n";
	cout << "\t[1] Deposit." << endl;
	cout << "\t[2] Withdraw." << endl;
	cout << "\t[3] Total Balance." << endl;
	cout << "\t[4] Exit." << endl;
	cout << "====================================================\n";

	PerfromTransactionsMenueScreen((enTransactionMenueOption)ReadTransactionMenueOpration());
}

void ShowMainMenue()
{
	system("cls");
	cout << "====================================================\n";
	cout << "\t\tMain Menue Screen \n";
	cout << "====================================================\n";
	cout << "\t[1] Print All Clients." << endl;
	cout << "\t[2] Add New client." << endl;
	cout << "\t[3] Delete Client." << endl;
	cout << "\t[4] Update Client Info." << endl;
	cout << "\t[5] Find Client." << endl;
	cout << "\t[6] Transaction Menue." << endl;
	cout << "\t[7] Manages users." << endl;
	cout << "\t[8] Logout." << endl;
	cout << "====================================================\n";



	PerfromMainMenueOption((enMainMenueOption)ReadMainMenueOpration());

}

bool  LoadUserInfo(string Username, string Password)
{

	if (FindUserByUsernameAndPassword(Username, Password, CurrentUser))
		return true;
	else
		return false;

}

void Login()
{
	bool LoginFaild = false;

	string Username, Password;
	do
	{
		system("cls");

		cout << "\n---------------------------------\n";
		cout << "\tLogin Screen";
		cout << "\n---------------------------------\n";

		if (LoginFaild)
		{
			cout << "Invlaid Username/Password!\n";
		}

		cout << "Enter Username? ";
		cin >> Username;

		cout << "Enter Password? ";
		cin >> Password;

		LoginFaild = !LoadUserInfo(Username, Password);

	} while (LoginFaild);

	ShowMainMenue();

}

int main()
{


	Login();
	system("pause>0");

	return 0;
}
