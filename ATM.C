#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

enum TransactionType { DEPOSIT, WITHDRAWAL, TRANSFER, QUICK_CASH };

struct Transaction {
    enum TransactionType type;
    int amount;
};

struct ATM {
    char username[30];
    char password[5];
    int balance;
    struct Transaction transactions[5];
};

void get_timestamp() {
    time_t t;
    struct tm *tm_info;
    time(&t);
    tm_info = localtime(&t);
    printf("%s", asctime(tm_info));
}

void delay(int milliseconds) {
    Sleep(milliseconds);
}

void get_data(struct ATM *a) {
    int i;
    printf("Enter username: ");
    scanf("%s", a->username);
    printf("Enter 4-digit password: ");
    scanf("%4s", a->password);
    printf("Enter initial balance: ");
    scanf("%d", &a->balance);

    for (i = 0; i < 5; ++i) {
        a->transactions[i].type = DEPOSIT;
        a->transactions[i].amount = 0;
    }
}

void update_transaction_history(struct ATM *a, enum TransactionType type, int amount) {
    for (int i = 4; i > 0; --i) {
        a->transactions[i] = a->transactions[i - 1];
    }

    a->transactions[0].type = type;
    a->transactions[0].amount = amount;
}

void show_data(struct ATM *a) {
    printf("Username: %s, Balance: %d\n", a->username, a->balance);
}

void print_receipt(struct ATM *a, int amount, const char *transaction) {
    printf("\n~~~~~~~~~ Receipt ~~~~~~~~~~~~~~\n");
    get_timestamp();
    printf("Username: %s\n", a->username);
    printf("%s: %d\n", transaction, amount);
    printf("New Balance: %d\n", a->balance);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    delay(2000);
}

void print_mini_statement(char *uname) {
    struct ATM a;
    FILE *file;
    file = fopen("atm.txt", "r");
    while (fread(&a, sizeof(struct ATM), 1, file) == 1) {
        if (strcmp(a.username, uname) == 0) {
            printf("\n~~~~~~~~~ Mini Statement ~~~~~~~~~\n");
            get_timestamp();
            printf("Username: %s\tBalance: %d", a.username, a.balance);
            printf("\n");
            for (int i = 0; i < 5; ++i) {
                if (a.transactions[i].amount != 0) {
                    switch (a.transactions[i].type) {
                        case DEPOSIT:
                            printf("Deposit: %d\n", a.transactions[i].amount);
                            break;
                        case WITHDRAWAL:
                            printf("Withdrawal: %d\n", abs(a.transactions[i].amount));
                            break;
                        case TRANSFER:
                            if (a.transactions[i].amount < 0) {
                                printf("Transfer: %d\n", abs(a.transactions[i].amount));
                            } else {
                                printf("Received transfer: %d\n", a.transactions[i].amount);
                            }
                            break;
                        case QUICK_CASH:
                            printf("Quick Cash Withdrawal: %d\n", abs(a.transactions[i].amount));
                            break;
                        default:
                            break;
                    }
                }
            }
            printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
            break;
        }
    }
    fclose(file);
}

int add_user() {
    struct ATM a;
    int ch;
    FILE *file;
    file = fopen("atm.txt", "a");
    printf("Enter account details to add: \n");
    do {
        get_data(&a);
        fwrite(&a, sizeof(struct ATM), 1, file);
        printf("Do you want to continue?\n1.Yes\t2.No\n");
        scanf("%d", &ch);
    } while (ch != 2);
    fclose(file);
    return 0;
}

void update_user_as_deposit(char *uname) {
    struct ATM a;
    FILE *file, *temp;
    file = fopen("atm.txt", "r+");
    temp = fopen("temp.txt", "a");
    while (fread(&a, sizeof(struct ATM), 1, file) == 1) {
        if (strcmp(a.username, uname) == 0) {
            int deposit;
            printf("\nEnter amount to deposit: ");
            scanf("%d", &deposit);
            a.balance += deposit;
            printf("\nBalance is: %d\n", a.balance);
            print_receipt(&a, deposit, "Deposit");
            update_transaction_history(&a, DEPOSIT, deposit);
            fwrite(&a, sizeof(struct ATM), 1, temp);
        } else {
            fwrite(&a, sizeof(struct ATM), 1, temp);
        }
    }
    fclose(file);
    fclose(temp);
    remove("atm.txt");
    rename("temp.txt", "atm.txt");
}

