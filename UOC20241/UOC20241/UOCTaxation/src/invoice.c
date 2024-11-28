#include "tenant.h"
#include "invoice.h"
#include "date.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>


// Initialize an invoice list
void invoiceList_init(tRentInvoiceData * list) {
    /* PR2 Ex 1a */
    /*INIT PROCEDURE*/
    assert(list != NULL);
    list->first = NULL;
    list->count = 0;
}

// Function to update or add a tenant's rent across their tenancy period
void invoiceList_update(tRentInvoiceData * list, tDate startMonthDate, tDate endMonthDate, char * cadastral_ref, float rent) {
    /* PR2 Ex 1b */
    /* RECEIVED FROM TEST */
    //tenant_initialize(&tenant,"01/09/2022","31/08/2023","T001","John Doe",1000.0f,30,"ZYX1234","LE24001");
                                //datecheck   datecheck
    //                          From both, check montly invoice
    //                          update invoices
    //                                                    calculate new invoice
    //                                                                                  get cadastral_ref
    //                                                                                  sort by cadastral ref
    assert(list != NULL);
    assert(cadastral_ref != NULL);

    tRentInvoiceMonthly *current = list->first;
    tRentInvoiceMonthly *prev = NULL;

    /*FIND POSITION IN LIST*/
    while (current != NULL && date_cmp(current->month, startMonthDate) < 0) {
    prev = current;
    current = current->next;
    } tDate currentMonth = startMonthDate;

    /*FOR EACH MONTH IN RANGE ITERATE THROUGH MONTHS*/
    while (date_cmp(currentMonth, endMonthDate) <= 0) {
        
    /*CREATE NEW INVOICE FOR MONTH*/
    if (current == NULL || date_cmp(current->month, currentMonth) != 0) {
        tRentInvoiceMonthly *newInvoice = malloc(sizeof(tRentInvoiceMonthly));
        if (newInvoice == NULL) {
            exit(EXIT_FAILURE);
        }
        monthlyInvoice_init(newInvoice, currentMonth);

        /*INVOICE INSERT PROCEDURE*/
        if (prev == NULL) {
            newInvoice->next = list->first;
            list->first = newInvoice;
        } else {
            newInvoice->next = prev->next;
            prev->next = newInvoice;
        }

        list->count++;
        prev = newInvoice;
        current = newInvoice;
    }

    /*ADD OR UPDATE INVOICES PRODEDURE*/
    tRentInvoiceNode *currentNode = current->first;
    tRentInvoiceNode *prevNode = NULL;
    bool updated = false;

    while (currentNode != NULL) {
        if (strcmp(currentNode->elem.cadastral_ref, cadastral_ref) == 0) {
            currentNode->elem.rent = rent;
            updated = true;
            break;
        } else if (strcmp(currentNode->elem.cadastral_ref, cadastral_ref) > 0) {
            break;
        }
        prevNode = currentNode;
        currentNode = currentNode->next;
    }

    if (!updated) {
        tRentInvoiceNode *newNode = malloc(sizeof(tRentInvoiceNode));
        if (newNode == NULL) {
            exit(EXIT_FAILURE);
        }
        invoiceNode_init(newNode, rent, cadastral_ref);

        if (prevNode == NULL) {
            newNode->next = current->first;
            current->first = newNode;
            } else {
            newNode->next = prevNode->next;
            prevNode->next = newNode;
        }
        current->count++;
    }

    /*NEXT MONTH*/
    date_addMonth(&currentMonth, 1);

    if (current != NULL && date_cmp(current->month, currentMonth) < 0) {
        prev = current;
        current = current->next;
        }
    }
}
// Get the rent amount for a certain invoice and date
float getInvoiceMonthly(tRentInvoiceData * data, tDate date) {
    /* PR2 Ex 1c */
    /*FLOAT CONDITIONS = 2 DECIMALS*/
    float totalRent = 0.00;
    tRentInvoiceMonthly* currentMonthly = data->first;
   
    /*ITERATE THROUGH INVOICES*/
    while (currentMonthly != NULL) {
       
        /*MATCHING MONTH&YEAR PROCEDURE*/
        if (currentMonthly->month.year == date.year && currentMonthly->month.month == date.month) {
            tRentInvoiceNode* currentNode = currentMonthly->first;
            while (currentNode != NULL) {
               
                /*ADD THE RENT TO TOTAL*/
                totalRent += currentNode->elem.rent;
                currentNode = currentNode->next;
                }
                return totalRent;
            }
        currentMonthly = currentMonthly->next;
    }
return totalRent;
}

// Release a list of invoices
void invoiceList_free(tRentInvoiceData * list) {
    /*PR2 Ex 1d*/
    /*VAR DEF*/
    tRentInvoiceMonthly* currentMonthly = list->first;
    tRentInvoiceMonthly* tempMonthly;

    /*CHECK FOR EMPTY LISTS*/
    if (list == NULL || list->first == NULL) {
        return;
    }

    /*ITERATE INVOICE PROCEDURE*/
    while (currentMonthly != NULL) {
        tRentInvoiceNode* currentNode = currentMonthly->first;
        tRentInvoiceNode* tempNode;
        while (currentNode != NULL) {
            tempNode = currentNode->next;
            free(currentNode);
            currentNode = tempNode;
        }
        /*move to next invoice*/
        tempMonthly = currentMonthly->next;
        free(currentMonthly);
        currentMonthly = tempMonthly;
    }

    /*LIST RESTART*/
    list->first = NULL;
    list->count = 0;
}

