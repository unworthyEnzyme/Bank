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
    std::vector<double> m_draws_and_deposits;
    double m_limit;
    double m_gold;
public:
    BankAccount(int id, std::string password)
    {
        m_balance = 0;
        m_id = id;
        m_password = password;
        m_draws_and_deposits = {};
        m_gold = 0;
    }
    double get_gold_amount()
    {
        return m_gold;
    }
    Result buy_gold(double amount, int exchange_rate)
    {
        if (amount * exchange_rate > m_balance) return Err;
        m_balance -= amount * exchange_rate;
        m_gold += amount;
        m_draws_and_deposits.push_back(-amount * exchange_rate);
        return Ok;
    }
    Result sell_gold(double amount, int exchange_rate)
    {
        if (amount > m_gold) return Err;
        m_gold -= amount;
        m_balance += amount * exchange_rate;
        m_draws_and_deposits.push_back(amount * exchange_rate);
        return Ok;
    }
    bool check_credentials(int id, std::string password) 
    {
        return id == m_id && password == m_password;
    }
    void deposit(double quantity) 
    {
        m_balance += quantity;
        m_limit = m_balance;
        m_draws_and_deposits.push_back(quantity);
    }
    Result withdraw(double quantity) 
    {
        if (quantity > m_limit) return Err;
        if (quantity > m_balance) return Err;
        m_balance -= quantity;
        m_draws_and_deposits.push_back(-quantity);
        return Ok;
    }
    std::vector<double>& draws_and_deposits() 
    {
        return m_draws_and_deposits;
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
        if (limit <= 0) return Err;
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
    double get_gold_amount(int id)
    {
        //check if the user exists in the caller
        BankAccount& account = m_bank_accounts.find(id)->second;
        return account.get_gold_amount();
    }
    Result buy_gold(int id, double amount, int exc_rate)
    {
        if (!does_account_exists(id)) return Err;
        BankAccount& account = m_bank_accounts.find(id)->second;
        Result result = account.buy_gold(amount, exc_rate);
        return result;
    }
    Result sell_gold(int id, double amount, int exc_rate)
    {
        if (!does_account_exists(id)) return Err;
        BankAccount& account = m_bank_accounts.find(id)->second;
        Result result = account.sell_gold(amount, exc_rate);
        return result;
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
        BankAccount& account = m_bank_accounts.find(id)->second;
        if (!account.check_credentials(id, password)) return Err;
        return Ok;
    }
    Result withdraw(int id, double amount)
    {
        if (!does_account_exists(id)) return Err;
        BankAccount& account = m_bank_accounts.find(id)->second;
        return account.withdraw(amount);

    }
    Result deposit(int id, double amount)
    {
        if (!does_account_exists(id)) return Err;
        BankAccount& account = m_bank_accounts.find(id)->second;
        account.deposit(amount);
        return Ok;
    }
    double balance_inquiry(int id)
    {
        //check if the user exists in the caller
        BankAccount& account = m_bank_accounts.find(id)->second;
        return account.balance_inquiry();
    }
    bool does_account_exists(int id)
    {
        return does_map_key_exists(m_bank_accounts, id);
    }
    std::vector<double>& draws_and_deposits(int id) 
    {
        //check if the user exists in the caller
        BankAccount& account = m_bank_accounts.find(id)->second;
        return account.draws_and_deposits();
    }
    Result money_transfer(int from, int to, double amount) 
    {
        if (!does_account_exists(from) || !does_account_exists(to)) return Err;
        BankAccount& from_account = m_bank_accounts.find(from)->second;
        BankAccount& to_account = m_bank_accounts.find(to)->second;
        if (from_account.balance_inquiry() < amount) return Err;
        from_account.withdraw(amount);
        to_account.deposit(amount);
        return Ok;
    }
    Result change_password(int id, std::string new_password)
    {
        if (!does_account_exists(id)) return Err;
        BankAccount& account = m_bank_accounts.find(id)->second;
        account.change_password(new_password);
        return Ok;
    }
    Result change_limit(int id, double new_limit)
    {
        if (!does_account_exists(id)) return Err;
        BankAccount& account = m_bank_accounts.find(id)->second;
        return account.change_limit(new_limit);
    }
};

const std::string INDEX_PROMPT = 
"1 => Login\n"
"2 => Create an Account\n"
"3 => Quit the Applicaton\n";
const std::string LOGGED_IN_PROMPT =
"a => Withdraw\n"
"b => Deposit\n"
"c => Balance Inquiry\n"
"d => Change Password\n"
"e => Change Limit\n"
"f => Buy Gold\n"
"g => Sell Gold\n"
"h => View Gold Exchange Rate\n"
"j => Transfer Money\n"
"k => View the List of Draws and Deposits\n"
"l => Log out\n";

