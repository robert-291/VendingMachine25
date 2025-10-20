#include <stdio.h>
#include <stdlib.h>  // for rand, srand
#include <time.h>    

// GLOBAL VARIABLES AND CONSTANTS

// The variables for product prices and quantities (values given by myself)
float price_A = 2.25;
int quantity_A = 7;

float price_B = 3.00;
int quantity_B = 7;

float price_C = 4.50;
int quantity_C = 7;

// Integer admin password variable (used to access admin mode)
int password = 9999;

// Symbolic constant MIN for minimum quantity alert
// When quantity of any product becomes less or equal to MIN, alert is sent
#define MIN 5

// variable for total amount of sales
// Tracks total sales since operator last reset it to zero
// Accumulates all successful purchases
float total_amount = 0.0;


// FUNCTION PROTOTYPES

// Function prototypes (needed because main comes before function definitions)
// These tell the compiler about functions that are defined later in the code

void displayProducts();                                   // Displays available products (Part 1.a)
int getUserSelection();                                   // Gets user's product choice (Part 1.b)
int confirmSelection(int selection);                      // Confirms user's selection (Part 1.c)
int processPayment(float price);                          // Handles payment with coins (Part 1.d)
void updateTransaction(int selection, float amount_paid); // Updates quantities and sales (Part 1.e)
void checkLowStock(int selection);                        // Checks and alerts low inventory (Part 1.f)

// ADMIN MODE FUNCTION PROTOTYPES (Part 2.a and 2.b)
int enterAdminMode();                 // Prompts for password; returns 1 if ok, 0 otherwise (Part 2.a)
void adminMenu();                     // Infinite admin menu loop after successful login (Part 2.b)
void adminReplenishProducts();        // 2.b.1: Randomly reset quantities in [1..20]
void adminChangePrices();             // 2.b.2: Allow operator to change prices 
void adminDisplayTotalSales();        // 2.b.3: Show total sales and optionally reset to zero
void adminDisplayQuantities();        // 2.b.4: Show quantities for all products


// MAIN FUNCTION

int main() {
    // Variable declarations for main function
    int main_choice;        // Stores user's choice from main menu (1, 2, or 3)
    int selection;          // Stores product selection (0-3)
    int confirm_result;     // Stores result from confirmation (0 = back, 1 = proceed)
    int payment_result;     // Stores result from payment (amount paid or -1 if cancelled)
    
    // Main menu embedded in infinite loop
    // Loop continues until user selects option 3 (Exit) or program terminates
    while (1) {
        
        printf("\n"); // Empty row 
        
        // Display instructions to the user
        printf("Please choose one of the following (a number from 1 to 3):\n");
        
        // Display main menu
        printf("1 - Purchase a product\n");
        printf("2 - Admin mode\n");
        printf("3 - Exit\n");
        
        // Read user's choice and store in main_choice variable
        scanf("%d", &main_choice);
        
        
        // Option 1: Purchase a product by the consumer

        if (main_choice == 1) {
            
            printf("\n");
            
            // Purchase menu embedded in infinite loop
            // Allows user to make multiple purchases without returning to main menu
            // Loop breaks when user cancels (enters 0) or completes a purchase
            while (1) {
                // Part 1.a: Display products with prices
                // Shows all available products and their prices to the user
                displayProducts();
                
                // Part 1.b: Get user selection
                // Prompts user to select a product (1-3) or cancel (0)
                selection = getUserSelection();
                
                // If user enters 0, cancel purchase and return to main menu
                // This breaks the purchase loop and goes back to main menu
                if (selection == 0) {
                    break;  // Exit purchase loop
                }
                
                // Part 1.c: Confirm selection
                // Shows selected product and asks user to confirm or go back
                confirm_result = confirmSelection(selection);
                
                // If user enters -1 in confirmation, loop back to 1.a
                // confirm_result = 0 means user wants to go back
                if (confirm_result == 0) {
                    continue;  // Skip rest of loop and start again from displayProducts
                }
                
                // Part 1.d: Process payment for the selected product
                // First, get the price of the selected product
                float product_price;  // Variable to store price of selected product
                
                // Determine which product was selected and get its price
                if (selection == 1) {
                    product_price = price_A;  // User selected Product A
                } else if (selection == 2) {
                    product_price = price_B;  
                } else if (selection == 3) {
                    product_price = price_C;  
                }
                
                // Call processPayment function to handle coin insertion
                // Function handles the entire payment process:
                // - Accepts coins one at a time
                // - Validates coin denominations
                // - Tracks amount paid and outstanding balance
                // - Calculates and returns change
                // Returns the total amount paid, or -1 if purchase was cancelled
                payment_result = processPayment(product_price);
                
                // If payment was cancelled (user returned -1), go back to product display
                // This allows user to select a different product or cancel entirely
                if (payment_result == -1) {
                    continue;  // Loop back to displayProducts
                }
                
                // Part 1.e: Update transaction (total_amount and quantity)
                // Also prints transaction summary to the consumer
                // This function:
                // - Decrements product quantity by 1
                // - Adds product price to total_amount
                // - Prints detailed transaction summary
                updateTransaction(selection, payment_result);
                
                // Part 1.f: Check if product quantity is low and send alert if needed
                // If quantity <= MIN (5), alerts operator to replenish stock
                checkLowStock(selection);
                
                // After successful purchase, exit purchase loop and return to main menu
                // User can then choose to make another purchase or exit
                break;
            }
        }
       
        // Option 2: Admin mode for the vending machine operator
       
        else if (main_choice == 2) {
            // Part 2.a: Prompt for admin password; if incorrect, print message and exit Admin mode
            if (enterAdminMode()) {
                // Part 2.b: Show Admin menu in an infinite loop with required options
                adminMenu();
            }
            // After returning from admin (success or failed), continue main loop
        }
        
        // Option 3: Exit
        
        else if (main_choice == 3) {
            // Exit the infinite loop and end the program
            // This is the normal way to terminate the vending machine program
            break;  // Exits the while(1) loop
        }
    }
    
    // Return 0 to indicate successful program termination
    return 0;
}


