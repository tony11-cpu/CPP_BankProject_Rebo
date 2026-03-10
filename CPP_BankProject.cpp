#pragma warning(disable : 4996)
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
using namespace std;
const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";
const string OldTransactionsFile = "OldTransactions.txt";
enum enTransactionsMenueOptions { eDeposit = 1, eWithdraw = 2, eShowTotalBalance = 3, eShowMainMenue = 4 };
enum enMainMenueOptions { eListClients = 1, eAddNewClient = 2, eDeleteClient = 3, eUpdateClient = 4, eFindClient = 5, eShowTransactionsMenue = 6, eMangeUsers = 7, eLogout = 8, eChoose = 9 };
enum enMangeUserOption { enListUsers = 1, enAddNewUser = 2, enDeleteUser = 3, enUbdateUser = 4, enFindUser = 5, enMainMenu = 6 };
enum enPermissions { pShowClientList = 1, pAddNewClient = 2, pDeleteClient = 4, pUbdateClient = 8, pFindClient = 16, pTransactions = 32, pMangeUsers = 64, pAll = -1 };
enum enWithdrawOptions { e20 = 1, e50 = 2, e100 = 3, e200 = 4, e400 = 5, e600 = 6, e800 = 7, e1000 = 8, eExit = 9 };
enum enATMoptions { eQuickWithDraw = 1, eNormalWithDraw = 2, eDeposite = 3, eCheckBAlance = 4, ALogout = 6, ePastTransactions = 5, eBackTomain = 7 };
void ShowMainMenue();
void ShowTransactionsMenue();
void ShowMangeUserMenu();
void GoBackToMainMenue();
void OptionScreen();
void ShowLoginScreen();
void ATMscreen();
void LoginATM();
void GoBackToATMscreen();
struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;


};
struct sUser
{
    string Name;
    string Password;
    short Permissions;
    bool MarkForDelete = false;
};
struct sDate
{
    short Year;
    short Month;
    short Day;
    string WhatTrans;
};
sDate GetSystemDate()
{
    sDate Date;
    time_t t = time(0);
    tm* now = localtime(&t);
    Date.Year = now->tm_year + 1900;
    Date.Month = now->tm_mon + 1;
    Date.Day = now->tm_mday;
    return Date;
}

sUser CurrentUSer;
sClient CurrentClient;

void PrintERORRmessege()
{
    cout << "\n------------------------------------\n";
    cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin.";
    cout << "\n------------------------------------\n";
}
bool CheckPermissions(enPermissions Permission)
{
    if (CurrentUSer.Permissions == enPermissions::pAll)
        return true;

    if (CurrentUSer.Permissions & Permission)
        return true;
    else
        return false;

}
string ReadUsername()
{
    string name;
    cout << "\nPlease enter UserName : ";
    getline(cin >> ws, name);
    return name;
}

vector<string> SplitString(string S1, string Delim)
{

    vector<string> vString;

    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;

}
sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{

    sClient Client;
    vector<string> vClientData;

    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double


    return Client;

}
sUser ConvertUserLinetoRecord(string Line, string Seperator = "#//#")
{

    sUser user;
    vector<string> vClientData;
    vClientData = SplitString(Line, Seperator);

    user.Name = vClientData[0];
    user.Password = vClientData[1];
    user.Permissions = stoi(vClientData[2]);

    return user;

}
sDate ConvertDateLinetoRecord(string Line, string Seperator = "/")
{

    sDate date;
    vector<string> vClientData;
    vector<string> vClientData1;

    vClientData = SplitString(Line, Seperator);

    date.Day = stoi(vClientData[0]);
    date.Month = stoi(vClientData[1]);
    date.Year = stoi(vClientData[2]);
    date.WhatTrans = vClientData[3];

    return date;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;

}

string ConvertRecordToLine(sUser user, string Seperator = "#//#")
{
    string stClientRecord = "";

    stClientRecord += user.Name + Seperator;
    stClientRecord += user.Password + Seperator;
    stClientRecord += to_string(user.Permissions);
    return stClientRecord;

}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }


            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return false;


}

