#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

//Declare a Structure for expense categories 
struct ExpenseCategory {
    char name[20];
    float amount;
    int priority;
};

//Structure for user information
struct User {
    char username[50];
    char password[50];
    char fullname[50];
};

//Initialize all categories (Categories where college students normally spend): amount & priority to be taken from user later
struct ExpenseCategory categories[7] = {
    {"Rent", 0.0f, 0},
    {"Groceries", 0.0f, 0},
    {"Insurance", 0.0f, 0},
    {"Education", 0.0f, 0},
    {"Entertainment", 0.0f, 0},
    {"Others", 0.0f, 0},
    {"Income", 0.0f, 0}
};

const int count = 7; //7 categories including Income

//input functions
bool fileExists(const char *filename); //Checks if the file exists
void input(); //Asks if new user or existing
void checkAccount(char check); //Checks if account exists or not
void existingUser(); //Reads the existing data if account exists
void newUser(); //Prompts for user data if a new user
void InputChoice(char filename[]); //asks user what they would like to do: edit, view analysis, or exit

//Functions for Data Manipulation
void getCategoryDetails(struct ExpenseCategory categories[], int count); //take amount and priority for each expense category
void sortCategories(struct ExpenseCategory categories[], int count); //sort expenses according to priority in ascending order
void saveToFile(struct ExpenseCategory categories[], int count, float totalExpenses, char fileName[]); //save the data in unique user file
void analyzeBudget(char fileName[]); //performs calculation based on budget and total expenses
void editExpenses(char fileName[]); //user can re-enter their amount and priority for every expense category
void generateReport(char fileName[]); //gives quick outlook on expenses sorted by priority stored in user file
//Main Function
int main() {
    printf("*-*-*-* Welcome to Orion! *-*-*-*\n");
    input(); //check if user has an account
    return 0;
}

//Checks if the user file exists
bool fileExists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}

//To take initial input of y/n from the user.
void input() {
    char check;
    printf("\nDo you already have an account? (Y/N): ");
    scanf("%c", &check);
    getchar(); //To consume newline
    checkAccount(check);
}

//To check if account already exists or not
void checkAccount(char check) {
    check = tolower(check);
    if (check == 'y') { //account already exists
        existingUser();
    } else if (check == 'n') {  //account doesn't exists
        newUser();
    } else {
        printf("Invalid input. Please try again.\n");
        input();
    }
}

//asks user what they would like to do: edit, view analysis, or exit
void InputChoice(char filename[]){ 
    printf("\nWhat would you like to do today?");
    printf("\n1. Edit expenses\n2. Generate Report\n3. Exit\n");
    printf("Enter your choice: ");
    int ActivityChoice;

    scanf("%d", &ActivityChoice);
    getchar();

    switch (ActivityChoice) {
        case 1:
            editExpenses(filename); //call function to edit previous user response 
            break;
        case 2:
            generateReport(filename); //gives quick outlook on expenses sorted by priority stored in user file
            break;
        case 3:
            printf("\nHave a great day!\n"); //exit
            exit(0);
        default:
            printf("\nInvalid choice.\n");
            return;
    }

    char choice;
    printf("\nWould you like to get personalized financial advice? (Y/N): ");
    scanf("%c", &choice);
    while (getchar() != '\n'); //consumes newline

    if (tolower(choice) == 'y') {
        printf("\nGetting personalized financial advice from Orion AI...\n\n");
        system("python ai_advice.py"); //integration of AI for personalized budget vs expense response
    }
    else if(tolower(choice) == 'n'){
        printf("\nHave a great day!\n");
    }
    else{
        printf("\nInvalid choice.\n");
        return;
    }
}