// PART 1.a: DISPLAY PRODUCTS FUNCTION

// Display all products with name, price, and input number
// This function shows the user what products are available for purchase
// Parameters: None (uses global price variables)
// Returns: void (nothing)
void displayProducts() {
    // Display message before product list
    printf("Available products:\n");
    
    printf("\n");
    
    // Display header line to explain what each column represents
    printf("    Name    Price (AED)\n");
    
    // Display Product A with its selection number, name, and price
    // %-8.2f formats the price: left-aligned, 8 spaces total, 2 decimal places
    printf("1 - A       %-8.2f\n", price_A);
    printf("2 - B       %-8.2f\n", price_B);
    printf("3 - C       %-8.2f\n", price_C);
    
    printf("\n");
    
    // Display cancel option
    // User can enter 0 to cancel purchase and return to main menu
    printf("0 - Cancel\n");
    
    printf("\n");
    
    // Display message after product list
    // Prompts user to input their selection
    printf("Enter your selection number:\n");
}


// PART 1.b: GET USER SELECTION FUNCTION

// Get user's product selection
// Reads user input and returns the selection number
// Parameters: None
// Returns: int - the selection number (0 to cancel, 1-3 for products)
int getUserSelection() {
    // Variable to store user's selection
    int selection;  // Will hold values 0-3 based on user input
    
    // Prompt user for input
    // scanf reads integer from standard input and stores in selection variable
    scanf("%d", &selection);
    
    printf("\n");
    
    // If user enters 0, they want to cancel purchase
    // Return 0 immediately to signal cancellation to main function
    if (selection == 0) {
        return 0;
    }
    
    // Return the selection to the calling function (main)
    // This will be a value from 1-3 representing the selected product
    return selection;
}


// PART 1.c: CONFIRM SELECTION FUNCTION