sClient ReadNewClient()
{
    sClient Client;

    cout << "Enter Account Number? ";

    // Usage of std::ws will extract allthe whitespace character
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }


    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;

}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);

            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return vClients;

}
vector <sUser> LoadUserDataFromFile(string FileName)
{

    vector <sUser> vUser;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sUser user;

        while (getline(MyFile, Line))
        {

            user = ConvertUserLinetoRecord(Line);

            vUser.push_back(user);
        }

        MyFile.close();

    }

    return vUser;

}
vector <sDate> LoadTimeDataFromFile(string FileName)
{
    vector <sDate> vDate;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sDate date;

        while (getline(MyFile, Line))
        {

            date = ConvertDateLinetoRecord(Line);

            vDate.push_back(date);
        }

        MyFile.close();

    }

    return vDate;

}

void PrintClientRecordLine(sClient Client)
{

    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;

}

void PrintUserRecordLine(sUser user)
{
    cout << "| " << setw(15) << left << user.Name;
    cout << "| " << setw(10) << left << user.Password;
    cout << "| " << setw(40) << left << user.Permissions;
}

void PrintClientRecordBalanceLine(sClient Client)
{

    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;

}

void ShowAllClientsScreen()
{
    if (!CheckPermissions(enPermissions::pShowClientList))
    {
        PrintERORRmessege();
        GoBackToMainMenue();
    }

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sClient Client : vClients)
        {

            PrintClientRecordLine(Client);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

}

void ShowTotalBalances()
{

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    double TotalBalances = 0;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sClient Client : vClients)
        {

            PrintClientRecordBalanceLine(Client);
            TotalBalances += Client.AccountBalance;

            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "\t\t\t\t\t   Total Balances = " << TotalBalances;

}

void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code     : " << Client.PinCode;
    cout << "\nName         : " << Client.Name;
    cout << "\nPhone        : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";
}

void PrintUserCard(sUser user)
{
    cout << "\nThe following are the User details:\n\n";
    cout << "-----------------------------------";
    cout << "\nUsername: " << user.Name;
    cout << "\nPassword: " << user.Password;
    cout << "\nPermissions: " << user.Permissions;
    cout << "\n-----------------------------------\n\n";
}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{

    for (sClient C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }

    }
    return false;

}

bool IsClientInfile(string AccountNumber, string PinCode, sClient& Client)
{

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    for (sClient& C : vClients)
    {

        if (C.AccountNumber == AccountNumber && PinCode == C.PinCode)
        {
            Client = C;
            return true;
        }

    }
    return false;

}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;

}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    for (sClient& C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }

    }

    return false;

}
bool MarkUserForDeleteByAccountNumber(string AccountNumber, vector <sUser>& vClients)
{

    for (sUser& C : vClients)
    {

        if (C.Name == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }

    }

    return false;

}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient>& vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (sClient& C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vClients;

}

vector <sUser> SaveUserDataToFile(string FileName, vector <sUser>& vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (sUser& C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vClients;

}

void AddDataLineToFile(string FileName, string  stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {

        MyFile << stDataLine;
        MyFile << endl;

        MyFile.close();
    }

}

void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));

}

string ReadPass()
{
    string pass;
    cout << "Password : ";
    getline(cin >> ws, pass);
    return pass;
}

void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        //system("cls");
        cout << "Adding New Client:\n\n";

        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";


        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');

}

bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);

        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);

            //Refresh Clients 
            vClients = LoadCleintsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

}

bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {

            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }

            }

            SaveCleintsDataToFile(ClientsFileName, vClients);

            cout << "\n\nClient Updated Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients)
{


    char Answer = 'n';


    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;

                return true;
            }

        }


        return false;
    }

}

string ReadClientAccountNumber()
{
    string AccountNumber = "";

    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;

}

void ShowDeleteClientScreen()
{
    if (!CheckPermissions(enPermissions::pDeleteClient))
    {
        PrintERORRmessege();
        GoBackToMainMenue();
    }

    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);

}

void ShowUpdateClientScreen()
{
    if (!CheckPermissions(enPermissions::pUbdateClient))
    {
        PrintERORRmessege();
        GoBackToMainMenue();
    }

    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);

}