class Program
{
    int m_current_account_id;
    Bank m_bank;
    int m_gold_exc_rate;
public:
    Program()
    {
        m_gold_exc_rate = get_gold_exchange_rate();
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
    void change_password()
    {
        std::string new_password = input<std::string>("Enter your new password: ");
        auto result = m_bank.change_password(m_current_account_id, new_password);
        switch (result)
        {
        case Ok:
            loggedin_page();
            break;
        case Err:
            std::cout << "An Error Occured\n";
            loggedin_page();
            break;
        default:
            std::cout << "An Error Occured\n";
            loggedin_page();
            break;
        }
    }
    void loggedin_page() 
    {
        std::cout << LOGGED_IN_PROMPT;
        std::string command = input<std::string>(" > ");
        switch (command[0])
        {
        case 'a':
            withdraw();
            break;
        case 'b':
            deposit();
            break;
        case 'c':
            balance_inquiry();
            break;
        case 'd':
            change_password();
            break;
        case 'e':
            change_limit();
            break;
        case 'f':
            buy_gold();
            break;
        case 'g':
            sell_gold();
            break;
        case 'h':
            view_gold_exchange_rate();
            break;
        case 'j':
            transfer_money();
            break;
        case 'k':
            draws_and_deposits();
            break;
        case 'l':
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
    void withdraw()
    {
        double amount = input<double>("Enter an amount: ");
        Result result = m_bank.withdraw(m_current_account_id, amount);
        switch (result)
        {
        case Ok:
            loggedin_page();
            break;
        case Err:
            std::cout << "You can withdraw that much\n";
            loggedin_page();
            break;
        default:
            break;
        }
    }
    void change_limit()
    {
        double amount = input<double>("Enter the new limit: ");
        Result result = m_bank.change_limit(m_current_account_id, amount);
        switch (result)
        {
        case Ok:
            loggedin_page();
            break;
        case Err:
            std::cout << "Can't change your limit!\n";
            loggedin_page();
            break;
        default:
            std::cout << "Can't change your limit!\n";
            loggedin_page();
            break;
        }
    }
    void transfer_money()
    {
        int to_id = input<int>("Enter the id of the account to transfer money: ");
        double amount = input<double>("Enter the amount of money to transfer: ");
        Result result = m_bank.money_transfer(m_current_account_id, to_id, amount);
        switch (result)
        {
        case Ok:
            std::cout << "Transfer is successfull\n";
            loggedin_page();
            break;
        case Err:
            std::cout << "Transfer is not successfull\n";
            loggedin_page();
            break;
        default:
            std::cout << "Transfer is not successfull\n";
            loggedin_page();
            break;
        }
    }
    void draws_and_deposits()
    {
        std::vector<double>& list = m_bank.draws_and_deposits(m_current_account_id);
        std::cout << "\tListing Draws and Deposits:\n";
        for (auto& k : list)
        {
            std::cout << "\t\t--> " << k << "\n";
        }
        loggedin_page();
    }
    void buy_gold()
    {
        std::cout << "EXCHANGE RATE: " << m_gold_exc_rate << "\n";
        std::cout << "You Currently Have: " << m_bank.get_gold_amount(m_current_account_id) << "g Gold\n";
        double amount = input<double>("Enter the amount you want to buy: ");
        Result result = m_bank.buy_gold(m_current_account_id, amount, m_gold_exc_rate);
        switch (result)
        {
        case Ok:
            std::cout << "Operation is successful\n";
            std::cout << "You have left " << m_bank.balance_inquiry(m_current_account_id) << " of dollars\n";
            m_gold_exc_rate = get_gold_exchange_rate();
            loggedin_page();
            break;
        case Err:
            std::cout << "There was an error\n";
            m_gold_exc_rate = get_gold_exchange_rate();
            loggedin_page();
            break;
        default:
            break;
        }
    }

    void sell_gold()
    {
        std::cout << "EXCHANGE RATE: " << m_gold_exc_rate << "\n";
        std::cout << "You Currently Have: " << m_bank.get_gold_amount(m_current_account_id) << "g Gold\n";
        double amount = input<double>("Enter the amount you want to sell: ");
        Result result = m_bank.sell_gold(m_current_account_id, amount, m_gold_exc_rate);
        switch (result)
        {
        case Ok:
            std::cout << "Operation is successful\n";
            std::cout << "You have " << m_bank.balance_inquiry(m_current_account_id) << " of dollars\n";
            m_gold_exc_rate = get_gold_exchange_rate();
            loggedin_page();
            break;
        case Err:
            std::cout << "There was an error\n";
            m_gold_exc_rate = get_gold_exchange_rate();
            loggedin_page();
            break;
        default:
            break;
        }
    }
    void view_gold_exchange_rate()
    {
        std::cout << "EXCHANGE RATE: " << m_gold_exc_rate << "\n";
        loggedin_page();
    }
};



int main()
{
    Program program = Program();
    program.index_page();
}
