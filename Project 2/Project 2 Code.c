/* Code written by Anuj Bungla, May 2020 (Student ID: XXXXXXX). Codes 
   copied/modified are referenced from their source below when used.*/

/* Algorithms are fun! */

/* Library headers */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/* Stage numbers */ 
#define STAGE_NUM_ONE 1			 
#define STAGE_NUM_TWO 2
#define STAGE_NUM_THREE 3
#define STAGE_NUM_FOUR 4

/* Stage header format string */
#define STAGE_HEADER \
"=========================Stage %d=========================\n" 

/* Status of transaction */
#define IN_BOTH_LIMITS "            IN_BOTH_LIMITS\n"
#define OVER_TRANS_LIMIT "            OVER_TRANS_LIMIT\n"
#define OVER_DAILY_LIMIT "            OVER_DAILY_LIMIT\n"
#define OVER_BOTH_LIMITS "            OVER_BOTH_LIMITS\n"


#define MAX_NO_OF_CARDS 100       /* Maximum number of cards in card record */
#define LEN_OF_CARD_ID 8          /* Length of a unique card ID */
#define LEN_OF_TRANSACTION_ID 12  /* Length of a unique transaction ID */
#define BS_FOUND 0                /* Binary search found */

/* Magic numbers */
#define ZERO 0
#define ONE 1
#define TWO 2
#define FIFTY 50

/* Terminating line */
#define TERMINATING_LINE "%%%%%%%%%%"

/* Null terminator character */
#define NULL_TERMINATOR '\0'

/* New line character */
#define NEW_LINE_CHAR '\n'

/* Credit card struct */
struct credit_template {
    char card_id[LEN_OF_CARD_ID + ONE];
    int daily_limit;
    int transaction_limit;
    /* Addition of variables for stage 4 */
    int count;
    int year, month, day;
    int daily_sum;
    
};
typedef struct credit_template credit_card_t;

/* Transaction record struct */
typedef struct transaction_template transaction_t;

struct transaction_template {
    char transaction_id[LEN_OF_TRANSACTION_ID + ONE];
    char card_id[LEN_OF_CARD_ID + ONE];
    int year, month, day, hour, minute, second;
    int transaction_amount;
    transaction_t* next;
};

/* **********Function prototypes********** */
void print_stage_header(int stage_num);
void stage_one(credit_card_t* card_records[]);
void stage_two(credit_card_t* card_records[], int* n);
void no_of_credit_cards(credit_card_t* card_records[], int* n);
void average_daily_limit(credit_card_t* card_records[], int* n);
void largest_transaction(credit_card_t* card_records[], int* n);
transaction_t* stage_three(void);
credit_card_t* read_one_credit_record(void);
void print_error(void);
void print_linked(transaction_t* ptr);
void stage_four(transaction_t* ptr, 
                credit_card_t* card_records[], int no_of_cards);
int binary_search(char* A[], int lo, int hi, char* key, int* locn);
int cmp(char *x1, char *x2);
void compare(credit_card_t* card, transaction_t* ptr);
int same_day(credit_card_t* card, transaction_t* ptr);
void status(credit_card_t* card, transaction_t* ptr);
void free_linked(transaction_t* head);
void free_array_of_structs(credit_card_t* card_records[], int n);
/* *************************************** */

/* Lets start */
int main() {
    
    /* Head of linked list that contains all transaction records */
    transaction_t* head;
   
    /* Contains pointers to char arrays that contain individual card records */
    credit_card_t* card_records[MAX_NO_OF_CARDS];
    
    /* Holds number of credit card records */
    int no_of_cards = ZERO;
    
    /* Stage 1: Store and print the first credit record */
    stage_one(card_records);
    no_of_cards++;
    
    /* Stage 2: Store all credit cards and provide summary */
    stage_two(card_records, &no_of_cards);
    
    /* Stage 3: Read and print transactions */
    head = stage_three();
    print_linked(head);
    
    /* Stage 4: Check for fraudulent transactions */
    stage_four(head, card_records, no_of_cards); 
    
    /* Free linked list */
    free_linked(head);
    
    /* Free the array of structs formed in Stage 2 */
    free_array_of_structs(card_records, no_of_cards);
    
    /* Done and dusted. Take some rest! */
    return ZERO;
}