void ShowAddNewClientsScreen()
{
    if (!CheckPermissions(enPermissions::pAddNewClient))
    {
        PrintERORRmessege();
        GoBackToMainMenue();
    }

    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";

    AddNewClients();

}

void ShowFindClientScreen()
{
    if (!CheckPermissions(enPermissions::pFindClient))
    {
        PrintERORRmessege();
        GoBackToMainMenue();
    }

    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";

}

void ShowDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";


    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();


    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }


    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> Amount;

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);

}

void ShowWithDrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";

    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();


    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter withdraw amount? ";
    cin >> Amount;

    //Validate that the amount does not exceeds the balance
    while (Amount > Client.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> Amount;
    }

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);

}

void ShowTotalBalancesScreen()
{

    ShowTotalBalances();

}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();

}
void GoBackToTransactionsMenue()
{
    cout << "\n\nPress any key to go back to Transactions Menue...";
    system("pause>0");
    ShowTransactionsMenue();

}
short ReadTransactionsMenueOption()
{
    cout << "Choose what do you want to do? [1 to 4]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void PerfromTranactionsMenueOption(enTransactionsMenueOptions TransactionMenueOption)
{
    switch (TransactionMenueOption)
    {
    case enTransactionsMenueOptions::eDeposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionsMenue();
        break;
    }

    case enTransactionsMenueOptions::eWithdraw:
    {
        system("cls");
        ShowWithDrawScreen();
        GoBackToTransactionsMenue();
        break;
    }


    case enTransactionsMenueOptions::eShowTotalBalance:
    {
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionsMenue();
        break;
    }


    case enTransactionsMenueOptions::eShowMainMenue:
    {

        ShowMainMenue();

    }
    }

}

void ShowTransactionsMenue()
{
    if (!CheckPermissions(enPermissions::pTransactions))
    {
        PrintERORRmessege();
        GoBackToMainMenue();
    }

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tTransactions Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "===========================================\n";
    PerfromTranactionsMenueOption((enTransactionsMenueOptions)ReadTransactionsMenueOption());
}

short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 8]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

