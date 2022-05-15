// ===================================================================================
//  Assignment: 2 
//  Milestone : 4
// ===================================================================================
//  Student Name  : Daniil Sen
//  Student ID    : 160970190
//  Student Email : dsen5@myseneca.ca
//  Course Section: NAA
// ===================================================================================


#include "accountTicketingUI.h"
#include "commonHelpers.h"

#include <string.h>


// ========== APPLICATION START / END ==========


// starts the application.
void applicationStartup(struct AccountTicketingData* ticketing_data) {
    int exit = 0, res = -1;
    while (!exit) {
        res = menuLogin(ticketing_data->accounts, ticketing_data->ACCOUNT_MAX_SIZE);
        if (res == -1) {
            exit = 1;
        }
        else {
            if (ticketing_data->accounts[res].account_type == 'C')
                menuClient(ticketing_data->tickets, ticketing_data->TICKET_MAX_SIZE, &ticketing_data->accounts[res]);
            else if (ticketing_data->accounts[res].account_type == 'A')
                menuAgent(ticketing_data, &ticketing_data->accounts[res]);
        }
    }
    printf("==============================================\n");
    printf("Account Ticketing System - Terminated\n");
    printf("==============================================\n");
}

// confirms application exit by yes/no prompt.
int confirmApplicationExit(void) {
    printf("Are you sure you want to exit? ([Y]es|[N]o): ");
    char r = getCharOption("YyNn");
    printf("\n");
    return (r == 'Y' || r == 'y') ? 1 : 0;
}


// ========== UPDATE INFO ==========


// update account menu, with option to change account type, login, and demographics.
void updateAccount(struct Account* account) {
    int exit = 0, res = 0;

    while (!exit) {
        printf("Account: %d - Update Options\n", account->account_number);
        printf("----------------------------------------\n");
        printf("1) Update account type (current value: %c)\n", account->account_type);
        printf("2) Login\n");
        printf("3) Demographics\n");
        printf("0) Done\n");
        printf("Selection: ");

        res = getIntFromRange(0, 3);
        printf("\n");

        switch (res) {
        case 0:
            exit = 1;
            break;
        case 1: // update account type
            printf("Enter the account type (A=Agent | C=Customer): ");
            account->account_type = getCharOption("AC");
            break;
        case 2: // update login
            updateUserLogin(&account->user_login);
            break;
        case 3: // update demographics
            updateDemographic(&account->demographic);
            break;
        default:
            break;
        }
    }
}

// update user login menu, with option to change display name, and password.
void updateUserLogin(struct UserLogin* login) {
    int exit = 0, res = 0;
    while (!exit) {
        printf("User Login: %s - Update Options\n", login->login_name);
        printf("----------------------------------------\n");
        printf("1) Display name (current value: %s)\n", login->customer_account_holder);
        printf("2) Password\n");
        printf("0) Done\n");
        printf("Selection: ");

        res = getIntFromRange(0, 2);
        printf("\n");

        switch (res) {
        case 0: // exit
            exit = 1;
            break;
        case 1: // update display name
            printf("Enter the display name (30 chars max): ");
            getCString(login->customer_account_holder, 1, 30);
            printf("\n");
            break;
        case 2: // update password
            getValidPassword(login->password, 8, 8);
            printf("\n");
            break;
        default:
            break;
        }
    }
}

// update demographic menu, with option to change household income, and country.
void updateDemographic(struct Demographic* demographic) {
    int exit = 0, res = 0;
    while (!exit) {
        printf("Demographic Update Options\n");
        printf("----------------------------------------\n");
        printf("1) Household Income (current value: $%0.2lf)\n", demographic->household_income);
        printf("2) Country (current value: %s)\n", demographic->country);
        printf("0) Done\n");
        printf("Selection: ");

        res = getIntFromRange(0, 2);
        printf("\n");

        switch (res) {
        case 0: // exit
            exit = 1;
            break;
        case 1: // update household income
            printf("Enter the household Income: $");
            demographic->household_income = getPositiveDouble();
            printf("\n");
            break;
        case 2: // update country
            printf("Enter the country (30 chars max.): ");
            getCString(demographic->country, 1, 30);
            toUppercase(demographic->country);
            printf("\n");
            break;
        default:
            break;
        }
    }
}

// updates account only if it exists by prompting user to enter account number and checking if it exists
// in the ticketing_data struct.
void updateExistingAccount(struct AccountTicketingData* ticketing_data) {
    int exist_idx = findAccountIndexByAcctNum(-1, ticketing_data->accounts, ticketing_data->ACCOUNT_MAX_SIZE, 1);
    printf("\n");
    if (exist_idx != -1)
        updateAccount(&ticketing_data->accounts[exist_idx]);
    else
        printf("ERROR: account does not exist\n\n");
}