/* Print stage headers */
void print_stage_header(int stage_num) {
	printf(STAGE_HEADER, stage_num);
}

/* Print error if memory is not allocated properly */
void print_error(void) {
    printf("Memory not allocated properly. STOP!\n");
}

credit_card_t* read_one_credit_record(void) {
    
    /* https://mkyong.com/c/how-to-handle-unknow-size-user-input-in-c/ */
    /* Modified by Anuj Bungla */
    
    /* Store unknown input size for each credit record into an array. Once
       this is done, we use the sscanf() function to read the information
       provided in credit record and store in our struct. */
    
    /* len_max can be any int. In this case, it is set to 50. If string is
       longer than 50 we can just realloc it. */    
    int len_max = FIFTY;
    
    int current_size = ZERO;
    char* pStr;
    credit_card_t* info;
   
    pStr = (char*)malloc(len_max * sizeof(char));
    current_size = len_max;
    
    if (pStr != NULL) {
        int c, i = ZERO;
        
        /* Accept user input until hit enter or end of file */
        while ((c = getchar()) != NEW_LINE_CHAR && c != EOF) {
            pStr[i++] = c;
            
            /* If 'i' reached maximize size then realloc size */
            if (i == current_size) {
                current_size = i + len_max;
                pStr = realloc(pStr, current_size);
            }
        }
        pStr[i] = NULL_TERMINATOR;
    } else {
        print_error();
    }
    
    /* Create credit_card_t struct using malloc() to store data */
    info = (credit_card_t*)malloc(sizeof(credit_card_t));
    
    /* Store data into dynamically allocated struct */
    sscanf(pStr, "%s %d %d", info->card_id, &(info->daily_limit), 
           &(info->transaction_limit));
    info->count = ZERO;
    info->daily_sum = ZERO;
    
    /* Free pStr and assign null so it cannot be used accidentally */
    free(pStr);
	pStr = NULL;
    
    /* Return pointer to struct, 'info' */
    return info;
}

/* STAGE 1 */
void stage_one(credit_card_t* card_records[]) {
    
    /* Print header for stage */
    print_stage_header(STAGE_NUM_ONE);
    
    /* Store address of the struct that contains the information for the
       first credit card into array */
    card_records[ZERO] = read_one_credit_record();
    
    /* Print information for the first credit card */
    printf("Card ID: %s\n", (card_records[ZERO]->card_id));
    printf("Daily limit: %d\n", (card_records[ZERO]->daily_limit));
    printf("Transaction limit: %d\n\n",
           (card_records[ZERO]->transaction_limit));
    return;    
}

/* STAGE 2 */
void stage_two(credit_card_t* card_records[], int* n) {
    
    /* Print header for stage */
    print_stage_header(STAGE_NUM_TWO);
    
    /* Print no.of credit card records */
    no_of_credit_cards(card_records, n);
    
    /* Print average daily limit */
    average_daily_limit(card_records, n);
    
    /* Print card with largest transaction limit */
    largest_transaction(card_records, n);
  
    return;
}

/* Print number of cards */
void no_of_credit_cards(credit_card_t* card_records[], int* n) {
    
    int i;
    char *end_record = TERMINATING_LINE; /* Terminating line */
    
    for (i = ONE; i < MAX_NO_OF_CARDS; i++) {
        card_records[i] = read_one_credit_record();
        /* Compare each record with terminating line. strcmp() returns
           0 if both are identical. If they are identical, break the
           loop */
        if (strcmp((card_records[i]->card_id), end_record) == ZERO) {
            break;
        } else {
            *n = *n + ONE;
        }
    }
    printf("Number of credit cards: %d\n", *n);
}

/* Print average daily limit */
void average_daily_limit(credit_card_t* card_records[], int* n) {
    
    int i, no_records;
    float average, total = ZERO;
    
    /* store in no_records for easier readability */
    no_records = *n;
    
    /* Iterate over all credit records and add all daily limit values
       present to get a total */
    for (i = ZERO; i < no_records; i++) {
        total += card_records[i]->daily_limit;
    }
    
    /* Divide total by no.of records to get the average */
    average = total / no_records;
    printf("Average daily limit: %.2f\n", average);
}

