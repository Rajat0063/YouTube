#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/* System Configuration */
#define MAX_ACCOUNTS 100          // Maximum accounts system can handle
#define FILENAME "bank_data.dat"  // File to store account data

/* Account structure */
typedef struct {
    int accountNumber;            // Unique ID
    char name[50];                // Customer name
    char address[100];            // Customer address  
    char phone[15];               // Phone number
    char email[50];               // Email address
    char accountType[20];         // Savings/Current
    float balance;                // Account balance
    time_t createdAt;             // Account creation time
} Account;

Account accounts[MAX_ACCOUNTS];   // Array to store accounts
int numAccounts = 0;              // Track number of accounts

/* Function prototypes */
void loadAccounts();
void saveAccounts();
void createAccount();
void displayAccountDetails(int index);
int findAccount(int accountNumber);
int validateEmail(const char *email);
void printHeader();
void clearInputBuffer();

/* Main function */
int main() {
    loadAccounts(); // Load existing accounts
    
    int choice;
    do {
        system("clear || cls"); // Clear screen
        printHeader();
        
        // Main Menu
        printf("\n\t\tMAIN MENU\n");
        printf("\t\t1. CREATE NEW ACCOUNT\n");
        printf("\t\t2. EXIT\n"); // Only 2 options for Video 1
        
        // Get user input
        printf("\n\t\tENTER YOUR CHOICE: ");
        if (scanf("%d", &choice) != 1) { // Input validation
            clearInputBuffer();
            printf("\n\t\tInvalid input! Please enter 1 or 2.\n");
            continue;
        }
        clearInputBuffer();
        
        switch(choice) {
            case 1: 
                createAccount();
                break;
            case 2:
                printf("\n\t\tThank you for using our banking system!\n");
                saveAccounts(); // Save before exiting
                exit(0);
            default:
                printf("\n\t\tInvalid choice! Please try again.\n");
        }
        
        printf("\n\t\tPress Enter to continue...");
        while (getchar() != '\n'); // Wait for Enter key
    } while (1); // Infinite loop
    
    return 0;
}

/* Load accounts from file */
void loadAccounts() {
    FILE *file = fopen(FILENAME, "rb");
    if (file != NULL) {
        // Read all account records at once
        size_t read = fread(accounts, sizeof(Account), MAX_ACCOUNTS, file);
        numAccounts = (int)read; // Cast size_t to int
        fclose(file);
    }
}

/* Save accounts to file */
void saveAccounts() {
    FILE *file = fopen(FILENAME, "wb");
    if (file != NULL) {
        fwrite(accounts, sizeof(Account), numAccounts, file);
        fclose(file);
    } else {
        printf("\n\t\tError saving accounts!\n");
    }
}

/* Create new account */
void createAccount() {
    system("clear || cls");
    printHeader();
    printf("\n\t\tCREATE NEW ACCOUNT\n");
    
    if (numAccounts >= MAX_ACCOUNTS) {
        printf("\n\t\tMaximum account limit reached!\n");
        return;
    }
    
    Account newAcc;
    // Generate account number (1000, 1001, etc.)
    newAcc.accountNumber = (numAccounts == 0) ? 1000 : accounts[numAccounts-1].accountNumber + 1;
    
    printf("\n\t\tAccount Number: %d\n", newAcc.accountNumber);
    
    // Get customer details
    printf("\t\tEnter Full Name: ");
    fgets(newAcc.name, sizeof(newAcc.name), stdin);
    newAcc.name[strcspn(newAcc.name, "\n")] = '\0'; // Remove newline
    
    printf("\t\tEnter Address: ");
    fgets(newAcc.address, sizeof(newAcc.address), stdin);
    newAcc.address[strcspn(newAcc.address, "\n")] = '\0';
    
    printf("\t\tEnter Phone Number: ");
    fgets(newAcc.phone, sizeof(newAcc.phone), stdin);
    newAcc.phone[strcspn(newAcc.phone, "\n")] = '\0';
    
    // Validate email format
    do {
        printf("\t\tEnter Email: ");
        fgets(newAcc.email, sizeof(newAcc.email), stdin);
        newAcc.email[strcspn(newAcc.email, "\n")] = '\0';
        
        if (!validateEmail(newAcc.email)) {
            printf("\t\tInvalid email format! Use name@domain.com\n");
        }
    } while (!validateEmail(newAcc.email));
    
    printf("\t\tEnter Account Type (Savings/Current): ");
    fgets(newAcc.accountType, sizeof(newAcc.accountType), stdin);
    newAcc.accountType[strcspn(newAcc.accountType, "\n")] = '\0';
    
    // Get initial deposit with validation
    printf("\t\tEnter Initial Deposit: ");
    while (scanf("%f", &newAcc.balance) != 1 || newAcc.balance < 0) {
        clearInputBuffer();
        printf("\t\tInvalid amount! Enter positive value: ");
    }
    clearInputBuffer();
    
    newAcc.createdAt = time(NULL); // Set creation timestamp
    
    // Add to accounts array and save
    accounts[numAccounts++] = newAcc;
    saveAccounts();
    
    printf("\n\t\tAccount created successfully!\n");
    displayAccountDetails(numAccounts-1); // Show new account
}

/* Display account details */
void displayAccountDetails(int index) {
    printf("\n\t\tAccount Number: %d", accounts[index].accountNumber);
    printf("\n\t\tName: %s", accounts[index].name);
    printf("\n\t\tAddress: %s", accounts[index].address);
    printf("\n\t\tPhone: %s", accounts[index].phone);
    printf("\n\t\tEmail: %s", accounts[index].email);
    printf("\n\t\tAccount Type: %s", accounts[index].accountType);
    printf("\n\t\tBalance: %.2f", accounts[index].balance);
    
    // Format creation date
    char dateStr[50];
    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d %H:%M:%S", 
            localtime(&accounts[index].createdAt));
    printf("\n\t\tCreated On: %s\n", dateStr);
}

/* Find account by number (returns index or -1) */
int findAccount(int accountNumber) {
    for (int i = 0; i < numAccounts; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            return i;
        }
    }
    return -1; // Not found
}

/* Basic email validation */
int validateEmail(const char *email) {
    int at = 0, dot = 0;
    for (int i = 0; email[i] != '\0'; i++) {
        if (email[i] == '@') at++;
        else if (email[i] == '.') dot++;
    }
    return (at == 1 && dot >= 1); // Must have exactly one @ and at least one .
}

/* Print system header */
void printHeader() {
    printf("\n\t\t********************************************");
    printf("\n\t\t*          BANKING MANAGEMENT SYSTEM        *");
    printf("\n\t\t********************************************\n");
}

/* Clear input buffer */
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}