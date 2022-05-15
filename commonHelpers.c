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

#include "commonHelpers.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

int currentYear(void) {
    time_t currentTime = time(NULL);
    return localtime(&currentTime)->tm_year + 1900;
}

void clearStandardInputBuffer(void) {
    while (getchar() != '\n');
}

int getInteger(void) {
    int ans, is_not_done = 1;
    char new_line;
    while (is_not_done) {
        if (scanf("%d%c", &ans, &new_line)) {
            if (new_line == '\n') {
                is_not_done = 0;
            }
            else {
                printf("ERROR: Value must be an integer: ");
                clearStandardInputBuffer();
            }
        }
        else {
            printf("ERROR: Value must be an integer: ");
            clearStandardInputBuffer();
        }
    }
    return ans;
}

int getPositiveInteger(void) {
    int ans, is_not_done = 1;;
    while (is_not_done) {
        ans = getInteger();
        if (ans > 0) {
            is_not_done = 0;
        }
        else {
            printf("ERROR: Value must be a positive integer greater than zero: ");
        }
    }
    return ans;
}

double getDouble(void) {
    double ans;
    char new_line;
    int is_not_done = 1;
    while (is_not_done) {
        if (scanf("%lf%c", &ans, &new_line)) {
            if (new_line == '\n') {
                is_not_done = 0;
            }
            else {
                printf("ERROR: Value must be a double floating-point number: ");
                clearStandardInputBuffer();
            }
        }
        else {
            printf("ERROR: Value must be a double floating-point number: ");
            clearStandardInputBuffer();
        }
    }
    return ans;
}

double getPositiveDouble(void) {
    double ans;
    int is_not_done = 1;
    while (is_not_done) {
        ans = getDouble();
        if (ans > 0) {
            is_not_done = 0;
        }
        else {
            printf("ERROR: Value must be a positive double floating-point number: ");
        }
    }
    return ans;
}

int getIntFromRange(int low, int high) {
    if (low > high) {
        printf("ERROR: low value is greater than high value.\n");
        return -1;
    }
    else {
        int ans, is_not_done = 1;
        while (is_not_done) {
            ans = getInteger();
            if (ans >= low && ans <= high) {
                is_not_done = 0;
            }
            else {
                if (low == high) {
                    printf("ERROR: Value must be exactly %d: ", low);
                }
                else {
                    printf("ERROR: Value must be between %d and %d inclusive: ", low, high);
                }
            }
        }
        return ans;
    }
}

char getCharOption(char valid_chars[]) {
    int running = 1;
    char ans = '\0';
    while (running) {
        char c1[] = { '\0', '\0', '\0' };
        if (scanf("%2s", c1)) {
            char* contained_char = strchr(valid_chars, *c1); // check if c1[0] will work instead of *c1
            if (c1[1] == '\0' && contained_char != NULL) {
                ans = contained_char[0];
                running = 0;
            }
            else {
                printf("ERROR: Character must be one of [%s]: ", valid_chars);
            }
        }
        clearStandardInputBuffer();
    }
    return ans;
}

void getCString(char c_str[], int min, int max) {
    if (min <= max) {
        int done = 0, len = 0, error = 1;
        while (!done) {
            char input_str[256] = { '\0' };
            char end = '\0';
            if (scanf("%255[^\n]s%c", input_str, &end)) {
                len = strlen(input_str);
                if (len >= min && len <= max && input_str[0] != '\0' && end != '\n') {
                    strcpy(c_str, input_str);
                    done = 1;
                    error = 0;
                }
            }
            if (error) {
                displayCStringLengthErrors(len, min, max);
            }
            clearStandardInputBuffer();
        }
    }
}

void getCStringNoWhitespace(char c_str[], int min, int max, const char* prompt, const char* error) {
    if (min <= max) {
        int done = 0;
        char str[256] = { '\0' };
        printf("%s", prompt);
        while (!done) {
            getCString(str, 0, 255);         
            int len = strlen(str);
            if (len >= min && len <= max) {
                char* whitespace = strchr(str, ' ');
                if (whitespace == NULL) {
                    strcpy(c_str, str);
                    done = 1;
                }
                else {
                    printf("%s\n%s", error, prompt);
                }
            }
            else {
                displayCStringLengthErrors(len, min, max);
            }
        }
    }
}

void displayCStringLengthErrors(int size, int min, int max) {
    if (min == max && size != min) {
        printf("ERROR: String length must be exactly %d chars: ", min);
    }
    else if (size > max) {
        printf("ERROR: String length must be no more than %d chars: ", max);
    }
    else {
        printf("ERROR: String length must be between %d and %d chars: ", min, max);
    }
}

// verifies if password is valid based on criteria and returns validity response.
int isValidPassword(char password[]) {
    int digitCount = 0, uppercaseCount = 0, lowercaseCount = 0, symbolCount = 0,
        size = strlen(password), i = 0;
    for (; i < size; i++) {
        if (password[i] >= '0' && password[i] <= '9') {
            digitCount++;
        }
        else if (password[i] >= 'a' && password[i] <= 'z') {
            lowercaseCount++;
        }
        else if (password[i] >= 'A' && password[i] <= 'Z') {
            uppercaseCount++;
        }
        else if (strchr("!@#$%^&*", password[i]) != NULL) {
            symbolCount++;
        }
    }
    return digitCount >= 2 && uppercaseCount >= 2 && lowercaseCount >= 2 && symbolCount >= 2;
}

// converts all lowercase alphabet characters to their uppercase counterparts.
void toUppercase(char c_str[]) {
    int i = 0, size = strlen(c_str);
    for (; i < size; i++) {
        if (c_str[i] >= 'a' && c_str[i] <= 'z') {
            c_str[i] = c_str[i] - 32;
        }
    }
}

// gets valid password by prompting user to enter one, and verifies if it is valid, otherwise tells
// user what requirements are. does this until valid password is acquired.
void getValidPassword(char password[], int min, int max) {
    if (min <= max) {
        int valid_input = 0;
        while (!valid_input) {
            if (min == max)
                printf("Enter the password (must be %d chars in length): ", min);
            else
                printf("Enter the password (must be between %d and %d chars in length): ", min, max);
            getCString(password, min, max);
            if (isValidPassword(password)) {
                valid_input = 1;
            }
            else {
                printf("SECURITY: Password must contain 2 of each:\n");
                printf("          Digit: 0-9\n");
                printf("          UPPERCASE character\n");
                printf("          lowercase character\n");
                // % is format specifier, needs to be escaped by using %% to remove warnings.
                printf("          symbol character: !@#$%%^&*\n");
            }
        }
    }
}

// gets a censored password by replacing every odd character with a *.
void getCensoredPassword(const char password[], char censored_password[]) {
    int i = 0, len = strlen(password);
    for (; i < len; i++) {
        censored_password[i] = ((i + 1) % 2 == 0) ? '*' : password[i];
    }
}

// gets positive integer, including zero.
int getNonNegativeInteger(void) {
    int ans, is_not_done = 1;
    while (is_not_done) {
        ans = getInteger();
        if (ans >= 0) {
            is_not_done = 0;
        }
        else {
            printf("ERROR: Value must be positive or zero: ");
        }
    }
    return ans;
}