// menu ui that agent can use to update a ticket, creating a new message, closing a ticket, or reopening a ticket.
void updateAgentTicket(struct Ticket* ticket, const struct Account* agent_account, struct Account* cust_account) {
    int exit = 0;
    while (!exit) {
        printf("----------------------------------------\n");
        printf("Ticket %6.6d - Update Options\n", ticket->id);
        printf("----------------------------------------\n");
        printf("Status  : %s\n", ticket->is_open ? "ACTIVE" : "CLOSED");
        printf("Subject : %s\n", ticket->subject_line);
        printf("Acct#   : %d\n", ticket->customer_account_number);
        printf("Customer: %s\n", cust_account->user_login.customer_account_holder);
        printf("----------------------------------------\n");
        printf("1) Add a message\n");
        printf("2) Close ticket\n");
        printf("3) Re-open ticket\n");
        printf("0) Done\n");
        printf("Selection: ");

        int res = getIntFromRange(0, 3);
        printf("\n");

        switch (res) {
        case 0: // exit
            exit = 1;
            break;
        case 1: // add message
            createNewMessage(ticket, agent_account);
            printf("\n");
            break;
        case 2: // close ticket
            closeTicketConfirm(ticket, agent_account);
            printf("\n");
            //pauseExecution();
            break;
        case 3: // re-open ticket
            reOpenTicket(ticket);
            break;
        default:
            break;
        }
    }
}


// ========== DISPLAY INFO ==========


// displays a message based on if it its ticket is a new ticket, active ticket, or closed ticket.
// if none specified, then print ticket message.
void printTicketMessages(struct Ticket* ticket, int new_ticket, int active_ticket, int closed_ticket) {
    if (ticket != NULL) {
        if (new_ticket) {
            if (ticket->is_open && ticket->message_counter == 1) {
                displayMessages(ticket);
            }
        }
        else if (active_ticket == 1) {
            if (ticket->is_open == 1) {
                displayMessages(ticket);
            }
        }
        else if (closed_ticket == 1) {
            if (ticket->is_open != 1) {
                displayMessages(ticket);
            }
        }
        else {
            displayMessages(ticket);
        }
    }

}

void displayMessages(struct Ticket* ticket) {
    printf("================================================================================\n");
    printf("%-6.6d (%s) Re: %s\n", ticket->id, ticket->is_open == 1 ? "ACTIVE" : "CLOSED", ticket->subject_line);
    printf("================================================================================\n");
    int i = 0, displayedMessageCount = 0;
    for (; i < ticket->message_counter; i++) {
        if (ticket->messages[i].account_type != '\0') {
            printf("%s (%s):\n", ticket->messages[i].account_type == 'C' ? "CUSTOMER" : "AGENT", ticket->messages[i].display_name);
            printf("   %s\n", ticket->messages[i].details);
            printf("\n");
            // added in milestone 3 
            displayedMessageCount++;
            if (displayedMessageCount % MESSAGE_DISPLAY_BUFFER_SIZE == 0 && displayedMessageCount != 0 && displayedMessageCount != ticket->message_counter) {
                pauseExecution();
            }
        }
    }
}

void displayAllAccountSummaryRecords(const struct Account accounts[], int size) {
    displayAccountSummaryHeader();
    int i;
    for (i = 0; i < size; i++) {
        if (accounts[i].account_number > 0) {
            displayAccountSummaryRecord(&accounts[i]);
        }
    }
}

void displayAllAccountDetailRecords(const struct Account accounts[], int size) {
    displayAccountDetailHeader();
    int i;
    for (i = 0; i < size; i++) {
        if (accounts[i].account_number > 0) {
            displayAccountDetailRecord(&accounts[i]);
        }
    }
}

void displayAccountSummaryHeader(void) {
    printf("Acct# Acct.Type Birth\n");
    printf("----- --------- -----\n");
}

void displayAccountDetailHeader(void) {
    printf("Acct# Acct.Type Birth Income      Country    Disp.Name       Login      Password\n");
    printf("----- --------- ----- ----------- ---------- --------------- ---------- --------\n");
}

void displayAccountSummaryRecord(const struct Account* account) {
    printf("%05d %-9s %5d\n", account->account_number,
        account->account_type == 'C' ? "CUSTOMER" : (account->account_type == 'A' ? "AGENT" : "NA"),
        account->demographic.customer_birth_year);
}

void displayAccountDetailRecord(const struct Account* account) {
    char censored_password[PASSWORD_SIZE] = { '\0' };
    getCensoredPassword(account->user_login.password, censored_password);

    printf("%05d %-9s %5d $%10.2lf %-10s %-15s %-10s %8s\n",
        account->account_number,
        account->account_type == 'C' ? "CUSTOMER" : (account->account_type == 'A' ? "AGENT" : "NA"),
        account->demographic.customer_birth_year,
        account->demographic.household_income,
        account->demographic.country,
        account->user_login.customer_account_holder,
        account->user_login.login_name,
        censored_password);
}

void displayTicketHeaderForCustomer(void) {
    printf("------ ------ ------------------------------ --------\n");
    printf("Ticket Status Subject                        Messages\n");
    printf("------ ------ ------------------------------ --------\n");
}

// displays relevant ticket info to customer, not all the ticket info.
void displayTicketForCustomer(const struct Ticket* ticket) {
    printf("%-6.6d %-6s %-30s %5d\n",
        ticket->id,
        ticket->is_open == 1 ? "ACTIVE" : "CLOSED",
        ticket->subject_line,
        ticket->message_counter);
}

