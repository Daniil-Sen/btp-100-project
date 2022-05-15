// ===================================================================================
//  Assignment: 2 
//  Milestone : 4
// ===================================================================================
//  Student Name  : Daniil Sen
//  Student ID    : 160970190
//  Student Email : dsen5@myseneca.ca
//  Course Section: NAA
// ===================================================================================

#ifndef _TICKET_H_
#define _TICKET_H_

#include "account.h"

#define TICKET_DISPLAY_NAME_LENGTH 30
#define TICKET_SUBJECT_LINE_LENGTH 30
#define TICKET_MESSAGE_DETAILS_LENGTH 150
#define MESSAGES_SIZE 20

struct Message {
    char account_type;
    char display_name[TICKET_DISPLAY_NAME_LENGTH + 1];
    char details[TICKET_MESSAGE_DETAILS_LENGTH + 1];
};

struct Ticket {
    int id;
    int customer_account_number;
    int is_open;
    char subject_line[TICKET_SUBJECT_LINE_LENGTH + 1];
    int message_counter;
    struct Message messages[MESSAGES_SIZE];
};

void createNewTicket(struct Ticket* tickets, int length, const struct Account* account, int ticket_id);

void createNewMessage(struct Ticket* ticket, const struct Account* account);

struct Ticket* getTicket(int ticket_num, struct Ticket* tickets, int length);

int closeTicketConfirm(struct Ticket* ticket, const struct Account* account);

void reOpenTicket(struct Ticket* ticket);

#endif // !TICKET_H_