// Initialize an invoice element
void invoice_init(tRentInvoice * invoice, float rent,
    const char * cadastral_ref) {
    invoice -> rent = rent;
    strncpy(invoice -> cadastral_ref, cadastral_ref, MAX_CADASTRAL_REF);
    invoice -> cadastral_ref[MAX_CADASTRAL_REF] = '\0';
}

// Initialize an invoice node element
void invoiceNode_init(tRentInvoiceNode * node, float rent,
    const char * cadastral_ref) {
    assert(node != NULL);
    invoice_init( & node -> elem, rent, cadastral_ref);
    node -> next = NULL;
}

// Initialize a monthly invoice element
void monthlyInvoice_init(tRentInvoiceMonthly * invoice, tDate date) {
    assert(invoice != NULL);
    invoice -> month = date;
    invoice -> first = NULL;
    invoice -> count = 0;
    invoice -> next = NULL;
}

// Remove a monthly invoice element data
void monthlyInvoice_free(tRentInvoiceMonthly * invoice) {
    assert(invoice != NULL);
    tRentInvoiceNode * current = invoice -> first;
    while (current != NULL) {
        tRentInvoiceNode * next = current -> next;
        free(current);
        current = next;
    }
    invoice -> first = NULL;
    invoice -> count = 0;
}

// Function to find a monthly invoice by date
tRentInvoiceMonthly * findMonthlyInvoiceByDate(tRentInvoiceData * list, tDate date) {
    assert(list != NULL);

    tRentInvoiceMonthly * current = list -> first;
    while (current != NULL) {
        int cmpResult = date_cmp(current -> month, date);
        if (cmpResult == 0) {
            // Found the monthly invoice for the specified date
            return current;
        } else if (cmpResult > 0) {
            // Since the list is ordered chronologically, no need to continue
            break;
        }
        current = current -> next;
    }
    // Monthly invoice for the specified date not found
    return NULL;
}

// Print on the screen the information of an invoice list
void printRentInvoiceData(tRentInvoiceData * list) {
    assert(list != NULL);
    tRentInvoiceMonthly * current = list -> first;
    while (current != NULL) {
        printf("Month: %d-%d\n", current -> month.year, current -> month.month);
        printRentInvoiceNode(current -> first);
        current = current -> next;
    }
}

// Print on the screen the information of a Node of a list of rent invoices
void printRentInvoiceNode(tRentInvoiceNode * node) {
    assert(node != NULL);
    while (node != NULL) {
        printf("Cadastral Ref: %s, Rent: %.2f\n", node -> elem.cadastral_ref, node -> elem.rent);
        node = node -> next;
    }
}

/////////////////////////////////////////
///// AUX Methods: Test PR2		   //////
/////////////////////////////////////////

// Function to verify invoices within a monthly invoice node
bool verifyMonthlyInvoices(tRentInvoiceMonthly * monthlyInvoice, ExpectedInvoices * expectedInvoices) {
    assert(monthlyInvoice != NULL);
    assert(expectedInvoices != NULL);

    // Verify the number of invoices
    if (monthlyInvoice -> count != expectedInvoices -> invoiceCount) {
        return false;
    }

    tRentInvoiceNode * currentInvoiceNode = monthlyInvoice -> first;
    int invoiceIndex = 0;

    while (currentInvoiceNode != NULL && invoiceIndex < expectedInvoices -> invoiceCount) {
        // Verify cadastral_ref and rent
        if (strcmp(currentInvoiceNode -> elem.cadastral_ref, expectedInvoices -> invoices[invoiceIndex].cadastral_ref) != 0 ||
            currentInvoiceNode -> elem.rent != expectedInvoices -> invoices[invoiceIndex].rent) {

            return false;
        }
        currentInvoiceNode = currentInvoiceNode -> next;
        invoiceIndex++;
    }

    // Check if there are extra invoices
    if (currentInvoiceNode != NULL || invoiceIndex != expectedInvoices -> invoiceCount) {
        return false;
    }

    // All invoices match
    return true;
}

int testVerifySpecificDate(tRentInvoiceData * invoiceList,
    const char * dateStr, ExpectedInvoices * expectedInvoices) {
    tDate dateToVerify;
    date_parse( & dateToVerify, dateStr);
    dateToVerify.day = 1;
    int passed = 0;
    tRentInvoiceMonthly * monthlyInvoice = findMonthlyInvoiceByDate(invoiceList, dateToVerify);

    if (monthlyInvoice == NULL) {

    }

    if (verifyMonthlyInvoices(monthlyInvoice, expectedInvoices)) {
        passed = 1;
    } else {

    }
    return passed;
}



/////////////////////////////////////////
///// AUX Methods: Top-down design //////
/////////////////////////////////////////