void displayTicketHeaderForAgent(void) {
    printf("------ ----- --------------- ------ ------------------------------ --------\n");
    printf("Ticket Acct# Display Name    Status Subject                        Messages\n");
    printf("------ ----- --------------- ------ ------------------------------ --------\n");
}

// displays ticket info just like displayTicketForCustomer, but also includes account number and name of customer.
void displayTicketForAgent(const struct Ticket* ticket, char login_name[]) {
    printf("%-6.6d %-5.5d %-15s %-6s %-30s %5d\n",
        ticket->id,
        ticket->customer_account_number,
        login_name,
        ticket->is_open == 1 ? "ACTIVE" : "CLOSED",
        ticket->subject_line,
        ticket->message_counter);
}


// ========== SEARCHES ==========
 

// find index of accounts array if num exists. user can also be prompted for num if flagged.
int findAccountIndexByAcctNum(int num, const struct Account accounts[], int size, int prompt) {
    if (prompt != 0) {
        printf("Enter the account#: ");
        num = getPositiveInteger(); // can reuse num as its not used if this line is executed
    }
    int i = 0, idx = -1;
    for (; i < size; i++) {
        if (accounts[i].account_number == num) {
            idx = i;
            break; // no point in continuing as open space is found.
        }
    }
    return idx;
}

// used for generating new account.
int getLargestAvailableAccountNumber(struct AccountTicketingData* ticketing_data) {
    int largest_account_number = 0, i = 0;
    for (i = 0; i < ticketing_data->ACCOUNT_MAX_SIZE; i++) {
        if (ticketing_data->accounts[i].account_number > largest_account_number) {
            largest_account_number = ticketing_data->accounts[i].account_number;
        }
    }
    return largest_account_number;
}

// used for generating new ticket.
int getHighestTicketNumber(struct Ticket* ticket, int length) {
    int i = 0, count = 0;
    for (; i < length; i++) {
        if (ticket[i].id > count) {
            count = ticket[i].id;
        }
    }
    return count;
}


// ========== CUSTOMER FUNCTIONS ==========


// menu for client, can view account details, create new tickets, modofy existing tickets, and list their tickets.
void menuClient(struct Ticket* tickets, int tickets_size, const struct Account* account) {
    int exit = 0, input, highestTicketNumber = getHighestTicketNumber(tickets, tickets_size);

    while (!exit) {
        printf("CUSTOMER: %s (%d)\n", account->user_login.customer_account_holder, account->account_number);
        printf("==============================================\n");
        printf("Customer Main Menu\n");
        printf("==============================================\n");
        printf("1) View your account detail\n");
        printf("2) Create a new ticket\n");
        printf("3) Modify an active ticket\n");
        printf("4) List my tickets\n");
        printf("----------------------------------------------\n");
        printf("0) Logout\n\n");
        printf("Selection: ");

        input = getIntFromRange(0, 4);
        printf("\n");

        switch (input) {
        case 0: // exit
            exit = 1;
            customerLogout(tickets, tickets_size);
            break;
        case 1: // display account details
            displayAccountDetailHeader();
            displayAccountDetailRecord(account);
            printf("\n");
            pauseExecution();
            break;
        case 2:
            createNewTicket(tickets, tickets_size, account, ++highestTicketNumber); // may cause issues. keep in mind.
            printf("\n");
            pauseExecution();
            break;
        case 3:
            modifyActiveTicketCustomer(tickets, tickets_size, account);
            pauseExecution();
            break;
        case 4:
            getRelatedMessagesCustomer(tickets, tickets_size, account, 0, 0, 0);
            break;
        default:
            break;
        }
    }
}

// lists all their tickets based on if the ticket is new, active, or closed.
// then prompts user to open messages related to listed tickets, or exit.
void getRelatedMessagesCustomer(struct Ticket* tickets, int length, const struct Account* account, int new_ticket, int active_ticket, int closed_ticket) {
    int exit = 0;
    while (!exit) {
        listTicketsByCustomer(tickets, length, account, new_ticket, active_ticket, closed_ticket);

        printf("Enter the ticket number to view the messages or\n");
        printf("0 to return to previous menu: ");
        int ticket_num = getNonNegativeInteger();
        printf("\n");
        if (ticket_num == 0) {
            exit = 1;
        }
        else {
            struct Ticket* ticket = getTicket(ticket_num, tickets, length);
            if (ticket != NULL) {
                if (ticket->customer_account_number == account->account_number) {
                    printTicketMessages(ticket, new_ticket, active_ticket, closed_ticket);
                }
                else {
                    printf("ERROR: Invalid ticket number - you may only access your own tickets.\n");
                    printf("\n");
                }
            }
            else {
                printf("ERROR: Invalid ticket number.\n");
                printf("\n");
            }
            pauseExecution();
        }
    }
}

