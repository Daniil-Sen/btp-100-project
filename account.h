// ===================================================================================
//  Assignment: 2 
//  Milestone : 4
// ===================================================================================
//  Student Name  : Daniil Sen
//  Student ID    : 160970190
//  Student Email : dsen5@myseneca.ca
//  Course Section: NAA
// ===================================================================================

#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_

#define COUNTRY_SIZE 31
#define PASSWORD_SIZE 9
#define LOGIN_NAME_SIZE 11
#define ACCOUNT_HOLDER_SIZE 31

struct Demographic {
    int customer_birth_year;
    double household_income;
    char country[COUNTRY_SIZE];
};

struct UserLogin {
    char customer_account_holder[ACCOUNT_HOLDER_SIZE];
    char login_name[LOGIN_NAME_SIZE];
    char password[ACCOUNT_HOLDER_SIZE];
};

struct Account {
    int account_number;
    char account_type;
    struct UserLogin user_login;
    struct Demographic demographic;
};

void getAccount(struct Account* account);

void getUserLogin(struct UserLogin* user_login);

void getDemographic(struct Demographic* demographic);

void pauseExecution(void);

#endif // !_ACCOUNT_H_