// Confirm selection by printing product name and price
// Shows user what they selected and asks for confirmation
// Parameters: int selection - the product number (1, 2, or 3)
// Returns: int - 0 if user wants to go back (-1 input), 1 to proceed
int confirmSelection(int selection) {
    // Variable declarations for this function
    char product_name;       // Stores product name
    float product_price;     // Stores price of selected product
    int user_input;          // Stores user's confirmation choice (1 or -1)
    
    // Get product name and price based on selection number
    // Uses if-else chain to match selection with corresponding product
    if (selection == 1) {
        product_name = 'A';      
        product_price = price_A; 
    } else if (selection == 2) {
        product_name = 'B';      
        product_price = price_B; 
    } else if (selection == 3) {
        product_name = 'C';      
        product_price = price_C; 
    }
    
    // Print product name and price to confirm user's selection
    // Format: character (product name), spaces, price with 2 decimals
    // %-8.2f ensures consistent formatting (8 spaces, left-aligned, 2 decimals)
    printf("%c       %-8.2f\n", product_name, product_price);
    
    printf("\n");
    
    // Display confirmation prompt with two options
    printf("Confirm your selection:\n");
    printf("1  - Yes\n");    // Proceed with this product
    printf("-1 - Back\n");   // Go back to product selection
    
    // Get user input for confirmation
    // User enters 1 to confirm or -1 to go back
    scanf("%d", &user_input);
    
    printf("\n");
    
    // If user inputs -1, they want to go back to product display
    // Return 0 to signal main function to loop back to 1.a
    if (user_input == -1) {
        return 0;  
    }
    
    // Otherwise user confirmed (entered 1)
    // Return 1 to signal main function to proceed with payment
    return 1;  
}


// PART 1.d: PROCESS PAYMENT FUNCTION

// Process payment by accepting coins until price is covered
// Handles coin validation, payment tracking, and change calculation
// Parameter: float price - the price of the selected product
// Returns: int - total amount paid by user, or -1 if purchase was cancelled
int processPayment(float price) {
    // Variable declarations with detailed explanations
    float amount_paid = 0.0;         // Tracks total amount user has inserted so far
    float outstanding = price;       // Tracks how much user still needs to pay
    float coin;                      // Stores each coin denomination inserted by user
    int cancel_choice;               // Stores user choice if they want to cancel after invalid coin
    
    // Display message prompting user to insert coins
    printf("Please insert coins (1, 0.5, or 0.25 AED):\n");
    
    printf("\n");
    
    // Loop continues until the full price is paid
    // outstanding > 0.01 instead of > 0 to avoid floating-point precision issues

    while (outstanding > 0.01) {
        // Display the amount still remaining to be paid
        // Keeps user informed of how much more they need to insert
        printf("Outstanding amount: %-8.2f AED\n", outstanding);
        
        // Prompt user to insert a coin
        printf("Insert coin:\n");
        
        // Read the coin value entered by user
        scanf("%f", &coin);
        
        printf("\n");
        
        // Check if the coin is valid (must be 1, 0.5, or 0.25 AED)
       
        if (coin == 1.0 || coin == 0.5 || coin == 0.25) {
            // Valid coin: add it to the total amount paid
            amount_paid += coin;
            
            // Subtract coin value from outstanding amount
            // This reduces the remaining balance user needs to pay
            outstanding -= coin;
            
            // If outstanding becomes negative, user overpaid
            // Set to zero to exit loop 
            if (outstanding < 0) {
                outstanding = 0;  // Set to zero to exit loop
            }
        }
        else {
            // Invalid coin entered (not 1, 0.5, or 0.25)
            // Inform user and give option to cancel
            printf("Invalid coin. Please insert 1, 0.5, or 0.25 AED only.\n");
            
            printf("\n");
            
            // Give user option to cancel purchase or continue trying
            printf("Would you like to cancel the purchase?\n");
            printf("1  - Continue\n");   
            printf("-1 - Cancel\n");     
            
            // Read user's choice
            scanf("%d", &cancel_choice);
            
            printf("\n");
            
            // If user chooses to cancel
            if (cancel_choice == -1) {
                // Calculate change to return 
                float change = amount_paid;
                
                // Print cancellation message
                printf("Purchase cancelled.\n");
                
                // Print message with amount of change to collect
                // User gets back all money they inserted
                printf("Please collect your change: %-8.2f AED\n", change);
                
                printf("\n");
                
                // Return -1 to signal main function that purchase was cancelled
                return -1;
            }
            // If user chooses to continue (entered 1), loop continues normally
            // They can try inserting a valid coin
        }
    }
    
    // Payment loop completed - user has paid enough
    // Calculate change if user overpaid
    float change = amount_paid - price;
    
    // Print success message telling user to collect their product
    printf("Payment successful!\n");
    printf("Please collect your product.\n");
    
    // If there is change (overpayment), print the change amount
    // Only print if change is more than 0.01 (to avoid floating-point precision issues)
    if (change > 0.01) {
        printf("Change to collect: %-8.2f AED\n", change);
    }
    
    printf("\n");
    
    // Return the total amount paid by the user
    // This will be used by updateTransaction to print transaction summary
    return amount_paid;
}