// checks if the ticket exists, is theirs, and is modifiable. then user can modify it.
void modifyActiveTicketCustomer(struct Ticket* tickets, int tickets_length, const struct Account* account) {
    int error = 1;

    printf("Enter ticket number: ");
    int ticket_num = getPositiveInteger();
    printf("\n");
    struct Ticket* ticket = getTicket(ticket_num, tickets, tickets_length);

    if (ticket != NULL) {
        if (account->account_type == 'C' && ticket->customer_account_number == account->account_number && ticket->is_open == 1) {
            updateCustomerTicket(ticket, account);
            error = 0;
        }
    }

    if (error) {
        if (ticket != NULL && ticket->is_open == 0) {
            printf("ERROR: Ticket is closed - changes are not permitted.\n");
        }
        else {
            printf("ERROR: Invalid ticket number - you may only modify your own ticket.\n");
        }
        printf("\n");
    }
}

// modify ticket menu ui. customer can modify the subject, add a new message, and close the ticket.
void updateCustomerTicket(struct Ticket* ticket, const struct Account* account) {
    int exit = 0;

    while (!exit) {
        printf("----------------------------------------\n");
        printf("Ticket %6.6d - Update Options\n", ticket->id);
        printf("----------------------------------------\n");
        printf("Status  : %s\n", ticket->is_open ? "ACTIVE" : "CLOSED");
        printf("Subject : %s\n", ticket->subject_line);
        printf("----------------------------------------\n");
        printf("1) Modify the subject\n");
        printf("2) Add a message\n");
        printf("3) Close ticket\n");
        printf("0) Done\n");
        printf("Selection: ");

        int res = getIntFromRange(0, 3);
        printf("\n");

        switch (res) {
        case 0: // exit
            exit = 1;
            break;
        case 1: // modify ticket
            printf("Enter the revised ticket SUBJECT (30 chars. maximum): ");
            getCString(ticket->subject_line, 1, TICKET_MESSAGE_DETAILS_LENGTH);
            printf("\n");
            break;
        case 2: // add message
            createNewMessage(ticket, account);
            printf("\n");
            break;
        case 3: // close ticket
            exit = closeTicketConfirm(ticket, account);
            printf("\n");
            // pauseExecution();
            break;
        default:
            break;
        }
    }
}

// lists tickets for a customer based on if the ticket is new, active, or closed.
void listTicketsByCustomer(const struct Ticket* tickets, int tickets_size, const struct Account* account, int new_ticket, int active_ticket, int closed_ticket) {
    int i = 0;
    displayTicketHeaderForCustomer();
    for (; i < tickets_size; i++) {
        if (tickets[i].customer_account_number == account->account_number) {
            if (new_ticket == 1) {
                if (tickets[i].is_open && tickets[i].message_counter == 1) {
                    displayTicketForCustomer(&tickets[i]);
                }
            }
            else if (active_ticket == 1) {
                if (tickets[i].is_open == 1) {
                    displayTicketForCustomer(&tickets[i]);
                }
            }
            else if (closed_ticket == 1) {
                if (tickets[i].is_open == 0) {
                    displayTicketForCustomer(&tickets[i]);
                }
            }
            else {
                displayTicketForCustomer(&tickets[i]);
            }
        }
    }
    printf("------ ------ ------------------------------ --------\n");
    printf("\n");
}


// ========== AGENT FUNCTIONS ==========


// agent menu ui. they can add a new account, modify existing one, remove an account, list the summary and detail of accounts,
// list new, active or closed tickets. can manage tickets, archive closed tickets, and view statistics for archived tickets and accounts.
void menuAgent(struct AccountTicketingData* ticketing_data, const struct Account* account) {
    int exit = 0, res = -1, largest_account_number = getLargestAvailableAccountNumber(ticketing_data);

    while (!exit) {
        printf("AGENT: %s (%d)\n", account->user_login.customer_account_holder, account->account_number);
        printf("==============================================\n");
        printf("Account Ticketing System - Agent Menu\n");
        printf("==============================================\n");
        printf(" 1) Add a new account\n");
        printf(" 2) Modify an existing account\n");
        printf(" 3) Remove an account\n");
        printf(" 4) List accounts: summary view\n");
        printf(" 5) List accounts: detailed view\n");
        printf("----------------------------------------------\n");
        printf(" 6) List new tickets\n");
        printf(" 7) List active tickets\n");
        printf(" 8) List closed tickets\n");
        printf(" 9) Manage a ticket\n");
        printf("10) Archive closed tickets\n");
        printf("----------------------------------------------\n");
        printf("11) View archived account statistics\n");
        printf("12) View archived tickets statistics\n");
        printf("----------------------------------------------\n");
        printf("0) Logout\n\n");
        printf("Selection: ");

        res = getIntFromRange(0, 12);
        printf("\n");

        switch (res) {
        case 0: // logout
            exit = 1;
            agentLogout(ticketing_data);
            break;
        case 1: // add new account if there is available space.
            addNewAccount(ticketing_data, &largest_account_number);
            pauseExecution();
            break;
        case 2: // update existing account
            updateExistingAccount(ticketing_data);
            break;
        case 3: // remove existing account only if its not currently used.
            removeExistingAccount(ticketing_data, account);
            pauseExecution();
            break;
        case 4: // display account summary
            displayAllAccountSummaryRecords(ticketing_data->accounts, ticketing_data->ACCOUNT_MAX_SIZE);
            printf("\n");
            pauseExecution();
            break;
        case 5: // display account details
            displayAllAccountDetailRecords(ticketing_data->accounts, ticketing_data->ACCOUNT_MAX_SIZE);
            printf("\n");
            pauseExecution();
            break;
        case 6: // list new tickets
            getRelatedMessagesAgent(ticketing_data, 1, 0, 0);
            break;
        case 7: // list active tickets
            getRelatedMessagesAgent(ticketing_data, 0, 1, 0);
            break;
        case 8: // list closed tickets
            getRelatedMessagesAgent(ticketing_data, 0, 0, 1);
            //pauseExecution();
            break;
        case 9: // agent can modify tickets here.
            modifyActiveTicketAgent(ticketing_data, account);
            break;
        case 10:
            archiveAllClosedTickets(ticketing_data);
            pauseExecution();
            break;
        case 11:
            printf("There are %d account(s) currently archived.\n\n", getLineCount(ACCOUNTS_ARCHIVE_PATH));
            pauseExecution();
            break;
        case 12:
        {   // This is a workaround to getting variables declared in a switch statement by using a code block.
            int ticket_count = 0, message_count = 0;
            getTicketAndMessageCount(TICKETS_ARCHIVE_PATH, '|', &ticket_count, &message_count);
            printf("There are %d ticket(s) and a total of %d message(s) archived.\n\n", ticket_count, message_count);
        }
            pauseExecution();
            break;
        default:
            break;
        }
    }
}

