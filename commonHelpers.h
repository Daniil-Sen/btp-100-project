// ===================================================================================
//  Assignment: 2
//  Milestone : 4
// ===================================================================================
//  Student Name  : Daniil Sen
//  Student ID    : 160970190
//  Student Email : dsen5@myseneca.ca
//  Course Section: NAA
// ===================================================================================

#ifndef COMMON_HELPERS_H_
#define COMMON_HELPERS_H_

int currentYear(void);

void clearStandardInputBuffer(void);

int getInteger(void);

int getPositiveInteger(void);

double getDouble(void);

double getPositiveDouble(void);

int getIntFromRange(int low, int high);

char getCharOption(char valid_chars[]);

void getCString(char c_str[], int min, int max);

void getCStringNoWhitespace(char c_str[], int min, int max, const char* prompt, const char* error);

void displayCStringLengthErrors(int size, int min, int max);

int isValidPassword(char password[]);

void toUppercase(char c_str[]);

void getValidPassword(char password[], int min, int max);

void getCensoredPassword(const char password[], char censored_password[]);

int getNonNegativeInteger(void);

#endif