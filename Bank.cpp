#include <iostream>
#include <string>
#include <vector>
#include <map>

class BankAccount
{
    double m_balance;
    int m_id;
    std::string m_password;
    std::vector<double> m_ins_and_outs;
public:
    BankAccount(int id, std::string password)
    {
        m_balance = 0;
        m_id = id;
        m_password = password;
        m_ins_and_outs = {};
    }
    bool check_credentials(int id, std::string password) {}
    void deposit(double quantity) {}
    Result withdraw(double quantity) {}
    std::vector<double>& balance_inquiry() {}
    void change_password(std::string new_password) {}
    Result change_limit() {}
};

enum Result
{
    Ok,
    Err,
};

class Bank
{
    std::map<int, BankAccount> m_bank_accounts;
public:
    Bank()
    {
        m_bank_accounts = {};
    }
    Result create_account(int id, std::string password) {}
    Result login(int id, std::string password) {}
    Result withdraw(int id, double amount) {}
    void deposit(int id, double amount) {}
    double balance_inquiry(int id) {}
    bool does_account_exists(int id) {}
    std::vector<double>& ins_and_outs(int id) {}
    Result money_transfer(int from, int to, double amount) {}
    Result change_password(int id, std::string new_password) {}
};

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
    void index_page() {}
    void loggedin_page() {}

};



int main()
{
    std::cout << "Hello World!\n";
}
