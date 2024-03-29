# ATM-Simulator
# ATM System Description

This C program simulates an ATM system with functionalities for user authentication, account management, and transaction processing. Below is a detailed description of its functionalities:

## Structures

- **Transaction**: Defines a structure to represent a transaction with its type (`enum TransactionType`) and amount.
- **ATM**: Represents an ATM user account with fields for username, password, balance, and transaction history.

## Functions

- **`get_timestamp()`**: Retrieves and prints the current timestamp.
- **`delay(int milliseconds)`**: Delays the execution of the program for a specified number of milliseconds.
- **`get_data(struct ATM *a)`**: Collects user data such as username, password, and initial balance.
- **`update_transaction_history(struct ATM *a, enum TransactionType type, int amount)`**: Updates the transaction history of an account.
- **`show_data(struct ATM *a)`**: Displays the details of an account.
- **`print_receipt(struct ATM *a, int amount, const char *transaction)`**: Prints a receipt for a transaction.
- **`print_mini_statement(char *uname)`**: Prints a mini statement of recent transactions for a user.
- **`add_user()`**: Adds a new user account to the ATM system.
- **`update_user_as_deposit(char *uname)`**: Updates user account balance after a deposit transaction.
- **`update_user_as_withdraw(char *uname)`**: Updates user account balance after a withdrawal transaction.
- **`transfer_amount(char *uname)`**: Transfers funds between two user accounts.
- **`quick_cash(char *uname)`**: Facilitates quick cash withdrawals for a user.
- **`change_pin(char *uname)`**: Allows a user to change their PIN.
- **`search_specific_user(char *uname, char *pass)`**: Searches for a specific user account based on username and password.
- **`search_all_user_to_display(char *uname)`**: Searches for a user account to display their details.
- **`user_menu(char *uname)`**: Displays a menu of options for a logged-in user.
- **`atm_user()`**: Implements the user interface for the ATM system.

## Main Function

- **`main()`**: Initiates the ATM system by calling the `atm_user()` function.

Overall, this program provides basic ATM functionalities such as account management, transaction processing, and user authentication. Users can perform operations like deposit, withdrawal, balance inquiry, transfer, quick cash withdrawal, and PIN change. Additionally, administrators can add new user accounts to the system.
