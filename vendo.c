#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STORES 4
#define NAME_LEN 50

typedef struct product {
    int storeId;
    char name[NAME_LEN];
    float price;
    int stock;
} Product;

const char *storeFiles[MAX_STORES] = {
    "drinks.txt", "fruits.txt", "meals.txt", "desserts.txt"
};

const char *storeNames[MAX_STORES] = {
    "DRINKS", "FRUITS", "MEALS", "DESSERTS"
};

void initializeFiles();
void viewAndBuyProducts();
float getStudentMoney();
void updateInventory(float newBalance, const char *newOrderToken);
void viewInventory();

int main() {
    initializeFiles();
    int choice;
 
menu_start:
    system("cls"); 
    printf("============================================\n");
    printf("            RY N JAR CANTEEN                  \n");
    printf("============================================\n");
    printf("  [1] View & Buy Products\n");
    printf("  [2] Inventory (Money & Orders)\n");
    printf("  [3] Exit\n");
    printf("--------------------------------------------\n");
    printf(" Choice: ");
    
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n'); 
        goto menu_start;
    }
    printf("\n");

    switch (choice) {
        case 1: 
            viewAndBuyProducts(); 
            goto menu_start; 
        case 2: 
            viewInventory(); 
            goto menu_start; 
        case 3: 
            system("cls");
            printf("Thank you for using RY N JAR CANTEEN! \n"); 
            return 0;
        default: 
            printf("Invalid option. Please try again.\n");
            printf("Press Enter to continue...");
            while (getchar() != '\n'); 
            getchar();
            goto menu_start;
    }
    return 0;
}

void initializeFiles() {
    for (int i = 0; i < MAX_STORES; i++) {
        FILE *fp = fopen(storeFiles[i], "r");
        if (fp == NULL) {
            fp = fopen(storeFiles[i], "w");
            if (i == 0) {
                fprintf(fp, "1 Coke 20.0 10\n1 Sprite 20.0 10\n1 Beer 40.0 8\n1 Juice 15.0 10\n1 RedHorse 50.0 5\n");
            } else if (i == 1) {
                fprintf(fp, "2 Apple 10.0 10\n2 Orange 10.0 8\n2 Banana 15.0 5\n2 Mango 12.0 7\n2 Avocado 15.0 6\n");
            } else if (i == 2) {
                fprintf(fp, "3 ChickenJoy 25.0 10\n3 Hotdog 10.0 20\n3 Burger 30.0 20\n3 Spaghetti 10.0 20\n3 Pizza 35.0 5\n");
            } else if (i == 3) {
                fprintf(fp, "4 Cake 50.0 4\n4 IceCream 20.0 10\n4 Cookie 15.0 15\n4 MangoFloat 20.0 12\n4 Chocolate 30.0 6\n");
            }
            fclose(fp);
        } else {
            fclose(fp);
        }
    }

    FILE *fp = fopen("inventory.txt", "r");
    if (fp == NULL) {
        fp = fopen("inventory.txt", "w");
        fprintf(fp, "500.00\n"); 
        fclose(fp);
    } else {
        fseek(fp, 0, SEEK_END);
        if (ftell(fp) == 0) {
            fclose(fp);
            fp = fopen("inventory.txt", "w");
            fprintf(fp, "500.00\n");
        }
        fclose(fp);
    }
}

float getStudentMoney() {
    FILE *ifp = fopen("inventory.txt", "r");
    if (ifp == NULL) return 0.0;

    float money = 0.0;
    fseek(ifp, 0, SEEK_SET);
    if (fscanf(ifp, "%f", &money) != 1) {
        money = 500.0; 
    }
    fclose(ifp);
    return money;
}

