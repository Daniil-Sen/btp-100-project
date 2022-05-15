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

#include "ticket.h"

#include <stdio.h>
#include <string.h>

#include "account.h"
#include "commonHelpers.h"

// customer creates new ticket if there is room.
void createNewTicket(struct Ticket* tickets, int length, const struct Account* account, int ticket_id) {    
    // gets the next available space for a ticket.
    int i = 0, idx = -1;
    for (; i < length; i++) {
        if (tickets[i].id == 0) {
            idx = i;
            break;
        }
    }
    // checks if there is available space for a new ticket.
    if (idx == -1) {
        printf("ERROR: Ticket listing is FULL, call ITS Support!\n");
    }
    else {
        printf("New Ticket (Ticket#:%6.6d)\n", ticket_id);
        printf("----------------------------------------\n");
        tickets[i].customer_account_number = account->account_number;
        tickets[i].id = ticket_id;
        tickets[i].is_open = 1;

        // get ticket subject
        printf("Enter the ticket SUBJECT (30 chars. maximum): ");
        getCString(tickets[i].subject_line, 1, TICKET_SUBJECT_LINE_LENGTH);
        printf("\n");
        // get ticket message
        createNewMessage(&tickets[i], account);
        printf("\n");
        printf("*** New ticket created! ***\n");        
    }
}

// creates a new message for a ticket if the ticket is not closed and has room for another ticket.
void createNewMessage(struct Ticket* ticket, const struct Account* account) {
    if (ticket->is_open == 1) {
        if (ticket->message_counter < MESSAGES_SIZE) {
            ticket->messages[ticket->message_counter].account_type = account->account_type;
            strcpy(ticket->messages[ticket->message_counter].display_name, account->user_login.customer_account_holder);

            printf("Enter the ticket message details (150 chars. maximum). Press the ENTER key to submit:\n");
            getCString(ticket->messages[ticket->message_counter].details, 1, TICKET_MESSAGE_DETAILS_LENGTH);
            ticket->message_counter++;
        }
        else {
            printf("ERROR: Message limit has been reached, call ITS Support!\n");
        }
    }
    else {
        printf("ERROR: Ticket is closed - new messages are not permitted.\n");
    }
}

// returns a ticket based on the ticket number.
struct Ticket* getTicket(int ticket_num, struct Ticket* tickets, int length) {
    struct Ticket* ticket = NULL;
    int i = 0;
    for (; i < length; i++) {
        if (tickets[i].id == ticket_num) {
            ticket = &tickets[i];
        }
    }
    return ticket;
}

// closes a ticket if its open and if there is room, asks the user if they want
// to include a final message.
int closeTicketConfirm(struct Ticket* ticket, const struct Account* account) {
    int closed = 0;
    if (ticket->is_open == 1) {
        printf("Are you sure you CLOSE this ticket? ([Y]es|[N]o): ");
        char res = getCharOption("YN");
        printf("\n");
        if (res == 'Y') {
            closed = 1;            
            if (ticket->message_counter < MESSAGES_SIZE) {
                printf("Do you want to leave a closing message? ([Y]es|[N]o): ");
                res = getCharOption("YN");
                printf("\n");
                if (res == 'Y') {
                    createNewMessage(ticket, account);
                    printf("\n");
                }
            }
            ticket->is_open = 0;
            printf("*** Ticket closed! ***\n");
        }
    }
    else {
        printf("ERROR: Ticket is already closed!\n");
    }
    return closed;
}

// reopens a closed ticket.
void reOpenTicket(struct Ticket* ticket) {
    if (ticket->is_open == 1) {
        printf("ERROR: Ticket is already active!\n\n");
    }
    else {
        printf("Are you sure you RE-OPEN this closed ticket? ([Y]es|[N]o): ");
        char res = getCharOption("YN");
        printf("\n");
        if (res == 'Y') {
            ticket->is_open = 1;
            printf("*** Ticket re-opened! ***\n\n");
        }
    }
}