void update_user_as_withdraw(char *uname) {
    struct ATM a;
    FILE *file, *temp;
    file = fopen("atm.txt", "r+");
    temp = fopen("temp.txt", "a");
    while (fread(&a, sizeof(struct ATM), 1, file) == 1) {
        if (strcmp(a.username, uname) == 0) {
            int withdraw;
            printf("\nEnter amount to withdraw: ");
            scanf("%d", &withdraw);
            if (a.balance < withdraw) {
                printf("\nInsufficient balance to withdraw\n");
                delay(2000);
            } else {
                a.balance -= withdraw;
                printf("\nBalance is: %d\n", a.balance);
                print_receipt(&a, withdraw, "Withdrawal");
                for (int i = 4; i > 0; --i) {
                    a.transactions[i] = a.transactions[i - 1];
                }
                a.transactions[0].type = WITHDRAWAL;
                a.transactions[0].amount = -withdraw;
                fwrite(&a, sizeof(struct ATM), 1, temp);
            }
        } else {
            fwrite(&a, sizeof(struct ATM), 1, temp);
        }
    }
    fclose(file);
    fclose(temp);
    remove("atm.txt");
    rename("temp.txt", "atm.txt");
}

void transfer_amount(char *uname) {
    struct ATM a, b;
    FILE *file, *temp;
    file = fopen("atm.txt", "r+");
    temp = fopen("temp.txt", "a");
    printf("\nEnter recipient's username: ");
    char recipient[30];
    scanf("%s", recipient);
    int transfer_amount;
    printf("Enter amount to transfer: ");
    scanf("%d", &transfer_amount);
    int found_sender = 0, found_recipient = 0;
    while (fread(&a, sizeof(struct ATM), 1, file) == 1) {
        if (strcmp(a.username, uname) == 0) {
            found_sender = 1;
                        if (a.balance < transfer_amount) {
                printf("\nInsufficient balance to transfer\n");
                delay(2000);
                return;
            } else {
                a.balance -= transfer_amount;
                for (int i = 4; i > 0; --i) {
                    a.transactions[i] = a.transactions[i - 1];
                }
                a.transactions[0].type = TRANSFER;
                a.transactions[0].amount = -transfer_amount;
                fwrite(&a, sizeof(struct ATM), 1, temp);
            }
        } else if (strcmp(a.username, recipient) == 0) {
            found_recipient = 1;
            b = a;
            b.balance += transfer_amount;
            for (int i = 4; i > 0; --i) {
                b.transactions[i] = b.transactions[i - 1];
            }
            b.transactions[0].type = TRANSFER;
            b.transactions[0].amount = transfer_amount;

            fwrite(&b, sizeof(struct ATM), 1, temp);
        } else {
            fwrite(&a, sizeof(struct ATM), 1, temp);
        }
    }
    fclose(file);
    fclose(temp);
    remove("atm.txt");
    rename("temp.txt", "atm.txt");

    if (!found_sender) {
        printf("\nNo account found with sender's username :(\n");
        delay(2000);
    }
    if (!found_recipient) {
        printf("\nNo account found with recipient's username :(\n");
        delay(2000);
    }
    if (found_sender && found_recipient) {
        printf("\nAmount transferred successfully!\n");
        delay(2000);
    }
}

void quick_cash(char *uname) {
    struct ATM a;
    int quick_cash_amount, choice;
    FILE *file, *temp;
    file = fopen("atm.txt", "r+");
    temp = fopen("temp.txt", "a");
    printf("\nSelect quick cash amount:\n1. 20\n2. 50\n3. 100\n4. Other\nEnter your choice: ");
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            quick_cash_amount = 20;
            break;
        case 2:
            quick_cash_amount = 50;
            break;
        case 3:
            quick_cash_amount = 100;
            break;
        case 4:
            printf("Enter other amount: ");
            scanf("%d", &quick_cash_amount);
            break;
    }
    while (fread(&a, sizeof(struct ATM), 1, file) == 1) {
        if (strcmp(a.username, uname) == 0) {
            if (a.balance < quick_cash_amount) {
                printf("\nInsufficient balance for quick cash\n");
                delay(2000);
            } else {
                a.balance -= quick_cash_amount;
                printf("\nAmount dispensed: %d\n", quick_cash_amount);
                delay(2000);
                for (int i = 4; i > 0; --i) {
                    a.transactions[i] = a.transactions[i - 1];
                }
                a.transactions[0].type = QUICK_CASH;
                a.transactions[0].amount = -quick_cash_amount;

                fwrite(&a, sizeof(struct ATM), 1, temp);
                print_receipt(&a, quick_cash_amount, "Quick Cash Withdrawal");
            }
        } else {
            fwrite(&a, sizeof(struct ATM), 1, temp);
        }
    }
    fclose(file);
    fclose(temp);
    remove("atm.txt");
    rename("temp.txt", "atm.txt");
}