/* Print card with largest transaction limit */
void largest_transaction(credit_card_t* card_records[], int* n) {
    
    int i, largest, mark, no_records;
    no_records = *n;
    
    /* Set largest to the transaction limit of first index */
    largest = card_records[ZERO]->transaction_limit;
    
    /* Mark the index of the current largest transaction limit which is 0 */
    mark = ZERO;
    
    /* Compare transaction limit of each record. If new largest transaction
       limit is found, mark the index of that credit card record */
    for (i = ONE; i < no_records; i++) {
        if (largest < card_records[i]->transaction_limit) {
            largest = card_records[i]->transaction_limit;
            mark = i;
        } else {
            continue;
        }
    }
    
    /* Using the index saved of the card with the biggest transaction limit,
       print out the ID */
    printf("Card with the largest transaction limit: %s\n\n",
           card_records[mark]->card_id);           
}

/* STAGE 3 */
/* Returns head value which points at the first item of the linked list */
transaction_t* stage_three(void) {
    
    print_stage_header(STAGE_NUM_THREE);
    
    /* Use 'block' variable to control while loop for linking nodes */
    int block = ONE;
    
    /* Set variables to construct a linked list */
    transaction_t *head, *temp, *new_node;
    head = temp = new_node = NULL;
    
    /* https://mkyong.com/c/how-to-handle-unknow-size-user-input-in-c/ */
    /* Modified by Anuj Bungla */
    while (block != ZERO) {
        int len_max = FIFTY;
        int current_size = ZERO;
        char* pStr;
        pStr = (char*)malloc(len_max * sizeof(char));
        current_size = len_max;
        if (pStr != NULL) {
            int c, i = ZERO;
            /* Accept user input until hit enter or end of file */
            while ((c = getchar()) != NEW_LINE_CHAR && c != EOF) {
                pStr[i++] = c;
                /* If 'i' reached maximize size then realloc size */
                if (i == current_size) {
                    current_size = i + len_max;
                    pStr = realloc(pStr, current_size);
                }
            }
            pStr[i] = NULL_TERMINATOR;
            
            if (strlen(pStr) != ZERO) {
                /* Make transacation struct to store information */
                new_node = (transaction_t*)malloc(sizeof(transaction_t));
                /* Use of sscanf to copy data into the node */
                sscanf(pStr, "%s %s %d:%d:%d:%d:%d:%d %d",
                       (new_node->transaction_id), (new_node->card_id),
                       &(new_node->year), &(new_node->month), &(new_node->day),
                       &(new_node->hour), &(new_node->minute),
                       &(new_node->second), &(new_node->transaction_amount));
                if (head == NULL) {
                    /* Case for first node into the list */
                    head = temp = new_node;
                } else {
                    /* Case for rest of the nodes  */
                    temp->next = new_node;
                    temp = new_node;    
                }
            } else {
                block = ZERO;
            }
        }
        /* Free pStr and assign to null so it cannot be used accidentally */
        free(pStr);
        pStr = NULL;
    }
    return head;
}

/* Prints data in linked list */
void print_linked(transaction_t* ptr) {
    while (ptr != NULL) {
        printf("%s\n", ptr->transaction_id);
        ptr = ptr->next;
    }
    printf("\n");
}

/* STAGE 4 */
void stage_four(transaction_t* ptr, credit_card_t* card_records[],
               int no_of_cards) {
    
    print_stage_header(STAGE_NUM_FOUR);
    
    int i, locn;
    char *credit_cards[no_of_cards];
     
    /* An array of char pointer to all the credit cards ONLY */
    for (i = ZERO; i < no_of_cards; i++) {
        credit_cards[i] = card_records[i]->card_id;
    }
    
    /* https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/binarysearch.c */
    /* Modified by Anuj Bungla */
    while (ptr!=NULL) {
		if (binary_search(credit_cards, ZERO, no_of_cards,
                          ptr->card_id, &locn) == BS_FOUND) {
            compare(card_records[locn], ptr);
		}
        ptr = ptr->next;
	}
}