// adds new account if there is room.
void addNewAccount(struct AccountTicketingData* ticketing_data, int* largest_account_number) {
    int avail_idx = findAccountIndexByAcctNum(0, ticketing_data->accounts, ticketing_data->ACCOUNT_MAX_SIZE, 0);
    if (avail_idx != ticketing_data->ACCOUNT_MAX_SIZE && avail_idx != -1) {
        ticketing_data->accounts[avail_idx].account_number = ++ * largest_account_number;
        getAccount(&ticketing_data->accounts[avail_idx]);
        getUserLogin(&ticketing_data->accounts[avail_idx].user_login);
        getDemographic(&ticketing_data->accounts[avail_idx].demographic);
        printf("*** New account added! ***\n");
    }
    else {
        printf("ERROR: Account listing is FULL, call ITS Support!\n");
    }
    printf("\n");
}

// remove existing account, archiving it and its closed tickets.
void removeExistingAccount(struct AccountTicketingData* ticketing_data, const struct Account* account) {
    int exist_idx = findAccountIndexByAcctNum(-1, ticketing_data->accounts, ticketing_data->ACCOUNT_MAX_SIZE, 1);
    if (exist_idx != -1) {
        if (ticketing_data->accounts[exist_idx].account_number != account->account_number) {
            displayAccountDetailHeader();
            displayAccountDetailRecord(&ticketing_data->accounts[exist_idx]);
            printf("\nAre you sure you want to remove this record? ([Y]es|[N]o): ");
            if (getCharOption("YN") == 'Y') {
                archiveRemovedAccount(&ticketing_data->accounts[exist_idx]);
                archiveClosedTicketsByAccount(ticketing_data->accounts[exist_idx].account_number, ticketing_data);
                removeTicketsByAccount(ticketing_data->accounts[exist_idx].account_number, ticketing_data);
                ticketing_data->accounts[exist_idx].account_number = 0;
                printf("\n*** Account Removed! ***\n\n");
            }
            else {
                printf("\n*** No changes made! ***\n\n");
            }
        }
        else {
            printf("\nERROR: You can't remove your own account!\n\n");
        }
    }
    else {
        printf("\nERROR: No Account exists\n\n");
    }
}

// lists all their tickets based on if the ticket is new, active, or closed.
// then prompts user to open messages related to listed tickets, or exit.
void getRelatedMessagesAgent(struct AccountTicketingData* ticketing_data, int new_ticket, int active_ticket, int closed_ticket) {
    int exit = 0;
    while (!exit) {
        listTicketsForAgent(ticketing_data, new_ticket, active_ticket, closed_ticket);

        printf("Enter the ticket number to view the messages or\n");
        printf("0 to return to previous menu: ");
        int ticket_num = getNonNegativeInteger();
        printf("\n");
        if (ticket_num == 0) {
            exit = 1;
        }
        else {
            struct Ticket* ticket = getTicket(ticket_num, ticketing_data->tickets, ticketing_data->TICKET_MAX_SIZE);
            if (ticket != NULL) {
                printTicketMessages(ticket, new_ticket, active_ticket, closed_ticket);
            }
            else {
                printf("ERROR: Invalid ticket number.\n");
                printf("\n");
            }
            pauseExecution();
        }
    }
}

