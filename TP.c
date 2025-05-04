#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "bank_data.txt"

typedef struct Account {
    char name[50];
    int account_number;
    char password[20];
    float balance;
    struct Account *next;
} Account;

Account *head = NULL;

void save_data() {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error saving data!\n");
        return;
    }

    Account *current = head;
    while (current != NULL) {
        fprintf(file, "%s %d %s %.2f\n", 
                current->name, 
                current->account_number, 
                current->password, 
                current->balance);
        current = current->next;
    }
    fclose(file);
}

void load_data() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) return;

    Account *last = NULL;
    while (1) {
        Account *new_account = (Account*)malloc(sizeof(Account));
        if (fscanf(file, "%49s %d %19s %f", 
                  new_account->name, 
                  &new_account->account_number, 
                  new_account->password, 
                  &new_account->balance) != 4) {
            free(new_account);
            break;
        }
        
        new_account->next = NULL;
        
        if (head == NULL) {
            head = new_account;
        } else {
            last->next = new_account;
        }
        last = new_account;
    }
    fclose(file);
}

Account* find_account(int account_number) {
    Account *current = head;
    while (current != NULL) {
        if (current->account_number == account_number) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void create_account() {
    Account *new_account = (Account*)malloc(sizeof(Account));
    
    printf("\nEnter your name: ");
    scanf("%49s", new_account->name);
    
    printf("Enter account number: ");
    scanf("%d", &new_account->account_number);
    
    if (find_account(new_account->account_number) != NULL) {
        printf("Account number already exists!\n");
        free(new_account);
        return;
    }
    
    printf("Enter password: ");
    scanf("%19s", new_account->password);
    
    new_account->balance = 0.0f;
    new_account->next = head;
    head = new_account;
    
    save_data();
    printf("Account created successfully!\n");
}

void account_login() {
    int account_number;
    char password[20];
    
    printf("\nEnter account number: ");
    scanf("%d", &account_number);
    
    printf("Enter password: ");
    scanf("%19s", password);
    
    Account *account = find_account(account_number);
    if (account == NULL || strcmp(account->password, password) != 0) {
        printf("Invalid account number or password!\n");
        return;
    }
    
    printf("\nWelcome, %s!\n", account->name);
    
    int choice;
    do {
        printf("\n1. Deposit money\n");
        printf("2. Transfer money\n");
        printf("3. Check balance\n");
        printf("4. Delete account\n");
        printf("5. Logout\n");
        printf("Your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: {
                float amount;
                printf("Enter amount to deposit: ");
                scanf("%f", &amount);
                if (amount > 0) {
                    account->balance += amount;
                    save_data();
                    printf("Deposit successful. New balance: %.2f\n", account->balance);
                } else {
                    printf("Invalid amount!\n");
                }
                break;
            }
            case 2: {
                int target_account;
                float amount;
                printf("Enter recipient account number: ");
                scanf("%d", &target_account);
                Account *target = find_account(target_account);
                if (target == NULL) {
                    printf("Account not found!\n");
                    break;
                }
                printf("Enter amount to transfer: ");
                scanf("%f", &amount);
                if (amount <= 0 || amount > account->balance) {
                    printf("Invalid amount or insufficient balance!\n");
                    break;
                }
                account->balance -= amount;
                target->balance += amount;
                save_data();
                printf("Transfer successful. New balance: %.2f\n", account->balance);
                break;
            }
            case 3:
                printf("Your balance: %.2f\n", account->balance);
                break;
            case 4:
                if (account->balance == 0) {
                    if (head == account) {
                        head = account->next;
                    } else {
                        Account *prev = head;
                        while (prev->next != account) {
                            prev = prev->next;
                        }
                        prev->next = account->next;
                    }
                    free(account);
                    save_data();
                    printf("Account deleted successfully!\n");
                    return;
                } else {
                    printf("Cannot delete account with balance!\n");
                }
                break;
            case 5:
                printf("Logged out successfully!\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 5);
}

void display_menu() {
    printf("\n    >> BANK MANAGEMENT SYSTEM <<\n");
    printf("1. Create new account\n");
    printf("2. Login to account\n");
    printf("3. Exit\n");
    printf("Your choice: ");
}

void free_memory() {
    Account *current = head;
    while (current != NULL) {
        Account *temp = current;
        current = current->next;
        free(temp);
    }
}

int main() {
    load_data();
    
    int choice;
    do {
        display_menu();
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                create_account();
                break;
            case 2:
                account_login();
                break;
            case 3:
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 3);
    
    save_data();
    free_memory();
    return 0;
}