short ReadMangeUserOption()
{
    cout << "Choose what do you want to do? [1 to 6]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void ShowLsitUser()
{
    vector <sUser> vUser = LoadUserDataFromFile(UsersFileName);

    cout << "\n\t\t\t\t\tUsers List (" << vUser.size() << ") User(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "User Name";
    cout << "| " << left << setw(10) << "Password";
    cout << "| " << left << setw(40) << "Permissions";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vUser.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sUser& user : vUser)
        {
            PrintUserRecordLine(user);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

void GoBackToUserMangeOptions()
{
    cout << "\nPress any key to go back to mange options..." << endl;
    system("pause >0");
    ShowMangeUserMenu();
}

bool isUserInfile(sUser& ReUser, string username, vector<sUser>& users)
{
    for (sUser& u : users)
    {
        if (u.Name == username)
        {
            ReUser = u;
            return true;
        }
    }return false;
}

//Find User ...
void FindUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind User Screen";
    cout << "\n-----------------------------------\n";
    string username = ReadUsername();
    vector<sUser> users = LoadUserDataFromFile(UsersFileName);
    sUser user;
    if (isUserInfile(user, username, users))
        PrintUserCard(user);
    else
        cout << "\nUser with USERNAME [ " << username << " ] is not found" << endl;
}
bool UserExsistByUsername(string Username, string filname)
{
    vector<sUser> Vec = LoadUserDataFromFile(UsersFileName);
    for (sUser& user : Vec)
    {
        if (user.Name == Username)
            return true;
    }
    return false;
}

short Permissions()
{
    sUser user;
    user.Permissions = 0;

    cout << "Do You want to give full access ? "; char Access; cin >> Access;
    if (toupper(Access) == 'Y')
        return user.Permissions = -1;
    else
    {
        cout << "\nDo you want to give access to : ";

        cout << "\n\nShow Client ? "; cin >> Access;
        if (toupper(Access) == 'Y')
            user.Permissions += 1;

        cout << "\nAdd New Client ? "; cin >> Access;
        if (toupper(Access) == 'Y')
            user.Permissions += 2;

        cout << "\nDelete Client ? "; cin >> Access;
        if (toupper(Access) == 'Y')
            user.Permissions += 4;

        cout << "\nUbdate Client ? "; cin >> Access;
        if (toupper(Access) == 'Y')
            user.Permissions += 8;

        cout << "\nFind Client ? "; cin >> Access;
        if (toupper(Access) == 'Y')
            user.Permissions += 16;

        cout << "\nTransactions ? "; cin >> Access;
        if (toupper(Access) == 'Y')
            user.Permissions += 32;

        cout << "\nMange Users ? "; cin >> Access;
        if (toupper(Access) == 'Y')
            user.Permissions += 64;
    }
    return user.Permissions;
}

//New User Addintion ...
sUser ReadNewUser()
{
    sUser user;
    user.Name = ReadUsername();
    while (UserExsistByUsername(user.Name, UsersFileName))
    {
        cout << "\nUserName [ " << user.Name << " ] is already exsits please enter new one !\n";
        user.Name = ReadUsername();
    }

    user.Password = ReadPass();
    user.Permissions = Permissions();

    return user;
}
void NewUser()
{
    sUser user;
    user = ReadNewUser();
    AddDataLineToFile(UsersFileName, ConvertRecordToLine(user));
}
void ShowAddNewUserScreen()
{
    char AddMore = 'Y';
    do
    {
        system("cls");
        cout << "\n-----------------------------------\n";
        cout << "\tAdd New User Screen";
        cout << "\n-----------------------------------\n";

        cout << "Adding New User:\n\n";

        NewUser();
        cout << "\nuser Added Successfully, do you want to add more users? Y/N? ";


        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');


}

//Delete User ...
void DeleteUser(vector <sUser>& vec, sUser user, string username)
{
    if (isUserInfile(user, username, vec))
    {
        PrintUserCard(user); char deleteuser;
        cout << "Are You sure you want to delete this user ?? [y/n] "; cin >> deleteuser;

        if (toupper(deleteuser) == 'Y')
        {
            MarkUserForDeleteByAccountNumber(username, vec);
            SaveUserDataToFile(UsersFileName, vec);

            vec = LoadUserDataFromFile(UsersFileName);
            cout << "\nuser Deleted Succefully ..." << endl;
        }
    }
    else
    {
        cout << "\nUser with username [ " << username << " ] is not found" << endl;
    }
}
void ShowDeleteUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete User Screen";
    cout << "\n-----------------------------------\n";
    string username = ReadUsername();
    vector<sUser> vec = LoadUserDataFromFile(UsersFileName);
    sUser user;

    DeleteUser(vec, user, username);

}

//Ubdate User ...
sUser Ubdateusercard(sUser user)
{
    cout << "Please enter your password : ";
    getline(cin >> ws, user.Password);

    user.Permissions = Permissions();
    return user;
}
void UbdateUser(vector <sUser>& vec, sUser user, string username)
{
    if (isUserInfile(user, username, vec))
    {
        PrintUserCard(user); char deleteuser;
        cout << "Are You sure you want to Ubdate this user ?? [y/n] "; cin >> deleteuser;


        if (toupper(deleteuser) == 'Y')
        {
            for (sUser& u : vec)
            {
                if (u.Name == username)
                    u = Ubdateusercard(u);
            }

            SaveUserDataToFile(UsersFileName, vec);

            cout << "\nuser Ubdated Succefully ..." << endl;
        }
    }
    else
    {
        cout << "\nUser with username [ " << username << " ] is not found" << endl;
    }
}
void ShowUbdateuserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUbdate User Screen";
    cout << "\n-----------------------------------\n";
    string username = ReadUsername();
    vector<sUser> vec = LoadUserDataFromFile(UsersFileName);
    sUser user;

    UbdateUser(vec, user, username);

}

