// ===================================================================================
//  Assignment: 2 
//  Milestone : 4
// ===================================================================================
//  Student Name  : Daniil Sen
//  Student ID    : 160970190
//  Student Email : dsen5@myseneca.ca
//  Course Section: NAA
// ===================================================================================

#define _CRT_SECURE_NO_WARNINGS

#include "account.h"
#include "commonHelpers.h"

#include <stdio.h>

void getAccount(struct Account* account) {
    printf("New Account Data (Account#:%d)\n", account->account_number);
    printf("----------------------------------------\n");
    printf("Enter the account type (A=Agent | C=Customer): ");
    account->account_type = getCharOption("AC");
    printf("\n");
}

void getUserLogin(struct UserLogin* user_login) {
    printf("User Login Data Input\n");
    printf("----------------------------------------\n");
    getCStringNoWhitespace(user_login->login_name, 1, 10, "Enter user login (10 chars max): ", 
        "ERROR:  The user login must NOT contain whitespace characters.");
    printf("Enter the display name (30 chars max): ");
    getCString(user_login->customer_account_holder, 1, 30);
    getValidPassword(user_login->password, 8, 8);
    printf("\n");
}

void getDemographic(struct Demographic* demographic) {
    printf("Demographic Data Input\n");
    printf("----------------------------------------\n");
    printf("Enter birth year (current age must be between 18 and 110): ");
    demographic->customer_birth_year = getIntFromRange(currentYear() - 110, currentYear() - 18);
    printf("Enter the household Income: $");
    demographic->household_income = getPositiveDouble();
    printf("Enter the country (30 chars max.): ");
    getCString(demographic->country, 1, 30);
    toUppercase(demographic->country);
    printf("\n");
}