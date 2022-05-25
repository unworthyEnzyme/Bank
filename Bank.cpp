#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "helpers.h"

enum Result
{
    Ok,
    Err,
};

class BankAccount
{
    double m_balance;
    int m_id;
    std::string m_password;
    std::vector<double> m_ins_and_outs;
    double m_limit;
public:
    BankAccount(int id, std::string password)
    {
        m_balance = 0;
        m_id = id;
        m_password = password;
        m_ins_and_outs = {};
    }
    bool check_credentials(int id, std::string password) 
    {
        return id == m_id && password == m_password;
    }
    void deposit(double quantity) 
    {
        m_balance += quantity;
        m_ins_and_outs.push_back(quantity);
    }
    Result withdraw(double quantity) 
    {
        if (quantity > m_limit) return Err;
        if (quantity > m_balance) return Err;
        m_balance += quantity;
        m_ins_and_outs.push_back(-quantity);
        return Ok;
    }
    std::vector<double>& ins_and_outs() 
    {
        return m_ins_and_outs;
    }
    double balance_inquiry()
    {
        return m_balance;
    }
    void change_password(std::string new_password) 
    {
        m_password = new_password;
    }
    Result change_limit(double limit) 
    {
        m_limit = limit;
        return Ok;
    }
};

class Bank
{
    std::map<int, BankAccount> m_bank_accounts;
public:
    Bank()
    {
        m_bank_accounts = {};
    }
    Result create_account(int id, std::string password) 
    {
        if (does_account_exists(id)) return Err;
        BankAccount newBankAccount(id, password);
        m_bank_accounts.emplace(id, newBankAccount);
        return Ok;
    }
    Result login(int id, std::string password) 
    {
        if (!does_account_exists(id)) return Err;
        BankAccount account = m_bank_accounts.find(id)->second;
        if (!account.check_credentials(id, password)) return Err;
        return Ok;
    }
    Result withdraw(int id, double amount) 
    {
        if (!does_account_exists(id)) return Err;
        BankAccount account = m_bank_accounts.find(id)->second;
        return account.withdraw(amount);
        
    }
    Result deposit(int id, double amount) 
    {
        if (!does_account_exists(id)) return Err;
        BankAccount account = m_bank_accounts.find(id)->second;
        account.deposit(amount);
        return Ok;
    }
    double balance_inquiry(int id) 
    {
        //check if the user exists in the caller
        BankAccount account = m_bank_accounts.find(id)->second;
        return account.balance_inquiry();
    }
    bool does_account_exists(int id) 
    {
        return does_map_key_exists(m_bank_accounts, id);
    }
    std::vector<double>& ins_and_outs(int id) {}
    Result money_transfer(int from, int to, double amount) {}
    Result change_password(int id, std::string new_password) {}
};

const std::string INDEX_PROMPT = 
"1 => Login\n"
"2 => Create an Account\n"
"3 => Quit the Applicaton\n";
const std::string LOGGED_IN_PROMPT =
"1 => Withdraw\n"
"2 => Deposit\n"
"3 => Balance Inquiry\n"
"4 => Change Password\n"
"5 => Change Limit\n"
"6 => Ins and Outs\n"
"7 => Log out\n";

class Program
{
    int m_current_account_id;
    Bank m_bank;
public:
    Program()
    {
        m_bank = Bank();
        m_current_account_id = NULL;
    }
    void index_page() 
    {
        std::cout << INDEX_PROMPT;
        std::string command = input<std::string>(" > ");
        if (command.length() > 1) {
            std::cout << "Invalid command!\n";
            index_page();
        }
        switch (command[0])
        {
        case '1':
            login();
            break;
        case '2':
            create_account();
            break;
        case '3':
            return;
            break;
        default:
            std::cout << "Invalid command!\n";
            index_page();
            break;
        }
    }
    void loggedin_page() 
    {
        std::cout << LOGGED_IN_PROMPT;
        std::string command = input<std::string>(" > ");
        if (command.length() > 1) {
            std::cout << "Invalid Command!\n";
            loggedin_page();
        }
        switch (command[0])
        {
        case '1':
            break;
        case '2':
            deposit();
            break;
        case '3':
            balance_inquiry();
            break;
        case '4':
            break;
        case '5':
            break;
        case '6':
            break;
        case '7':
            index_page();
            break;
        default:
            std::cout << "Invalid Command!\n";
            loggedin_page();
            break;
        }
    }
    void create_account()
    {
        int id = input<int>("Enter your id: ");
        std::string password = input<std::string>("Enter your password: ");
        Result result = m_bank.create_account(id, password);
        switch (result)
        {
        case Ok:
            m_current_account_id = id;
            loggedin_page();
            break;
        case Err:
            std::cout << "Cannot create an account\n";
            create_account();
            break;
        default:
            std::cout << "Cannot create an account\n";
            break;
        }
    }
    void login()
    {
        int id = input<int>("Enter you id: ");
        std::string password = input<std::string>("Enter your password: ");
        Result result = m_bank.login(id, password);
        switch (result)
        {
        case Ok:
            std::cout << "You are logged in\n";
            m_current_account_id = id;
            loggedin_page();
            break;
        case Err:
            std::cout << "Couldn't log in\n";
            login();
            break;
        default:
            std::cout << "Could't log in\n";
            login();
            break;
        }
    }
    void deposit()
    {
        double amount = input<double>("Enter an amount: ");
        //if someone can choose they have to logged in so i don't have check if the user exists
        m_bank.deposit(m_current_account_id, amount);
        loggedin_page();
    }
    void balance_inquiry()
    {
        std::cout << m_bank.balance_inquiry(m_current_account_id) << "\n";
        loggedin_page();
    }
};



int main()
{
    Program program = Program();
    program.index_page();
}