//Has an account
void existingUser() {
    char username[50], password[50], filename[54];
    struct User user;

    printf("\nEnter your username: ");
    fgets(username, sizeof(username), stdin);

    //strcspn finds new line character taken by fgets and replaces it with null character so that creating text file of that username is possible
    username[strcspn(username, "\n")] = '\0';

    strcpy(filename, username); //Copy username excluding new line
    strcat(filename, ".txt"); //Create a txt file for that user


    if (!fileExists(filename)) { //since every user has unique user file, is the file doesn't exist, user isn't authorized
        printf("Account does not exist.\n");
        input();
        return;
    }

    FILE *userfile = fopen(filename, "r");
    if (!userfile) {
        printf("Error opening user file.\n");
        return;
    }

    //Read user data from user file
    fscanf(userfile, "%s %s %s", user.username, user.password, user.fullname);
    fclose(userfile);

    printf("\nEnter your password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0'; //replace new line with null character for accurate comparison

    if (strcmp(password, user.password) != 0) { //if stored password and user-input password doesn't match, don't allow access
        printf("Incorrect password.\n");
        input();
        return;
    }

    //if program reaches here, password has matched, access allowed
    printf("\nWelcome to Orion, %s!\n", user.fullname);
    InputChoice(filename); //asks user what they want to do today
    
}

//Doesn't have an account: prompt to create a new one
void newUser() {
    char username[50], filename[54];
    struct User user;

    printf("\nTo create your Orion Account, please enter the following information: ");


    while (1) {
        printf("\nChoose a username: ");
        fgets(username, sizeof(username), stdin);

        username[strcspn(username, "\n")] = '\0';

        strcpy(filename, username);
        strcat(filename, ".txt");
        if (fileExists(filename)) {//if the file exists, another user has already choosen that username, so don't allow replacing
            printf("This username is taken. Please try something else.\n");
        } else { //file doesn't exist so username is safe to proceed
            break;
        }
    }

    //Get other user details
    strcpy(user.username, username);

    printf("Enter your password: ");
    fgets(user.password, sizeof(user.password), stdin);
    user.password[strcspn(user.password, "\n")] = '\0';

    printf("Enter your full name: ");
    fgets(user.fullname, sizeof(user.fullname), stdin);
    user.fullname[strcspn(user.fullname, "\n")] = '\0';

    //Save user info to user file
    FILE *userfile = fopen(filename, "w");
    if (!userfile) {
        printf("Error creating user file.\n");
        return;
    }

    fprintf(userfile, "%s %s %s\n\n", user.username, user.password, user.fullname);
    fclose(userfile);

    printf("\nAccount created successfully!\n");

    //Now get expense details for the new user
    getCategoryDetails(categories, count);

    //sort the given input based on priority
    sortCategories(categories, count - 1);

    float totalExpenses = 0;
    for (int i = 0; i < count - 1; i++) {
        totalExpenses += categories[i].amount;
    }//sum up all expenses expect the last category (income)

    //save all data to user file
    saveToFile(categories, count, totalExpenses, filename);

    printf("\nExpenditure and budget tracking complete... Data saved to %s.\n", filename);
    char choice;
    InputChoice(filename);//ask what the user wants to do next
}

//Function to get details for each category
void getCategoryDetails(struct ExpenseCategory categories[], int count) {
    printf("\nEnter details for each category:\n");
    for (int i = 0; i < count; i++) {
        printf("\nFor %s:\n", categories[i].name);
        if (i < count-1) {
            //prompts user to enter monthly spending in a category
            printf("Monthly spending($): ");
            scanf("%f", &categories[i].amount);
            
            //checks if the entered number is valid
            while (categories[i].amount < 0) {
                printf("Please enter a non-negative amount: $");
                scanf("%f", &categories[i].amount);
            }
            
            //prompts user to enter priority for that category (only for expenses)
            printf("Priority (1-%d, 1 = highest): ", count-1);
            scanf("%d", &categories[i].priority);
                        
            //checks if the entered number is valid
            while (categories[i].priority < 1 || categories[i].priority > count-1) {
                printf("Please enter a valid priority (1-%d): ", count-1);
                scanf("%d", &categories[i].priority);
            }
        } 
        else {
            printf("Monthly income($): "); //This is the budget and won't be included in total expenditure
            scanf("%f", &categories[i].amount);
            
            //checks if the entered number is valid
            while (categories[i].amount <= 0) {
                printf("Please enter a positive income amount: $");
                scanf("%f", &categories[i].amount);
            }
            categories[i].priority = count; //it is declared last priority because we don't want it while sorting expenses
        }
    }
}

//selection sort method (ascending order)
void sortCategories(struct ExpenseCategory categories[], int count) {
    for (int i = 0; i < count; i++) {
        int smallest_priority = i; //Assume the current priority is the smallest
        
        //search for the smaller priority in the unsorted part
        for (int j = i + 1; j < count; j++) {
            if (categories[j].priority < categories[smallest_priority].priority) { 
                smallest_priority = j; //if smaller priority is found, update smallest_priority
            }
        }
        
        if (smallest_priority != i) { //if initially assumed priority was not smallest, swap
            struct ExpenseCategory temp = categories[i]; //temporary space to store current priority
            categories[i] = categories[smallest_priority]; //current priority replaced by smaller priority
            categories[smallest_priority] = temp; //in the initial place of smaller priority, current priority is stored
        }
    }
}

//function to save data to a unique user file
void saveToFile(struct ExpenseCategory categories[], int count, float totalExpenses, char fileName[]) {
    FILE *file = fopen(fileName, "a");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    //The total Budget is the income provided by the user
    float totalBudget = categories[count - 1].amount;

    fprintf(file, "\n--- Monthly Expenditure and Budget Track ---\n\n");
    fprintf(file, "Total Budget (Income): $%.2f\n\n", totalBudget);
    fprintf(file, "Categories of Expenditure:\n");

    for (int i = 0; i < count - 1; i++) {//print the categories, amount, and priority of expenses in the unique user file created using the username
        fprintf(file, "%s: $%.2f (Priority %d)\n", categories[i].name, categories[i].amount, categories[i].priority);
    }

    fprintf(file, "\nTotal Expenses: $%.2f\n", totalExpenses);
    fclose(file);

    FILE *pyfile = fopen("ai_input.txt", "w"); //AI integration for personalized response
    if (!pyfile) {
        printf("Error creating AI input file!\n");
        return;
    }

    fprintf(pyfile, "Income: %.2f\n", totalBudget); //print all the data in the py file for Gemini AI to access
    for (int i = 0; i < count - 1; i++) {
        fprintf(pyfile, "%s: %.2f\n", categories[i].name, categories[i].amount);
    }
    fprintf(pyfile, "Total Expenditure: %.2f\n", totalExpenses);
    fclose(pyfile);
}

//Analyze budget vs. expenses from file
void analyzeBudget(char fileName[]) {
    FILE *analyzeFile = fopen(fileName, "r");
    if (!analyzeFile) {
        printf("Error: File not found!\n");
        return;
    }
    char line[250];
    float totalBudget = 0, totalExpenses = 0;

    while (fgets(line, sizeof(line), analyzeFile)) {//read file line by line
        if (sscanf(line, "Total Budget (Income): $%f", &totalBudget) == 1) continue; //if total budget is encountered, store the float value and go to next line
        if (sscanf(line, "Total Expenses: $%f", &totalExpenses) == 1) continue; //if total expenses is encountered, store the float value and continue
    }
    fclose(analyzeFile);

    //Calculate the difference and return simple mathematical analysis
    float difference = totalBudget - totalExpenses;

    printf("\n=== Budget Analysis ===\n");
    printf("Total Budget (Income): $%.2f\n", totalBudget);
    printf("Total Expenses: $%.2f\n", totalExpenses);

    if (difference >= 0) {
        printf("Within budget! Remaining: $%.2f\n", difference);
    } else {
        printf("Overspending by $%.2f\n", -difference);
    }
}

//to edit expenses, we're going to call getCategoryDetails and sortCategories functions again but we will use the same file for the user
void editExpenses(char fileName[]) {
    printf("\nEditing your expense data...\n");
    getCategoryDetails(categories, count);
    sortCategories(categories, count - 1);
    float totalExpenses = 0;
    for (int i = 0; i < count - 1; i++) {
        totalExpenses += categories[i].amount;
    }//calculate total expenses
    saveToFile(categories, count, totalExpenses, fileName);
    printf("\nExpenses updated.\n");
}

//this function scans information stored in unique user file and prints it to the terminal
void generateReport(char fileName[]) {
    FILE *file = fopen(fileName, "r");
    
    char line[256];

    // Discard the first line that stores user name and password
    if (fgets(line, sizeof(line), file) == NULL) {
        // File is empty or error occurred
        fclose(file);
        return;
    }
    //start printing from second line and print all data stored in the file
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    //call analyze budget function that runs simple mathematical calculation
    analyzeBudget(fileName);
    fclose(file);
}