void compare(credit_card_t* card, transaction_t* ptr) {
    /* First encounter of a credit card in the transaction record */
    if (card->count == ZERO) {
        /* Copy over year, month and day to credit card so it can be used 
           to determine whether the next transaction for the 
           same credit card is on the same day or not */
        card->year = ptr->year;
        card->month = ptr->month;
        card->day = ptr->day;
        card->daily_sum += ptr->transaction_amount;
        card->count += ONE;
        /* Check status of transaction */
        status(card, ptr);
    } else {
        /*  Credit card is encountered again and in the same day */
        if (same_day(card, ptr)) {
            /* Keep on adding to daily_sum as transaction is in the 
               same day */
            card->daily_sum += ptr->transaction_amount;
            /* Check status of transaction */
            status(card, ptr);
        } else {
        /*  Credit card is encountered again but not in the same day */
            card->year = ptr->year;
            card->month = ptr->month;
            card->day = ptr->day;
            /* Reset daily sum to 0 becuase daily_sum resets every day */
            card->daily_sum = ZERO;
            card->daily_sum += ptr->transaction_amount;
            /* Check status of transaction */
            status(card,ptr);
        }
    }
}

/* Check if another transaction from the same credit card is in the same 
   day or not. Return 1 if it is, 0 otherwise. */
int same_day(credit_card_t* card, transaction_t* ptr) {
    
    if (((card->year) == (ptr->year)) &&
        ((card->month) == (ptr->month)) &&
        ((card->day) == (ptr->day)) ) {
        return ONE;
    } else {
        return ZERO;
    }
}

/* Print status of the transaction */
void status(credit_card_t* card, transaction_t* ptr) {
    
    if (((ptr->transaction_amount) > (card->transaction_limit)) && 
            ((card->daily_sum) > (card->daily_limit))) {
        printf("%s %s", ptr->transaction_id, OVER_BOTH_LIMITS);
    } else if (((ptr->transaction_amount) > (card->transaction_limit)) && 
            ((card->daily_sum) < (card->daily_limit))) {
        printf("%s %s", ptr->transaction_id, OVER_TRANS_LIMIT);
    } else if (((ptr->transaction_amount) < (card->transaction_limit)) && 
            ((card->daily_sum) > (card->daily_limit))) {
        printf("%s %s", ptr->transaction_id, OVER_DAILY_LIMIT);
    } else {
        printf("%s %s", ptr->transaction_id, IN_BOTH_LIMITS);
    }
}

/* https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/binarysearch.c */
/* Modified by Anuj Bungla */
int binary_search(char* A[], int lo, int hi, char* key, int* locn) {
	int mid, outcome;
	/* if key is in A, it is between A[lo] and A[hi-1] */
	mid = (lo + hi) / TWO;
	if ((outcome = cmp(key, A[mid])) < ZERO) {
		return binary_search(A, lo, mid, key, locn);
	} else if (outcome > ZERO) {
		return binary_search(A, mid + ONE, hi, key, locn);
	} else {
		*locn = mid;
		return BS_FOUND;
	}
}

/* https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/binarysearch.c */
/* Modified by Anuj Bungla */
/* Compares key string to base strings for binary search */
int cmp(char *x1, char *x2) {
	return strcmp(x1, x2);
}

/* Free all nodes in linked list */
void free_linked(transaction_t* head) {
    transaction_t* tmp;
    while (head != NULL) {
        tmp = head;
        head = head -> next;
        free(tmp);
    }
}

/* Free array of structs in stage 2 */
void free_array_of_structs(credit_card_t* card_records[], int n)  {
    int i;
    for (i = ZERO; i < n; i++) {
        free(card_records[i]);
    }
}

/* 

Time complexity analysis:
   
Lets say there were 15 credit cards in total. It would take one guess for 
1 credit card. Two guesses for 2 cards. Three guesses for 4 cards and four
guesses for 8 cards using binary search. It would look like something like
this in a table.
   
   Total: 15 elements
   
   Elements  |  Guesses
      1            1
      2            2
      4            3
      8            4
      
The average would then simply be the expectation of both variables.
Formulae for expectation is
Σ(xy)/n = ((1*1)+(2*2)+(4*3)+(8*4))/15 = 3.237 ≈ 3
   
We can see that there is approximately log2(n) pattern here. Hence, 
it would take an average time of O(log2(m)) to find the credit card 
corresponding to the unique transaction ID, where 'm' is the number 
of credit card records. However, this is for a single transaction. 
If we have 'n' more transactions, we would have to repeat the process
'n' more times. As a result, this is would give us an average time
complexity of O(nlog2(m)).

*/