// PART 1.e: UPDATE TRANSACTION FUNCTION

// Update transaction details after successful payment
// Updates total_amount and quantity, and prints transaction summary
// Parameter: int selection - the product number (1, 2, or 3)
// Parameter: float amount_paid - the total amount the customer paid
// Returns: void (nothing)
void updateTransaction(int selection, float amount_paid) {
    // Variable declarations
    char product_name;       // Stores product name for display
    float product_price;     // Stores price of purchased product
    float change;            // Stores change amount (overpayment)
    
    // Determine product details based on selection
    // Also decrement the quantity of the purchased product
    if (selection == 1) {
        product_name = 'A';           // Product A was purchased
        product_price = price_A;      // Get price of Product A
        quantity_A--;                 // Decrease quantity of product A by 1
    } else if (selection == 2) {
        product_name = 'B';          
        product_price = price_B;      
        quantity_B--;                 
    } else if (selection == 3) {
        product_name = 'C';           
        product_price = price_C;      
        quantity_C--;                
    }
    
    // Add the product price to the total sales amount
    // I add the price, not the amount_paid, because change is returned
    // Only the actual product price counts as profit
    total_amount += product_price;
    
    // Calculate change (amount paid minus product price)
    // This is how much overpayment the customer made
    change = amount_paid - product_price;
    
    // Print transaction summary to the consumer
    // This provides a detailed receipt of the purchase
    printf("Transaction Summary:\n");
    printf("Product purchased: %c\n", product_name);
    printf("Price:             %-8.2f AED\n", product_price);
    printf("Amount paid:       %-8.2f AED\n", amount_paid);
    
    // Only print change if there is any
    // Check if change > 0.01 to avoid floating-point precision issues
    if (change > 0.01) {
        printf("Change:            %-8.2f AED\n", change);
    }
    
    printf("\n");
}


// PART 1.f: CHECK LOW STOCK FUNCTION

// Check if product quantity is low and send alert
// Sends alert to operator if quantity <= MIN (5 units)
// Parameter: int selection - the product number (1, 2, or 3)
// Returns: void (nothing)
void checkLowStock(int selection) {
    // Variable declarations
    char product_name;       // Stores product name for alert message
    int current_quantity;    // Stores current quantity of product
    
    // Determine product details based on selection
    // Get the current quantity after the purchase was made
    if (selection == 1) {
        product_name = 'A';             
        current_quantity = quantity_A;   
    } else if (selection == 2) {
        product_name = 'B';              
        current_quantity = quantity_B;   
    } else if (selection == 3) {
        product_name = 'C';              
        current_quantity = quantity_C;   
    }
    
    // Check if current quantity is less than or equal to MIN threshold
    // This alerts operator when stock is running low
    if (current_quantity <= MIN) {
        // Send alert to operator with detailed information
        printf("ALERT: Product %c is running low!\n", product_name);
        
        // Print current quantity so operator knows how urgent it is
        printf("Current quantity: %d\n", current_quantity);
        
        // Remind operator to replenish stock
        printf("Please replenish stock.\n");
        
        printf("\n");
    }
}


// PART 2.a: ENTER ADMIN MODE (PASSWORD CHECK)

// Prompt for admin password; if incorrect, print a message and exit Admin mode
// Parameters: none
// Returns: int - 1 if password correct; 0 otherwise
int enterAdminMode() {
    // Variable for typed password
    int typed;

    // Prompt the user to input the admin password
    printf("\n");
    printf("Enter admin password:\n");

    // Read the password
    scanf("%d", &typed);

    printf("\n");

    // Check password
    if (typed != password) {
        // Incorrect password: print relevant message and exit Admin mode
        printf("Incorrect password. Exiting Admin mode.\n");
        printf("\n");
        return 0;  // Not allowed into Admin menu
    }

    // Correct password
    printf("Access granted.\n");

    return 1;      // Proceed to Admin menu
}


// PART 2.b: ADMIN MENU (INFINITE LOOP)

