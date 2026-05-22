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

void initializeFiles();
void viewAndBuyProducts();
float getStudentMoney();
void updateInventory(float newBalance, const char *newOrderToken);
void viewInventory();

int main() {
    initializeFiles();
    int choice;
 
    // Label for restarting the main menu loop
menu_start:
    printf("\n============================================\n");
    printf("            RICO VENDING MACHINE          \n");
    printf("============================================\n");
    printf("  [1] View & Buy Products\n");
    printf("  [2] Inventory (Money & Orders)\n");
    printf("  [3] Exit\n");
    printf("--------------------------------------------\n");
    printf(" Choice: ");
    
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n'); // Clear input buffer
        goto menu_start;
    }
    printf("\n");

    switch (choice) {
        case 1: 
            viewAndBuyProducts(); 
            goto menu_start; // Return to menu after exiting view/buy block
        case 2: 
            viewInventory(); 
            goto menu_start; // Return to menu after checking inventory
        case 3: 
            printf("Thank you! \n"); 
            return 0;
        default: 
            printf("Invalid option. Please try again.\n");
            goto menu_start;
    }
    return 0;
}

void initializeFiles() {
    FILE *fp = fopen("products.txt", "r");
    if (fp == NULL) {
        fp = fopen("products.txt", "w");
        fprintf(fp, "1 Coke 20.0 10\n1 Sprite 20.0 10\n1 Beer 40.0 8\n1 Juice 15.0 10\n1 RedHorse 50.0 5\n");
        fprintf(fp, "2 Apple 10.0 10\n2 Orange 10.0 8\n2 Ates 15.0 5\n2 Mango 12.0 7\n2 Avocado 15.0 6\n");
        fprintf(fp, "3 ChickenJoy 25.0 10\n3 Hotdog 10.0 20\n3 Burger 30.0 20\n3 Spaghetti 10.0 20\n3 Pizza 35.0 5\n");
        fprintf(fp, "4 Cake 50.0 4\n4 IceCream 20.0 10\n4 Cookie 15.0 15\n4 MangoFloat 20.0 12\n4 Chocolate 30.0 6\n");
        fclose(fp);
    } else {
        fclose(fp);
    }

    fp = fopen("inventory.txt", "r");
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
    FILE *fp = fopen("products.txt", "r");
    if (fp == NULL) return;

    Product *list = NULL;
    int count = 0;
    Product temp;

    fseek(fp, 0, SEEK_SET);
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

    if (count == 0) {
        printf("The vending machine is completely empty!\n");
        free(list);
        return;
    }

    int storeCounts[MAX_STORES] = {0};
    int maxItemsInAnyStore = 0;
    for (int i = 0; i < count; i++) {
        if (list[i].storeId >= 1 && list[i].storeId <= MAX_STORES) {
            storeCounts[list[i].storeId - 1]++;
        }
    }
    for (int i = 0; i < MAX_STORES; i++) {
        if (storeCounts[i] > maxItemsInAnyStore) {
            maxItemsInAnyStore = storeCounts[i];
        }
    }
    //code

    const char *storeNames[MAX_STORES] = {
        "DRINKS", "FRUITS", "MEALS", "DESSERTS"
    };

    int targetStore = -1;

    // Menu to select the store
    while (1) {
        store_select:
        printf("============================================\n");
        printf("             VENDING STORE MENU             \n");
        printf("============================================\n");
        for (int i = 0; i < MAX_STORES; i++) {
            printf("  [%d] Store %d: %s\n", i + 1, i + 1, storeNames[i]);
        }
        printf("  [0] Go BACK to Main Menu\n");
        printf("--------------------------------------------\n");
        printf(" Enter Store Number (0-4): ");

        if (scanf("%d", &targetStore) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Use numbers.\n\n");
            continue;
        }

        if (targetStore == 0) {
            free(list);
            return; 
        }

        if (targetStore < 1 || targetStore > MAX_STORES) {
            printf("Invalid Store Choice. Try again.\n\n");
            continue;
        }

        break; 
    }

    // Inside the Selected Store View
    printf("\n============================================\n");
    printf("         WELCOME TO STORE %d: %s         \n", targetStore, storeNames[targetStore - 1]);
    printf("============================================\n");
    printf("%-15s %-12s %-8s\n", "PRODUCT NAME", "PRICE", "STOCK");
    printf("--------------------------------------------\n");

    int itemsInStore = 0;
    for (int i = 0; i < count; i++) {
        if (list[i].storeId == targetStore) {
            printf("%-15s P%-11.2f (%02d)\n", list[i].name, list[i].price, list[i].stock);
            itemsInStore++;
        }
    }

    if (itemsInStore == 0) {
        printf(" (This store is currently out of stock)\n");
    }
    printf("============================================\n");

    char targetItem[NAME_LEN];
    int qty;

buy_prompt:
    printf("\n--- Purchasing from Store %d ---\n", targetStore);
    printf("Enter Product Name (or type 'back' to change stores): ");
    scanf("%s", targetItem);

    if (strcmp(targetItem, "back") == 0) {
        printf("\n");
    }

    printf("Enter Quantity (or type '0' to change stores): ");
    if (scanf("%d", &qty) != 1) {
        while (getchar() != '\n');
        goto buy_prompt;
    }

    if (qty == 0) {
        printf("\n");
        goto store_select; // Jump back to store select section
    }

    if (qty < 0) {
        printf(" Quantity cannot be negative.\n");
        goto buy_prompt;
    }

    float studentMoney = getStudentMoney();
    int found = 0;

    for (int i = 0; i < count; i++) {
        if (list[i].storeId == targetStore && strcmp(list[i].name, targetItem) == 0) {
            found = 1;
            float totalCost = list[i].price * qty;

            if (list[i].stock < qty) {
                printf(" Transaction Failed: Not enough stock available.\n");
                goto buy_prompt;
            }
            else if (studentMoney < totalCost) {
                printf("\n===================================\n");
                printf("        TRANSACTION FAILED!        \n");
                printf("===================================\n");
                printf("Reason:           Insufficient Funds\n");
                printf("Student Money:    P%.2f\n", studentMoney);
                printf("%s Total Cost: P%.2f\n", targetItem, totalCost);
                printf("Shortage Amount:  P%.2f\n", totalCost - studentMoney);
                printf("===================================\n");
                goto buy_prompt;
            }
            else {
                float previousMoney = studentMoney;
                list[i].stock -= qty;
                studentMoney -= totalCost;

                fp = fopen("products.txt", "w");
                for (int j = 0; j < count; j++) {
                    fprintf(fp, "%d %s %.2f %d\n", list[j].storeId, list[j].name, list[j].price, list[j].stock);
                }
                fclose(fp);

                char newOrder[100];
                sprintf(newOrder, "%s(x%d)(Store%d)", list[i].name, qty, targetStore);
                updateInventory(studentMoney, newOrder);

                printf("\n===================================\n");
                printf("       PURCHASE SUCCESSFUL!        \n");
                printf("===================================\n");
                printf("Item:             %s (x%d)\n", targetItem, qty);
                printf("Initial Money:    P%.2f\n", previousMoney);
                printf("%s Total Cost: P%.2f\n", targetItem, totalCost);
                printf("-----------------------------------\n");
                printf("Remaining Money:  P%.2f\n", studentMoney);
                printf("===================================\n");
            }
            break;
        }
    }
    
    if (!found) {
        printf(" Product '%s' not found in Store %d.\n", targetItem, targetStore);
        goto buy_prompt;
    }

    free(list); 
}

void viewInventory() {  
    FILE *ifp = fopen("inventory.txt", "r");
    printf("============================================\n");
    printf("              STUDENT INVENTORY           \n");
    printf("============================================\n");

    if (ifp == NULL) {
        printf("Inventory records missing.\n");
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
    printf("\nEnter '0' to go back to Main Menu: ");
    if (scanf("%d", &backChoice) != 1 || backChoice != 0) {
        while (getchar() != '\n');
        goto inventory_back;
    }
}