// checks if the ticket exists, is theirs, and is modifiable. then agent can modify it.
void modifyActiveTicketAgent(struct AccountTicketingData* ticketing_data, const struct Account* account) {
    printf("Enter ticket number: ");
    int ticket_num = getPositiveInteger();
    printf("\n");
    struct Ticket* ticket = getTicket(ticket_num, ticketing_data->tickets, ticketing_data->TICKET_MAX_SIZE);

    if (ticket != NULL) {
        int cust_index = findAccountIndexByAcctNum(ticket->customer_account_number, ticketing_data->accounts, ticketing_data->ACCOUNT_MAX_SIZE, 0);

        if (account->account_type == 'A' && cust_index != -1) {
            updateAgentTicket(ticket, account, &ticketing_data->accounts[cust_index]);
        }
    }
}

// removes the tickets from the tickets array, creating room. can either remove tickets from all accounts or by account number.
void removeTicketsByAccount(int account_number, struct AccountTicketingData* ticketing_data) {
    int i = 0;
    for (; i < ticketing_data->TICKET_MAX_SIZE; i++) {
        if (ticketing_data->tickets[i].customer_account_number == account_number || account_number == ALL_ACCOUNTS) {
            ticketing_data->tickets[i].customer_account_number = 0;
            ticketing_data->tickets[i].id = 0;
            ticketing_data->tickets[i].is_open = 0;
        }
    }
}

// lists tickets for all customers based on if the ticket is new, active, or closed.
void listTicketsForAgent(struct AccountTicketingData* ticketing_data, int new_ticket, int active_ticket, int closed_ticket) {
    int i = 0;
    displayTicketHeaderForAgent();
    for (; i < ticketing_data->TICKET_MAX_SIZE; i++) {
        if (ticketing_data->tickets[i].customer_account_number != 0 && ticketing_data->tickets[i].id != 0) {
            if (new_ticket == 1) {
                if (ticketing_data->tickets[i].is_open && ticketing_data->tickets[i].message_counter == 1) {
                    displayTicketForAgent(&ticketing_data->tickets[i], ticketing_data->tickets[i].messages[0].display_name);
                }
            }
            else if (active_ticket == 1) {
                if (ticketing_data->tickets[i].is_open == 1) {
                    displayTicketForAgent(&ticketing_data->tickets[i], ticketing_data->tickets[i].messages[0].display_name);
                }
            }
            else if (closed_ticket == 1) {
                if (ticketing_data->tickets[i].is_open == 0) {
                    displayTicketForAgent(&ticketing_data->tickets[i], ticketing_data->tickets[i].messages[0].display_name);
                }
            }
            else {
                displayTicketForAgent(&ticketing_data->tickets[i], ticketing_data->tickets[i].messages[0].display_name);
            }
        }
    }
    printf("------ ----- --------------- ------ ------------------------------ --------\n");
    printf("\n");
}


// ========== ACCOUNT LOGIN ==========


// login menu ui, can log in to an existing account or exit the application.
int menuLogin(const struct Account accounts[], int size) {
    int exit = 0, res, ans = -1;

    while (!exit) {
        printf("==============================================\n");
        printf("Account Ticketing System - Login\n");
        printf("==============================================\n");
        printf("1) Login to the system\n");
        printf("0) Exit application\n");
        printf("----------------------------------------------\n\n");
        printf("Selection: ");

        res = getIntFromRange(0, 1);
        printf("\n");

        switch (res) {
        case 0: // exit
            exit = confirmApplicationExit();
            break;
        case 1: // attempt to log in
            ans = attemptLogin(accounts, size);
            exit = ans != -1 ? 1 : 0;
            break;
        default:
            break;
        }
    }
    return ans;
}

// attempts to login to an account based on if account exists through the account number, and it's associated user name and password.
int attemptLogin(const struct Account accounts[], int size) {
    int exit = 0, attempts = 3, ans = -1;
    while (!exit && attempts-- > 0) {
        printf("Enter the account#: ");
        int account_number = getPositiveInteger();
        printf("User Login    : ");
        char user_login_name[256] = { '\0' };
        getCString(user_login_name, 0, 255);
        printf("Password      : ");
        char password[256] = { '\0' };
        getCString(password, 0, 255);

        int index = findAccountIndexByAcctNum(account_number, accounts, size, 0);
        if (index >= 0) {
            struct Account account = accounts[index];
            if (strcmp(user_login_name, account.user_login.login_name) == 0 && strcmp(password, account.user_login.password) == 0) {
                ans = index;
                exit = 1;
            }
        }
        if (ans == -1) {
            printf("INVALID user login/password combination! [attempts remaining:%d]\n", attempts);
            printf("\n");
        }
    }
    if (ans == -1) {
        printf("ERROR:  Login failed!\n");
        printf("\n");
        pauseExecution();
    }
    else {
        printf("\n");
    }
    return ans;
}

// logs agent out of account, saving all accounts, tickets, and messages.
void agentLogout(struct AccountTicketingData* ticketing_data) {
    int account_saved = saveAllAccounts(ticketing_data),
        tickets_saved = saveAllTickets(ticketing_data->tickets, ticketing_data->TICKET_MAX_SIZE);
    printf("Saving session modifications...\n");
    printf("   %d account saved.\n", account_saved);
    printf("   %d tickets saved.\n", tickets_saved);
    printf("### LOGGED OUT ###\n\n");
}