void change_pin(char *uname) {
    struct ATM a;
    FILE *file, *temp;
    int i;
    file = fopen("atm.txt", "r+");
    temp = fopen("temp.txt", "a");
    while (fread(&a, sizeof(struct ATM), 1, file) == 1) {
        if (strcmp(a.username, uname) == 0) {
            char new_pin[5];
            printf("\nEnter new 4-digit PIN: ");
            for (i = 0; i < 4; ++i) {
                new_pin[i] = getch();
                printf("*");
            }
            new_pin[i] = '\0';
            strcpy(a.password, new_pin);

            fwrite(&a, sizeof(struct ATM), 1, temp);
            printf("\nPIN changed successfully!\n");
            delay(2000);
        } else {
            fwrite(&a, sizeof(struct ATM), 1, temp);
        }
    }
    fclose(file);
    fclose(temp);
    remove("atm.txt");
    rename("temp.txt", "atm.txt");
}

int search_specific_user(char *uname, char *pass) {
    struct ATM a;
    FILE *file;
    file = fopen("atm.txt", "r");
    while (fread(&a, sizeof(struct ATM), 1, file) == 1) {
        if (strcmp(a.username, uname) == 0 && strcmp(a.password, pass) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int search_all_user_to_display(char *uname) {
    struct ATM a;
    FILE *file;

    file = fopen("atm.txt", "r");
    while (fread(&a, sizeof(struct ATM), 1, file) == 1) {
        if (strcmp(a.username, uname) == 0) {
            show_data(&a);
            delay(2000);
            fclose(file);
            return 0;
        }
    }
    fclose(file);
    return 0;
}

void user_menu(char *uname) {
    int ch2, ch3;

    do {
        printf("\n~~~~~~~~~~~~~ MENU ~~~~~~~~~~~~~~\n");
        printf("1. Deposit\n2. Withdraw\n3. View Account\n");
        printf("4. Change PIN\n5. Transfer\n6. Quick Cash\n7. Print Mini Statement\n");
        printf("Enter your choice:");
        scanf("%d", &ch2);
        switch (ch2) {
            case 1:
                update_user_as_deposit(uname);
                break;
            case 2:
                update_user_as_withdraw(uname);
                break;
            case 3:
                search_all_user_to_display(uname);
                break;
            case 4:
                change_pin(uname);
                break;
            case 5:
                transfer_amount(uname);
                break;
            case 6:
                quick_cash(uname);
                break;
            case 7:
                print_mini_statement(uname);
                break;
        }
        printf("Do you want to continue\n1.Yes\t2.No\n");
        scanf("%d", &ch3);
    } while (ch3 != 2);
}

void atm_user() {
    char uname[30], pass[5], ch;
    int found = 0, i;
    while (1) {
        printf("\n~~~~~~~~~~ WELCOME ~~~~~~~~~~~~~~\n");
        printf("Enter option\na: to login as Admin\nu: to login as User\nc: to close\n");
        ch = getch();
        switch (ch) {
            case 'a': {
                printf("\nEnter details to login as Admin\nEnter password: ");
                for (i = 0; i < 4; i++) {
                    pass[i] = getch();
                    printf("*");
                }
                pass[i] = '\0';
                getch();
                printf("\n");
                if (strcmp(pass, "1234") == 0) {
                    delay(2000);
                    add_user();
                } else {
                    printf("\nDetails are incorrect! Please try again\n");
                    delay(2000);
                }
                break;
            }
            case 'u': {
                printf("\nEnter username: ");
                scanf("%s", uname);
                printf("Enter password: ");
                for (i = 0; i < 4; i++) {
                    pass[i] = getch();
                    printf("*");
                }
                pass[i] = '\0';
                delay(2000);
                printf("\n");
                found = search_specific_user(uname, pass);
                if (found) {
                    user_menu(uname);
                } else {
                    printf("\nDetails are incorrect! Please try again\n");
                    delay(2000);
                }
                break;
            }
            case 'c':
                return;
        }
    }
}

int main() {
    atm_user();
    return 1;
}
