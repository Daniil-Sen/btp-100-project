// ===================================================================================
//  Assignment: 2 
//  Milestone : 4
// ===================================================================================
//  Student Name  : Daniil Sen
//  Student ID    : 160970190
//  Student Email : dsen5@myseneca.ca
//  Course Section: NAA
// ===================================================================================

#ifndef ACCOUNT_TICKETING_H_
#define ACCOUNT_TICKETING_H_

// ========== PREPROCESSOR STUFF ==========

// This is a conditional directive determining if the program is running in test mode,
// or running on windows or unix based os. I have different project directory structures
// depending on the os. It's easier to have this here rather than messing up what files
// are located where, as well as the altering the original vs copies.

//#define TEST

#ifdef TEST

#define ACCOUNTS_PATH "accounts_test.txt"
#define TICKETS_PATH "tickets_test.txt"
#define ACCOUNTS_ARCHIVE_PATH "accounts_arc_test.txt"
#define TICKETS_ARCHIVE_PATH "tickets_arc_test.txt"

#else

#ifdef _WIN32

#define ACCOUNTS_PATH "../Resources/accounts.txt"
#define TICKETS_PATH "../Resources/tickets.txt"
#define ACCOUNTS_ARCHIVE_PATH "../Resources/accounts_arc.txt"
#define TICKETS_ARCHIVE_PATH "../Resources/tickets_arc.txt"

#elif __unix__

#define ACCOUNTS_PATH "accounts.txt"
#define TICKETS_PATH "tickets.txt"
#define ACCOUNTS_ARCHIVE_PATH "accounts_arc.txt"
#define TICKETS_ARCHIVE_PATH "tickets_arc.txt"

#endif

#endif

#define _CRT_SECURE_NO_WARNINGS

#define ALL_ACCOUNTS -1
#define MESSAGE_DISPLAY_BUFFER_SIZE 5

// These headers are required in this header file. They are not included again in the source files
// because they are already here and will inherit these headers. Any includes in the source files 
// are are only neccessary there.

#include <stdio.h>

#include "account.h"
#include "ticket.h"


struct AccountTicketingData {
  struct Account* accounts;
  const int ACCOUNT_MAX_SIZE;

  struct Ticket* tickets;
  const int TICKET_MAX_SIZE;
};

// All the comments for the functions are found in the source file. Having the comments there 
// makes more sense to me as I can see the description of the purpose of the function while looking 
// at the source code. An IDE would show the comments from the prototypes while hovering over the 
// definition in the source file. Text editors don't do that, unless there is a vim plug in I 
// haven't heard of for it.

// ========== APPLICATION START / END ==========

void applicationStartup(struct AccountTicketingData* ticketing_data);

int confirmApplicationExit(void);

// ========== UPDATE INFO ==========

void updateAccount(struct Account* account);

void updateUserLogin(struct UserLogin* login);

void updateDemographic(struct Demographic* demographic);

void updateExistingAccount(struct AccountTicketingData* ticketing_data);

void updateAgentTicket(struct Ticket* ticket, const struct Account* agent_account, struct Account* cust_account);

// ========== DISPLAY INFO ==========

void printTicketMessages(struct Ticket* ticket, int new_ticket, int active_ticket, int closed_ticket);

void displayMessages(struct Ticket* ticket);

void displayAllAccountSummaryRecords(const struct Account accounts[], int size);

void displayAllAccountDetailRecords(const struct Account accounts[], int size);

void displayAccountSummaryHeader(void);

void displayAccountDetailHeader(void);

void displayAccountSummaryRecord(const struct Account* account);

void displayAccountDetailRecord(const struct Account* account);

void displayTicketHeaderForCustomer(void);

void displayTicketForCustomer(const struct Ticket* ticket);

void displayTicketHeaderForAgent(void);

void displayTicketForAgent(const struct Ticket* ticket, char login_name[]);

// ========== SEARCHES ==========

int findAccountIndexByAcctNum(int num, const struct Account accounts[], int size, int prompt);

int getLargestAvailableAccountNumber(struct AccountTicketingData* ticketing_data);

int getHighestTicketNumber(struct Ticket* ticket, int length);

// ========== CUSTOMER FUNCTIONS ==========

void menuClient(struct Ticket* tickets, int tickets_size, const struct Account* account);

void getRelatedMessagesCustomer(struct Ticket* tickets, int length, const struct Account* account, int new_ticket, int active_ticket, int closed_ticket);

void modifyActiveTicketCustomer(struct Ticket* tickets, int tickets_length, const struct Account* account);

void updateCustomerTicket(struct Ticket* ticket, const struct Account* account);

void listTicketsByCustomer(const struct Ticket* tickets, int tickets_size, const struct Account* account, int new_ticket, int active_ticket, int closed_ticket);

// ========== AGENT FUNCTIONS ==========

void menuAgent(struct AccountTicketingData* ticketing_data, const struct Account* account);

void addNewAccount(struct AccountTicketingData* ticketing_data, int* largest_account_number);

void removeExistingAccount(struct AccountTicketingData* ticketing_data, const struct Account* account);

void getRelatedMessagesAgent(struct AccountTicketingData* ticketing_data, int new_ticket, int active_ticket, int closed_ticket);

void modifyActiveTicketAgent(struct AccountTicketingData* ticketing_data, const struct Account* account);

void removeTicketsByAccount(int account_number, struct AccountTicketingData* ticketing_data);

void listTicketsForAgent(struct AccountTicketingData* ticketing_data, int new_ticket, int active_ticket, int closed_ticket);

// ========== ACCOUNT LOGIN ==========

int menuLogin(const struct Account accounts[], int size);

int attemptLogin(const struct Account accounts[], int size);

void agentLogout(struct AccountTicketingData* ticketing_data);

void customerLogout(struct Ticket* tickets, int tickets_length);

// ========== LOADING FROM FILE ==========

int loadAccount(FILE* accounts_file, struct Account* account);

int loadTicket(FILE* tickets_file, struct Ticket* ticket);

int loadMessage(FILE* tickets_file, struct Message* message);

int loadAccounts(struct Account* accounts, int accounts_length);

int loadTickets(struct Ticket* tickets, int tickets_length);

// ========== SAVING TO FILE ==========

int saveAccount(FILE* accounts_file, const struct Account* account);

int saveTicket(FILE* tickets_file, struct Ticket* ticket);

int saveMessage(FILE* tickets_file, struct Message* message);

int saveAllTickets(struct Ticket* tickets, int tickets_length);

int saveAllAccounts(struct AccountTicketingData* ticketing_data);

// ========== ARCHIVING DATA ==========

int archiveClosedTicketsByAccount(int account_number, struct AccountTicketingData* ticketing_data);

void archiveAllClosedTickets(struct AccountTicketingData* ticketing_data);

void archiveRemovedAccount(const struct Account* account);

// ========== OTHER ==========

int getLineCount(const char* file_path);

void getTicketAndMessageCount(const char* file_path, char delimiter, int* tickets_count, int* message_count);

void pauseExecution(void);

#endif