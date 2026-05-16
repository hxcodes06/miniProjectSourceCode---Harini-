// Fully improved Bank Management System (fixed bugs + better validation)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100

struct clientData {
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void viewSingleRecord(FILE *fPtr);
void listAllAccounts(FILE *fPtr);

int main() {
    FILE *cfPtr = fopen("credit.dat", "rb+");
    unsigned int choice;

    if (cfPtr == NULL) {
        printf("Error: File could not be opened.\n");
        return 1;
    }

    while ((choice = enterChoice()) != 7) {
        switch (choice) {
            case 1: listAllAccounts(cfPtr); break;
            case 2: textFile(cfPtr); break;
            case 3: updateRecord(cfPtr); break;
            case 4: newRecord(cfPtr); break;
            case 5: deleteRecord(cfPtr); break;
            case 6: viewSingleRecord(cfPtr); break;
            default: printf("Invalid choice! Try again.\n");
        }
    }

    fclose(cfPtr);
    printf("Program closed successfully.\n");
    return 0;
}

void listAllAccounts(FILE *fPtr) {
    struct clientData client = {0, "", "", 0.0};

    rewind(fPtr);

    printf("\nAcct   Last Name       First Name      Balance\n");
    printf("-----------------------------------------------\n");

    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1) {
        if (client.acctNum != 0) {
            printf("%-6u %-15s %-15s %.2f\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);
        }
    }
}

void textFile(FILE *readPtr) {
    FILE *writePtr = fopen("accounts.txt", "w");
    struct clientData client = {0, "", "", 0.0};

    if (writePtr == NULL) {
        printf("Cannot create accounts.txt\n");
        return;
    }

    rewind(readPtr);

    fprintf(writePtr,
            "Acct   Last Name       First Name      Balance\n");

    while (fread(&client, sizeof(struct clientData), 1, readPtr) == 1) {
        if (client.acctNum != 0) {
            fprintf(writePtr,
                    "%-6u %-15s %-15s %.2f\n",
                    client.acctNum,
                    client.lastName,
                    client.firstName,
                    client.balance);
        }
    }

    fclose(writePtr);
    printf("accounts.txt created successfully!\n");
}

void updateRecord(FILE *fPtr) {
    unsigned int account;
    double transaction;
    struct clientData client = {0, "", "", 0.0};

    printf("Enter account number (1-100): ");
    scanf("%u", &account);

    if (account < 1 || account > MAX_ACCOUNTS) {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0) {
        printf("Account not found.\n");
        return;
    }

    printf("Current balance: %.2f\n", client.balance);
    printf("Enter amount (+deposit / -withdraw): ");
    scanf("%lf", &transaction);

    if (client.balance + transaction < 0) {
        printf("Insufficient balance. Transaction cancelled.\n");
        return;
    }

    client.balance += transaction;

    /* only here negative offset is needed */
    fseek(fPtr,
          -(long)sizeof(struct clientData),
          SEEK_CUR);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Balance updated successfully!\n");
}

void newRecord(FILE *fPtr) {
    struct clientData client = {0, "", "", 0.0};
    unsigned int accountNum;

    printf("Enter new account number (1-100): ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > MAX_ACCOUNTS) {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0) {
        printf("Account already exists!\n");
        return;
    }

    printf("Enter lastname firstname balance: ");
    scanf("%14s %9s %lf",
          client.lastName,
          client.firstName,
          &client.balance);

    if (client.balance < 0) {
        printf("Balance cannot be negative.\n");
        return;
    }

    client.acctNum = accountNum;

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("New account created successfully!\n");
}

void deleteRecord(FILE *fPtr) {
    struct clientData blankClient = {0, "", "", 0.0};
    struct clientData client;
    unsigned int accountNum;

    printf("Enter account number to delete: ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > MAX_ACCOUNTS) {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0) {
        printf("Account does not exist.\n");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fwrite(&blankClient,
           sizeof(struct clientData),
           1,
           fPtr);

    printf("Account deleted successfully!\n");
}

void viewSingleRecord(FILE *fPtr) {
    struct clientData client = {0, "", "", 0.0};
    unsigned int account;

    printf("Enter account number to view: ");
    scanf("%u", &account);

    if (account < 1 || account > MAX_ACCOUNTS) {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
        printf("Account not found.\n");
    else
        printf("%u | %s %s | Balance: %.2f\n",
               client.acctNum,
               client.firstName,
               client.lastName,
               client.balance);
}

unsigned int enterChoice(void) {
    unsigned int choice;

    printf("\n===== BANK MANAGEMENT SYSTEM =====\n");
    printf("1. View all accounts\n");
    printf("2. Export all accounts to text file\n");
    printf("3. Update account balance\n");
    printf("4. Add new account\n");
    printf("5. Delete account\n");
    printf("6. View single account\n");
    printf("7. Exit\n");
    printf("Enter choice: ");

    scanf("%u", &choice);

    return choice;
}