//Manging user ...
void PerFormMangeUserOption(enMangeUserOption UserMange)
{
    switch (UserMange)
    {
    case enListUsers:
    {
        system("cls");
        ShowLsitUser();
        GoBackToUserMangeOptions();
        break;
    }
    case enAddNewUser:
    {
        system("cls");
        ShowAddNewUserScreen();
        GoBackToUserMangeOptions();
        break;
    }
    case enDeleteUser:
    {
        system("cls");
        ShowDeleteUserScreen();
        GoBackToUserMangeOptions();
        break;
    }
    case enUbdateUser:
    {
        system("cls");
        ShowUbdateuserScreen();
        GoBackToUserMangeOptions();
        break;
    }
    case enFindUser:
    {
        system("cls");
        FindUserScreen();
        GoBackToUserMangeOptions();
        break;
    }
    case enMainMenu:
    {
        system("cls");
        ShowMainMenue();
        break;
    }
    }
}
void ShowMangeUserMenu()
{
    if (!CheckPermissions(enPermissions::pMangeUsers))
    {
        PrintERORRmessege();
        GoBackToMainMenue();
    }

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMange User Menu\n";
    cout << "===========================================\n";
    cout << "\t[1] List Users " << endl;
    cout << "\t[2] Add New User " << endl;
    cout << "\t[3] Delete User " << endl;
    cout << "\t[4] Ubdate User " << endl;
    cout << "\t[5] Find User " << endl;
    cout << "\t[6] Main Menu " << endl;
    cout << "===========================================\n";
    PerFormMangeUserOption((enMangeUserOption)ReadMangeUserOption());
}

//OLD TRANSACTION
string WhatTransaction(enATMoptions Options, sDate& date)
{

    switch (Options)
    {
    case enATMoptions::eDeposite:
    {
        return date.WhatTrans = "Depositing";
    }
    case enATMoptions::eQuickWithDraw:
    {
        return date.WhatTrans = "WithDrawing";
    }
    }
}
void OldTransactions(bool TransDone, enATMoptions option)
{
    if (TransDone)
    {
        sDate date = GetSystemDate();
        date.WhatTrans = WhatTransaction(option, date);
        fstream MyFile;
        MyFile.open(OldTransactionsFile, ios::out | ios::app);
        if (MyFile.is_open())
        {
            MyFile << date.Day << "/" << date.Month << "/" << date.Year << "/" << date.WhatTrans << endl;
        }
        MyFile.close();
    }
}
void showOldTransactions()
{
    cout << "===========================================\n";
    cout << "\tOld Transactions Screen\n";
    cout << "===========================================\n";
    vector <sDate> vDate = LoadTimeDataFromFile(OldTransactionsFile);
    if (vDate.size() == 0)
        cout << "\nThere is no transaction done Reacently ! " << endl;

    short counter = 0;
    for (sDate& d : vDate)
    {
        counter++;
        cout << "\n[" << counter << "] " << d.Day << "/" << d.Month << "/" << d.Year << " " << d.WhatTrans << endl;
    }
}

//Quick WithDraw ...
void QuickWithDrawTransaction(int Option)
{
    vector<sClient>client = LoadCleintsDataFromFile(ClientsFileName);
    for (sClient& c : client)
    {
        if (CurrentClient.AccountNumber == c.AccountNumber && CurrentClient.PinCode == c.PinCode && Option <= CurrentClient.AccountBalance)
        {
            c.AccountBalance -= Option;
            CurrentClient.AccountBalance -= Option;
            SaveCleintsDataToFile(ClientsFileName, client);
            OldTransactions(true, enATMoptions::eQuickWithDraw);
        }
        else
        {
            cout << "\nAmount Exceeds your balance, please make sure your range in [" << CurrentClient.AccountBalance << "]";
            GoBackToATMscreen();
        }
    }
}
void QuickWithdrawCalculations(enWithdrawOptions Withdraw)
{
    switch (Withdraw)
    {
    case e20:
        QuickWithDrawTransaction(20);
        break;
    case e50:
        QuickWithDrawTransaction(50);
        break;
    case e100:
        QuickWithDrawTransaction(100);
        break;
    case e200:
        QuickWithDrawTransaction(200);
        break;
    case e400:
        QuickWithDrawTransaction(400);
        break;
    case e600:
        QuickWithDrawTransaction(600);
        break;
    case e800:
        QuickWithDrawTransaction(800);
        break;
    case e1000:
        QuickWithDrawTransaction(1000);
        break;
    case eExit:
        GoBackToATMscreen();
        break;
    }
}
short QuickWithdrawOptions()
{
    short number = 0;
    cout << "\nChoose what to do from [1 to 9] ? ";
    cin >> number;

    return number;
}
void QuickWithDrawMange(enWithdrawOptions Withdraw)
{
    char trans = 'n';
    cout << "Are you sure you want to do this transaction [y/n] ?? ";
    cin >> trans;
    vector<sClient> client = LoadCleintsDataFromFile(ClientsFileName);
    if (toupper(trans) == 'Y')
    {
        QuickWithdrawCalculations(Withdraw);
        cout << "\nDone Successfully. Your New Balance now is " << CurrentClient.AccountBalance;
    }
    GoBackToATMscreen();
}
void ShowQuickWithdraw()
{
    cout << "===========================================\n";
    cout << "\t\tQucik Withdraw\n";
    cout << "===========================================\n";
    cout << "\t[1] 20\t\t[2] 50\n";
    cout << "\t[3] 100\t\t[4] 200\n";
    cout << "\t[5] 400\t\t[6] 600\n";
    cout << "\t[7] 800\t\t[8] 1000\n";
    cout << "\t[9] Exit\n";
    cout << "===========================================\n";
    cout << "Your Balance is " << CurrentClient.AccountBalance << endl;

    QuickWithDrawMange((enWithdrawOptions)QuickWithdrawOptions());
}

