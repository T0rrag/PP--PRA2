#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include "tax_office.h"
#include "invoice.h"

// Initialize a tax office
void taxOffice_init(tTaxOffice * office,  const char * office_code) {
	/*Ex PR2 2a*/
   
   /*NULL & LEN CHECKERS*/
   assert(office != NULL);
   assert(office_code != NULL);
   assert(strlen(office_code) <= MAX_OFFICE_CODE);
   
   /*GET OFFICECODE PROCEDURE*/
   strncpy(office->office_code, office_code, MAX_OFFICE_CODE);
   office->office_code[MAX_OFFICE_CODE] = '\0';
   
   /*INVOICE INIT STORE IN OFFICE */
   invoiceList_init(&office->rentInvoices);
}

// Release a tax office's data
void taxOffice_free(tTaxOffice * office) {
	/*Ex PR2 2b*/
    
    /*NULL CHECKER*/
    assert(office != NULL);
    
    /*LIST FREE*/
    invoiceList_free(&office->rentInvoices);
}

// Initialize a list of tax offices
void taxOfficeList_init(tTaxOfficeList * list) {
	/*Ex PR2 2c*/
    
    /*NULL CHECKER*/
    assert(list != NULL);
    
    /*LIST INIT*/
    list->first = NULL;
    list->count = 0;
}

// Check if an office code is valid
bool check_office_code(const char * office_code) {
	/*Ex PR2 2d*/
    
    /*VAR*/
    size_t len = strlen(office_code);

    /*NULL & LEN CHECKER*/
    if (office_code == NULL) {
        return false;
    }
    if (len != 6 && len != 7) {
        return false;
    }
    
    /*PROVINCE PREFIX & LEN CHECKER*/
    if (!isupper(office_code[0])) {
        return false;
    }
    if (len == 7 && !isupper(office_code[1])) {
        return false;
    }
    size_t start_index = (len == 7) ? 2 : 1;
    for (size_t i = start_index; i<len; i++) {
        if (!isdigit(office_code[i])) {
            return false;
        }
    }
    
    /*IF EVERYTHING PASSES RETURN TRUE*/    
    return true;
}

// Insert a new tax office
void taxOfficeList_insert(tTaxOfficeList * list,  const char * office_code) {
	/*Ex PR2 2e*/
   
   /*VAR*/

   
   /*NULL CHECKER*/
   assert(list != NULL);
   assert(office_code != NULL);
   
   /*OFFICECODE CHECKER*/
   if (!check_office_code(office_code)) {
       return;
   }
   
    /*OFFICE EXIST PROCEDURE*/
    tTaxOfficeNode *current=list->first;
    tTaxOfficeNode *previous = NULL;
    while (current != NULL && strcmp(current->elem.office_code, office_code) < 0) {
        previous = current;
        current = current->next;
    }
    
    /*AVOID REPEATED OFFICES*/
    if (current != NULL && strcmp(current->elem.office_code, office_code) == 0) {
    return;
    }
    
    /*ADD OFFICE NODE*/
    tTaxOfficeNode *newNode = (tTaxOfficeNode *) malloc(sizeof(tTaxOfficeNode));
    assert(newNode != NULL);
    
    /*NEW NODE INIT PROCEDURE*/
    taxOffice_init(&newNode->elem, office_code);
    newNode->next = NULL;
    
    /*NODE INSERT*/
    if (previous == NULL) {
        newNode->next = list->first;
        list->first=newNode;
    } else {
        previous->next = newNode;
        newNode->next = current;
    }
    /*ADD LIST COUNTER*/
    list->count++;
}

// Find a tax office
tTaxOffice * taxOfficeList_find(tTaxOfficeList * list,  const char * office_code) {
	/* Ex PR2 2f*/
    
    /*NULL CHECKER*/
    assert(list != NULL);
    assert(office_code != NULL);
    
    /*ITERATE THROUGH LISTS*/
    tTaxOfficeNode *current = list->first;
    while (current != NULL) {
        if (strcmp(current->elem.office_code, office_code) == 0) {
            return &current->elem; /*POINTER TO TARGET ELEMENT*/
        }
        current = current->next;
    }
   /*ELSE RETURN NULL*/
   return NULL;
}

// Release a list of tax offices
void taxOfficeList_free(tTaxOfficeList * list) {
	/*Ex PR2 2g*/
    
    /*NULL CHECKER*/
    assert(list != NULL);
    
    tTaxOfficeNode *current = list->first;
    tTaxOfficeNode *next;
    
    /*ITERATE THROUGH & FREE NODES*/
    while (current != NULL) {
        next = current->next;
        
        /*OFFICE FREE*/
        taxOffice_free(&current->elem);
        
        /*NODE FREE*/
        free(current);
        current = next;
    }
    /*RESTART COUNTERS*/
    list->first = NULL;
    list->count = 0;
}