// logs customer out of account, saving all tickets and messages.
void customerLogout(struct Ticket* tickets, int tickets_length) {
    int tickets_saved = saveAllTickets(tickets, tickets_length);
    printf("Saving session modifications...\n");
    printf("   %d tickets saved.\n", tickets_saved);
    printf("### LOGGED OUT ###\n\n");
}


// ========== LOADING FROM FILE ==========


// loads account to account struct using format and position of account file.
int loadAccount(FILE* accounts_file, struct Account* account) {
    return fscanf(accounts_file, "%d~%c~%[^\n~]~%[^\n~]~%[^\n~]~%d~%lf~%[^\n~]",
        &account->account_number,
        &account->account_type,
        account->user_login.customer_account_holder,
        account->user_login.login_name,
        account->user_login.password,
        &account->demographic.customer_birth_year,
        &account->demographic.household_income,
        account->demographic.country);
}

// loads ticket to ticket struct using format and position of ticket file.
// loads all messages related to ticket as well.
int loadTicket(FILE* tickets_file, struct Ticket* ticket) {
    return fscanf(tickets_file, "%d|%d|%d|%[^\n|]|%d|",
        &ticket->id,
        &ticket->customer_account_number,
        &ticket->is_open,
        ticket->subject_line,
        &ticket->message_counter);
}

// loads the message of a ticket to a message struct.
int loadMessage(FILE* tickets_file, struct Message* message) {
    return fscanf(tickets_file, "%c|%[^\n|]|%[^\n|]|",
        &message->account_type,
        message->display_name,
        message->details);
}

// loads all accounts from account_file to account array.
int loadAccounts(struct Account* accounts, int accounts_length) {
    FILE* accounts_file = NULL;
    int length = 0, loaded_accounts = 0;

    if ((accounts_file = fopen(ACCOUNTS_PATH, "r")) != NULL) {
        while (length != accounts_length && loadAccount(accounts_file, &accounts[loaded_accounts])) {
            if (accounts[loaded_accounts].account_number != 0) {
                loaded_accounts++;
            }
            length++;
        }
        fclose(accounts_file);
    }
    else {
        printf("ERROR: %s does not exist.\n", ACCOUNTS_PATH);
    }
    return loaded_accounts;
}

// loads all tickets and their messages from ticket_file to ticket array.
int loadTickets(struct Ticket* tickets, int tickets_length) {
    FILE* tickets_file = NULL;
    int length = 0, counter = 0, loaded_tickets = 0;

    if ((tickets_file = fopen(TICKETS_PATH, "r")) != NULL) {
        while (length != tickets_length && loadTicket(tickets_file, &tickets[loaded_tickets])) {
            if (tickets[length].id != 0) {
                counter = 0;
                while (counter != tickets[loaded_tickets].message_counter && loadMessage(tickets_file, &tickets[loaded_tickets].messages[counter])) {
                    counter++;
                }
                loaded_tickets++;
            }
            length++;
        }
        fclose(tickets_file);
    }
    else {
        printf("ERROR: %s does not exist.\n", ACCOUNTS_PATH);
    }
    return loaded_tickets;
}


// ========== SAVING TO FILE ==========


// saves account data from account struct to account file.
int saveAccount(FILE* accounts_file, const struct Account* account) {
    return fprintf(accounts_file, "%d~%c~%s~%s~%s~%d~%0.2lf~%s\n",
        account->account_number,
        account->account_type,
        account->user_login.customer_account_holder,
        account->user_login.login_name,
        account->user_login.password,
        account->demographic.customer_birth_year,
        account->demographic.household_income,
        account->demographic.country);
}

// saves ticket data from ticket struct to tickets file.
int saveTicket(FILE* tickets_file, struct Ticket* ticket) {
    return fprintf(tickets_file, "%d|%d|%d|%s|%d|",
        ticket->id,
        ticket->customer_account_number,
        ticket->is_open,
        ticket->subject_line,
        ticket->message_counter);
}

// saves message data from message struct to tickets file inline with their ticket.
int saveMessage(FILE* tickets_file, struct Message* message) {
    return fprintf(tickets_file, "%c|%s|%s|",
        message->account_type,
        message->display_name,
        message->details);
}

// saves all tickets from tickets array to ticket file.
int saveAllTickets(struct Ticket* tickets, int tickets_length) {
    FILE* tickets_file = NULL;
    int ticket_count = 0, message_count = 0, saved_tickets = 0;

    if ((tickets_file = fopen(TICKETS_PATH, "w+")) != NULL) {
        for (ticket_count = 0; ticket_count < tickets_length; ticket_count++) {
            if (tickets[ticket_count].id != 0 && tickets[ticket_count].customer_account_number != 0) {
                if (saveTicket(tickets_file, &tickets[ticket_count])) {
                    for (message_count = 0; message_count < tickets[ticket_count].message_counter; message_count++) {
                        if (saveMessage(tickets_file, &tickets[ticket_count].messages[message_count])) {
                        }
                    }
                    saved_tickets++;
                }
                fputc('\n', tickets_file);
            }
        }
        fclose(tickets_file);
    }
    return saved_tickets;
}