void CheckingAndTransactingNormalWithdraw(int amount)
{
    char Accept = 'n';
    while (amount % 5 != 0)
    {
        cout << "\nInvalid Transaction PLEASE enter amount multiple of 5's ! "; cin >> amount;
    }
    cout << "\nAre you sure you want to perform this transaction [y/n] ? "; cin >> Accept;
    if (toupper(Accept) == 'Y')
    {
        QuickWithDrawTransaction(amount);
        cout << "\ntransaction Done successfuly, your balance is [" << CurrentClient.AccountBalance << "]";
    }
    GoBackToATMscreen();
}
void NormalWithdrawScreen()
{
    cout << "===========================================\n";
    cout << "\t\Normal Withdraw\n";
    cout << "===========================================\n";
    double amount = 0;
    cout << "\nEnter an amount that is multiple of 5's : "; cin >> amount;

    CheckingAndTransactingNormalWithdraw(amount);
}

void DepositeAddingmoney(double amount)
{
    vector<sClient>client = LoadCleintsDataFromFile(ClientsFileName);
    for (sClient& c : client)
    {
        c.AccountBalance += amount;
        CurrentClient.AccountBalance += amount;
        SaveCleintsDataToFile(ClientsFileName, client);
        OldTransactions(true, enATMoptions::eDeposite);
    }
}
void DepositeScreen()
{
    cout << "===========================================\n";
    cout << "\t\tDeposites Screen\n";
    cout << "===========================================\n";
    double Amount = 0;
    cout << "\nPlease enter amount you want to deposite : "; cin >> Amount;

    char Accept = 'n';
    cout << "\nAre you sure you want to perform this transaction [y/n] ? "; cin >> Accept;
    if (toupper(Accept) == 'Y')
    {
        DepositeAddingmoney(Amount);
        cout << "\nDone Successfully, your Balance now is [" << CurrentClient.AccountBalance << "]";
    }
    else
    {
        GoBackToATMscreen();
    }
}

void GoBackToATMscreen()
{
    cout << "\n\nPress Any Key To Go Back To ATM Menu..." << endl;
    system("pause >0");
    ATMscreen();
}

void CheckBalanceScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tCheck Balance screen";
    cout << "\n-----------------------------------\n";

    cout << "\nYour Balance is : " << CurrentClient.AccountBalance;

}

void PerformATMmeunuOption(enATMoptions ATM)
{
    switch (ATM)
    {
    case eQuickWithDraw:
    {
        system("cls");
        ShowQuickWithdraw();
        break;
    }
    case eNormalWithDraw:
    {
        system("cls");
        NormalWithdrawScreen();
        GoBackToATMscreen();
        break;
    }
    case eDeposite:
    {
        system("cls");
        DepositeScreen();
        GoBackToATMscreen();
        break;
    }
    case eCheckBAlance:
    {
        system("cls");
        CheckBalanceScreen();
        GoBackToATMscreen();
        break;
    }
    case ePastTransactions:
    {
        system("cls");
        showOldTransactions();
        GoBackToATMscreen();
    }
    case ALogout:
    {
        LoginATM();
        break;
    }
    case eBackTomain:
    {
        OptionScreen();
    }
    }
}