// Display the Admin menu and dispatch to dedicated operations
// Parameters: none
// Returns: void
void adminMenu() {
    // Variable for admin menu choice
    int admin_choice;

    // Infinite loop until operator chooses 0 to exit Admin mode
    while (1) {
        // Empty row for readability
        printf("\n");

        // Display Admin menu options
        printf("Admin Menu - Choose an option (0 to exit):\n");
        printf("1 - Replenish Products\n");
        printf("2 - Change Product Prices\n");
        printf("3 - Display the total sale amount\n");
        printf("4 - Display number of items of each product\n");
        printf("0 - Exit Admin Mode\n");

        // Read operator choice
        scanf("%d", &admin_choice);

        printf("\n");

        // Dispatch based on choice
        if (admin_choice == 1) {
            // 2.b.1: Replenish Products
            adminReplenishProducts();
        } else if (admin_choice == 2) {
            // 2.b.2: Change Product Prices
            adminChangePrices();
        } else if (admin_choice == 3) {
            // 2.b.3: Display total sale amount (with option to reset)
            adminDisplayTotalSales();
        } else if (admin_choice == 4) {
            // 2.b.4: Display quantities of all products
            adminDisplayQuantities();
        } else if (admin_choice == 0) {
            // Exit Admin Mode
            printf("Exiting Admin mode.\n");
            printf("\n");
            break;  // Leave admin loop, return to main menu
        } else {
            // Unexpected input (spec assumes valid inputs)
            printf("Invalid option.\n");
            printf("\n");
        }
    }
}


// PART 2.b.1: REPLENISH PRODUCTS

// Reset randomly the quantity of each product in the range [1..20]
// Parameters: none
// Returns: void
void adminReplenishProducts() {
    // Seed the random generator once per call using current time
    srand((unsigned int)time(NULL));

    // Random quantity in [1..20] for each product
    quantity_A = (rand() % 20) + 1;
    quantity_B = (rand() % 20) + 1;
    quantity_C = (rand() % 20) + 1;

    // Print confirmation with current quantities
    printf("Products replenished.\n");
    printf("A quantity: %-8d\n", quantity_A);
    printf("B quantity: %-8d\n", quantity_B);
    printf("C quantity: %-8d\n", quantity_C);
    printf("\n");
}


// PART 2.b.2: CHANGE PRODUCT PRICES

// Give the operator the option to change the price of every product and update it
// Parameters: none
// Returns: void
void adminChangePrices() {
    // Temporary new prices
    float newA, newB, newC;

    // Show current prices
    printf("Current prices:\n");
    printf("A: %-8.2f AED\n", price_A);
    printf("B: %-8.2f AED\n", price_B);
    printf("C: %-8.2f AED\n", price_C);
    printf("\n");

    // Prompt for new prices (spec assumes valid input)
    printf("Enter new price for A (AED):\n");
    scanf("%f", &newA);
    printf("Enter new price for B (AED):\n");
    scanf("%f", &newB);
    printf("Enter new price for C (AED):\n");
    scanf("%f", &newC);
    printf("\n");

    // Update global prices
    price_A = newA;
    price_B = newB;
    price_C = newC;

    // Confirm updates
    printf("Prices updated.\n");
    printf("A: %-8.2f AED\n", price_A);
    printf("B: %-8.2f AED\n", price_B);
    printf("C: %-8.2f AED\n", price_C);
    printf("\n");
}


// PART 2.b.3: DISPLAY TOTAL SALE AMOUNT

// Print total_amount and prompt to reset it to zero; if yes, reset and remind to collect money
// Parameters: none
// Returns: void
void adminDisplayTotalSales() {
    // Show current total sales amount
    printf("Total sale amount: %-8.2f AED\n", total_amount);
    printf("\n");

    // Ask if operator wants to reset to zero
    int choice;
    printf("Reset total sale amount to zero?\n");
    printf("1  - Yes\n");
    printf("-1 - No\n");
    scanf("%d", &choice);
    printf("\n");

    // If yes, reset and print reminder
    if (choice == 1) {
        total_amount = 0.0f;
        printf("Total sale amount reset to zero.\n");
        printf("Reminder: Please collect the money from the machine.\n");
        printf("\n");
    }
}


// PART 2.b.4: DISPLAY NUMBER OF ITEMS OF EACH PRODUCT

// Print out all products on sale along with the quantity of each, including sold-out
// Parameters: none
// Returns: void
void adminDisplayQuantities() {
    // Display header
    printf("Product   Quantity\n");
    // Print all three products, even if zero
    printf("A         %-8d\n", quantity_A);
    printf("B         %-8d\n", quantity_B);
    printf("C         %-8d\n", quantity_C);
    printf("\n");
}