// saves all accounts from accounts array to account file.
int saveAllAccounts(struct AccountTicketingData* ticketing_data) {
    FILE* accounts_file = NULL;
    int i = 0, saved_accounts = 0;

    if ((accounts_file = fopen(ACCOUNTS_PATH, "w+")) != NULL) {
        for (; i < ticketing_data->ACCOUNT_MAX_SIZE; i++) {
            if (ticketing_data->accounts[i].account_number != 0) {
                if (saveAccount(accounts_file, &ticketing_data->accounts[i])) {
                    saved_accounts++;
                }
            }
        }
        fclose(accounts_file);
    }
    return saved_accounts;
}


// ========== ARCHIVING DATA ==========


// saves all closed tickets based on account number to tickets archive file. removes them from tickets array.
int archiveClosedTicketsByAccount(int account_number, struct AccountTicketingData* ticketing_data) {
    FILE* tickets_file = NULL;
    int ticketCount = 0, messageCount = 0, archivedTicketCount = 0;

    if ((tickets_file = fopen(TICKETS_ARCHIVE_PATH, "a")) != NULL) {
        for (ticketCount = 0; ticketCount < ticketing_data->TICKET_MAX_SIZE; ticketCount++) {
            if (ticketing_data->tickets[ticketCount].customer_account_number != 0) {
                if ((ticketing_data->tickets[ticketCount].customer_account_number == account_number || account_number == ALL_ACCOUNTS) &&
                    ticketing_data->tickets[ticketCount].is_open == 0 && ticketing_data->tickets[ticketCount].id != 0) {
                    if (saveTicket(tickets_file, &ticketing_data->tickets[ticketCount])) {
                        for (messageCount = 0; messageCount < ticketing_data->tickets[ticketCount].message_counter; messageCount++) {
                            if (saveMessage(tickets_file, &ticketing_data->tickets[ticketCount].messages[messageCount])) {
                            }
                        }
                        archivedTicketCount++;
                        ticketing_data->tickets[ticketCount].id = 0;
                        ticketing_data->tickets[ticketCount].customer_account_number = 0;
                    }
                    fputc('\n', tickets_file);
                }
            }
        }
        fclose(tickets_file);
    }
    return archivedTicketCount;
}

// saves all closed tickets to ticket archive. 
void archiveAllClosedTickets(struct AccountTicketingData* ticketing_data) {
    printf("Are you sure? This action cannot be reversed. ([Y]es|[N]o): ");
    char res = getCharOption("YN");
    if (res == 'Y') {
        int archiveCount = archiveClosedTicketsByAccount(ALL_ACCOUNTS, ticketing_data);
        printf("\n*** %d tickets archived ***\n\n", archiveCount);
    }
    else {
        printf("*** Closed tickets not archived ***\n\n");
    }
}

// saves account to account archive file.
void archiveRemovedAccount(const struct Account* account) {
    FILE* archived_accounts_file = NULL;

    if ((archived_accounts_file = fopen(ACCOUNTS_ARCHIVE_PATH, "a")) != NULL) {
        if (saveAccount(archived_accounts_file, account)) {
        }
        fclose(archived_accounts_file);
    }
    else {
        printf("ERROR: %s does not exist.\n", ACCOUNTS_ARCHIVE_PATH);
    }
}


// ========== OTHER ==========


// gets line count by checking all chars of a file and returns occurences count of new line '\n'.
int getLineCount(const char* file_path) {
    FILE* file = NULL;
    int line_count = 0;
    if ((file = fopen(file_path, "r")) != NULL) {
        char c = '\0';
        while ((c = getc(file)) != EOF) {
            if (c == '\n') {
                line_count++;
            }
        }
        fclose(file);
    }
    return line_count;
}

// gets ticket count and total message count. gets ticket by counting how many delimiters there are.
// after the 4th delimiter, file scans for number of messages next to 4th delimiter and adds to total message count.
// then gets all characters until new line is hit. repeats process until file is entirely read.
// tickets count and message count is return via a out/ref parameter, by using a pointer. this way we can 'return'
// two values. I would have used a pair/tuple/custom data type normally, but I didn't want to use any extra libraries
// or struct definitions.
void getTicketAndMessageCount(const char* file_path, char delimiter, int* tickets_count, int* message_count) {
    FILE* file = NULL;
    int tc = 0, mc = 0, tdelim = 0, mcl = 0;

    if ((file = fopen(file_path, "r")) != NULL) {
        char c = '\0';
        while ((c = getc(file)) != EOF) {
            if (c == delimiter) {
                tdelim++;
                if (tdelim == 4) {
                    tc++;
                    tdelim = 0;
                    if (fscanf(file, "%d|", &mcl)) {
                        mc += mcl;
                        mcl = 0;
                        while (c != '\n' && c != EOF) {
                            c = getc(file);
                        }
                    }
                }
            }
        }
        fclose(file);
    }
    *tickets_count = tc;
    *message_count = mc;
}

void pauseExecution(void) {
    printf("<< ENTER key to Continue... >>");
    clearStandardInputBuffer();
    putchar('\n');
}