short ReadATMoption()
{
    cout << "Choose what do you want to do? [1 to 7]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}
void ATMscreen()
{
    system("cls");
    cout << "====================================================\n";
    cout << "\tATM Main Menu Screen - WELCOME MR " << CurrentClient.Name << endl;
    cout << "====================================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposite.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] Past Transactions.\n";
    cout << "\t[6] Logout.\n";
    cout << "\t[7] Main Menu.\n";
    cout << "====================================================\n";
    PerformATMmeunuOption((enATMoptions)ReadATMoption());
}
void LoginATM()
{
    string AcountNumber = "", Pincode = "";
    bool Accept = false;
    do
    {
        system("cls");
        cout << "\n-----------------------------------\n";
        cout << "\tLogin ATM screen";
        cout << "\n-----------------------------------\n";
        if (Accept)
            cout << "Wrong PinCode / Acount Number !!\n";

        cout << "\nClient Acount Number : ";
        getline(cin >> ws, AcountNumber);
        cout << "Client PinCode : ";
        getline(cin >> ws, Pincode);

        Accept = !IsClientInfile(AcountNumber, Pincode, CurrentClient);

    } while (Accept);
    ATMscreen();
}

void PerfromMainMenueOption(enMainMenueOptions MainMenueOption)
{
    switch (MainMenueOption)
    {
    case enMainMenueOptions::eListClients:
    {
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eAddNewClient:
    {
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eDeleteClient:
    {
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eUpdateClient:
    {
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eFindClient:
    {
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eShowTransactionsMenue:
    {
        system("cls");
        ShowTransactionsMenue();
        break;
    }
    case enMainMenueOptions::eMangeUsers:
    {
        system("cls");
        ShowMangeUserMenu();
        break;
    }
    case enMainMenueOptions::eLogout:
    {
        system("cls");
        ShowLoginScreen();
        break;
    }
    case enMainMenueOptions::eChoose:
        OptionScreen();
    }

}
void ShowMainMenue()
{
    system("cls");
    cout << "=================================================\n";
    cout << "\tMain Menue Screen - WELCOME " << CurrentUSer.Name << endl;
    cout << "=================================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "\t[9] Main Screen.\n";
    cout << "===========================================\n";
    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}

//Login Screen UESER ... // useable code.......
bool CheckIfuserInSystem(string pass, string username, sUser& User)
{
    vector <sUser> vUser = LoadUserDataFromFile(UsersFileName);
    for (sUser& user : vUser)
        if (user.Name == username && user.Password == pass)
        {
            User = user;
            return true;
        }
    return false;
}
void ShowLoginScreen()
{
    string name = "", pass = "";
    short counter = 0;
    bool userInSy = false;
    do
    {
        system("cls");
        cout << "\n---------------------------" << endl;
        cout << "\tLogin Screen " << endl;
        cout << "---------------------------" << endl;
        if (counter++ >= 1)
            cout << "Wrong Pass/Username !!\n";
        name = ReadUsername();
        pass = ReadPass();

        userInSy = !CheckIfuserInSystem(pass, name, CurrentUSer);
    } while (userInSy);
    ShowMainMenue();
}

void OptionScreen()
{
    system("cls");
    cout << "====================================================\n";
    cout << "\t\tBank Main System \n";
    cout << "====================================================\n";
    cout << " ==============  =================================== " << endl;
    cout << "|     ATM      ||     Manage Users And Clients      |" << endl;
    cout << " ==============  ===================================  " << endl;
    cout << "\n ATM Option [1] \tManging users [2]  "; short Answer; cin >> Answer;
    if (Answer == 1)
        LoginATM();
    else
        ShowLoginScreen();
}

int main()
{
    OptionScreen();
    system("pause>0");
    return 0;
}