void updateInventory(float newBalance, const char *newOrderToken) {
    FILE *ifp = fopen("inventory.txt", "r");
    if (ifp == NULL) return;

    char **savedOrders = NULL;
    char tempOrder[100];
    int orderCount = 0;

    float dummy;
    fseek(ifp, 0, SEEK_SET);
    if (fscanf(ifp, "%f", &dummy) == 1) {
        while (fscanf(ifp, "%s", tempOrder) != EOF) {
            savedOrders = realloc(savedOrders, (orderCount + 1) * sizeof(char *));
            savedOrders[orderCount] = malloc(strlen(tempOrder) + 1);
            strcpy(savedOrders[orderCount], tempOrder);
            orderCount++;
        }
    }
    fclose(ifp);

    ifp = fopen("inventory.txt", "w");
    if (ifp == NULL) return;

    fprintf(ifp, "%.2f\n", newBalance);

    for (int i = 0; i < orderCount; i++) {
        fprintf(ifp, "%s\n", savedOrders[i]);
        free(savedOrders[i]);
    }
    free(savedOrders);

    if (newOrderToken != NULL) {
        fprintf(ifp, "%s\n", newOrderToken);
    }

    fclose(ifp);
}

void viewAndBuyProducts() {
    int targetStore = -1;

    while (1) {
        store_select:
        system("cls"); 
        printf("============================================\n");
        printf("                 STORE MENU                  \n");
        printf("============================================\n");
        for (int i = 0; i < MAX_STORES; i++) {
            printf("  [%d] Store %d: %s\n", i + 1, i + 1, storeNames[i]);
        }
        printf("  [0] Go BACK to Main Menu\n");
        printf("--------------------------------------------\n");
        printf(" Enter Store Number (0-4): ");

        if (scanf("%d", &targetStore) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Use numbers.\n");
            printf("Press Enter to continue...");
            getchar();
            continue;
        }

        if (targetStore == 0) {
            return; 
        }

        if (targetStore < 1 || targetStore > MAX_STORES) {
            printf("Invalid Store Choice. Try again.\n");
            printf("Press Enter to continue...");
            while (getchar() != '\n');
            getchar();
            continue;
        }

        break; 
    }

    const char *selectedFile = storeFiles[targetStore - 1];
    FILE *fp = fopen(selectedFile, "r");
    if (fp == NULL) {
        printf("Error opening file for Store %d.\n", targetStore);
        printf("Press Enter to continue...");
        while (getchar() != '\n');
        getchar();
        return;
    }

    Product *list = NULL;
    int count = 0;
    Product temp;

    while (fscanf(fp, "%d %s %f %d", &temp.storeId, temp.name, &temp.price, &temp.stock) != EOF) {
        Product *tempList = realloc(list, (count + 1) * sizeof(Product));
        if (tempList == NULL) {
            printf("Memory allocation error.\n");
            free(list);
            fclose(fp);
            return;
        }
        list = tempList;
        list[count] = temp;
        count++;
    }
    fclose(fp);

    float currentMoney = getStudentMoney();

    system("cls"); 
    printf("\n============================================\n");
    printf("         WELCOME TO STORE %d: %s         \n", targetStore, storeNames[targetStore - 1]);
    printf("============================================\n");
    printf("CURRENT FUNDS: P%.2f\n", currentMoney);
    printf("--------------------------------------------\n");
    printf("%-5s %-15s %-12s %-8s\n", "NO.", "PRODUCT NAME", "PRICE", "STOCK");
    printf("--------------------------------------------\n");

    if (count == 0) {
        printf(" (This store is currently out of stock)\n");
    } else {
        for (int i = 0; i < count; i++) {
            // Displays a item selection number starting from 1
            printf(" [%d]  %-15s P%-11.2f (%02d)\n", i + 1, list[i].name, list[i].price, list[i].stock);
        }
    }
    printf("============================================\n");

    if (count == 0) {
        free(list);
        printf("Press Enter to return to Store Menu...");
        while (getchar() != '\n');
        getchar();
        goto store_select;
    }

    int itemNum;
    int qty;

buy_prompt:
    printf("\n--- Purchasing from Store %d ---\n", targetStore);
    printf("Enter Product Number (or type '0' to change stores): ");
    if (scanf("%d", &itemNum) != 1) {
        while (getchar() != '\n');
        printf(" Invalid input. Please enter a number.\n");
        goto buy_prompt;
    }

    if (itemNum == 0) {
        free(list);
        goto store_select;
    }

    // Check if chosen menu item number is valid within array bounds
    if (itemNum < 1 || itemNum > count) {
        printf(" Invalid Product Number. Try again.\n");
        goto buy_prompt;
    }

    // Match selected array item based on index
    int selectedIdx = itemNum - 1;

    printf("Enter Quantity (or type '0' to change stores): ");
    if (scanf("%d", &qty) != 1) {
        while (getchar() != '\n');
        goto buy_prompt;
    }

    if (qty == 0) {
        free(list);
        goto store_select; 
    }

    if (qty < 0) {
        printf(" Quantity cannot be negative.\n");
        goto buy_prompt;
    }

    float studentMoney = getStudentMoney();
    float totalCost = list[selectedIdx].price * qty;

    if (list[selectedIdx].stock < qty) {
        printf(" Transaction Failed: Not enough stock available.\n");
        goto buy_prompt;
    }
    else if (studentMoney < totalCost) {
        printf("\n===================================\n");
        printf("        TRANSACTION FAILED!        \n");
        printf("===================================\n");
        printf("Reason:           Insufficient Funds\n");
        printf("Student Money:    P%.2f\n", studentMoney);
        printf("%s Total Cost: P%.2f\n", list[selectedIdx].name, totalCost);
        printf("Shortage Amount:  P%.2f\n", totalCost - studentMoney);
        printf("===================================\n");
        goto buy_prompt;
    }
    else {
        float previousMoney = studentMoney;
        list[selectedIdx].stock -= qty;
        studentMoney -= totalCost;

        fp = fopen(selectedFile, "w");
        for (int j = 0; j < count; j++) {
            fprintf(fp, "%d %s %.2f %d\n", list[j].storeId, list[j].name, list[j].price, list[j].stock);
        }
        fclose(fp);

        char newOrder[100];
        sprintf(newOrder, "%s(x%d)(Store%d)", list[selectedIdx].name, qty, targetStore);
        updateInventory(studentMoney, newOrder);

        printf("\n===================================\n");
        printf("       PURCHASE SUCCESSFUL!        \n");
        printf("===================================\n");
        printf("Item:             %s (x%d)\n", list[selectedIdx].name, qty);
        printf("Initial Money:    P%.2f\n", previousMoney);
        printf("%s Total Cost: P%.2f\n", list[selectedIdx].name, totalCost);
        printf("-----------------------------------\n");
        printf("Remaining Money:  P%.2f\n", studentMoney);
        printf("===================================\n");
        
        printf("\nPress Enter to continue...");
        while (getchar() != '\n');
        getchar();
    }

    free(list); 
}

void viewInventory() {  
    system("cls"); 
    FILE *ifp = fopen("inventory.txt", "r");
    printf("============================================\n");
    printf("              STUDENT INVENTORY           \n");
    printf("============================================\n");

    if (ifp == NULL) {
        printf("Inventory records missing.\n");
        printf("\nPress Enter to return...");
        while (getchar() != '\n');
        getchar();
        return;
    }

    float money = 0.0;
    fseek(ifp, 0, SEEK_SET);
    
    if (fscanf(ifp, "%f", &money) == 1) {
        printf("CURRENT FUNDS : P%.2f\n", money);
        printf("--------------------------------------------\n");
        printf(" PURCHASED ITEMS:\n");
    }

    char item[100];
    int checkEmpty = 0;
    while (fscanf(ifp, "%s", item) != EOF) {
        printf("  - %s\n", item);
        checkEmpty = 1;
    }
    if (!checkEmpty) {
        printf("  (No items purchased yet)\n");
    }
    printf("============================================\n");
    fclose(ifp);

    int backChoice;

inventory_back:
    printf("\n [0] Go back to Menu: ");
    if (scanf("%d", &backChoice) != 1 || backChoice != 0) {
        while (getchar() != '\n');
        goto inventory_back;
    }
}